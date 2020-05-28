  

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   2 // determinar pino de conexão do módulo 
#define CSN_PIN 10

const byte slaveAddress[5] = {'R','x','A','A','A'}; 


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int led1 = 3;
int led2 = 4;
int led3 = 5;

const int ignitorpin = 6;// botão para iniciar ignição
const int testpin = 7;// botão para iniciar teste

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long prevMillisignition;
unsigned long txIntervalMillis = 250; // send once per second
unsigned long intervalcancel = 3000;


int ignitorstate = LOW ;
int teststate = LOW;
void setup() {
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(ignitorpin, INPUT);
    pinMode(testpin, INPUT);
    
    Serial.begin(9600);
    Serial.println("Transmitter ready");
    radio.begin();
    digitalWrite(led1, HIGH);    // Led que indica que o transmissor está ligado
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count        
    radio.openWritingPipe(slaveAddress);         
    delay(1000);
    
}

//===================================================================================================

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
      ignitorstate = digitalRead(ignitorpin); //verifica se tem conexão
      teststate = digitalRead(testpin);
      
      if (ignitorstate == HIGH) {
        if (currentMillis - prevMillisignition >= intervalcancel) { //only send signal again after determined interval which will result in canceling the ignition
          ignition();
         }
      }
      if (teststate == HIGH) {
         testignition();
      }
      prevMillis = millis();
   }
}

//====================

void ignition() { //send ignition command
    char dataToSend[2] = "I";
    digitalWrite(led2, LOW);
    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );       
    Serial.println("Sending ignition signal...");
    
    if (rslt) {
         Serial.println("ignition activated");
         prevMillisignition = millis();
         digitalWrite(led2, HIGH);
         delay(5000);
         digitalWrite(led2, LOW);
    }
    else {
        Serial.println("ignition failed, signal not received by igniter");
    }
}

void testignition() { // send test communuication command
    char dataToSend[2] = "T";
      
    Serial.println(dataToSend);
    bool teste;
    teste = radio.write( &dataToSend, sizeof(dataToSend) );
    Serial.println("Sending test signal...");
     if (teste) {
         Serial.println("Test successful");
          digitalWrite(led3, HIGH);
          delay(2000);
         prevMillisignition = millis();
         digitalWrite(led3, LOW);
    }
    else {
        Serial.println("Test signal not received by igniter");
         delay(500);
    }
}
//================
