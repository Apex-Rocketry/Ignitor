  

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   2 // determinar pino de conexão do módulo 
#define CSN_PIN 10

const byte slaveAddress[5] = {'R','x','A','A','A'}; 


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio



int sendtest = 6; // pinos de testes
int sendign = 3;

const int ignitorpin = 5;
const int testpin = 9;// the number of the pushbutton pin

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long prevMillisignition;
unsigned long txIntervalMillis = 250; // send once per second
unsigned long intervalcancel = 3000;


int ignitorstate = LOW ;
int teststate = LOW;
void setup() {
    digitalWrite(3, HIGH); // leds
    pinMode(6, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(6, HIGH);
    Serial.begin(9600);
    Serial.print("Transmitter ready");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
    delay(1000);
    digitalWrite(3, LOW);
    digitalWrite(6, LOW);

    
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
    digitalWrite(3, LOW);
    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
    Serial.print("Sending ignition signal, ");
    
    if (rslt) {
         Serial.println("ignition activated");
         prevMillisignition = millis();
         digitalWrite(3, HIGH);
         delay(5000);
         digitalWrite(3, LOW);
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
    Serial.print("Sending test signal, ");
     if (teste) {
         Serial.println("Test successful");
          digitalWrite(6, HIGH);
          delay(250);
         prevMillisignition = millis();
         digitalWrite(6, LOW);
    }
    else {
        Serial.println("Test signal not received by igniter");
         //digitalWrite(3, HIGH);
         delay(500);
    }
}
//================
