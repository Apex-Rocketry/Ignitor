

// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;
int relaypin = 3;
int led1 = 4;
int led2 = 5;
int k = 0;

bool launch = true;
unsigned long lauchtime;
//===========

void setup() {
  Serial.begin(9600);
  pinMode (relaypin,OUTPUT);
  pinMode (led1,OUTPUT);
  pinMode (led2,OUTPUT);
  digitalWrite(relaypin, LOW);
  Serial.println("Receiver ready");
  digitalWrite(led1, HIGH);            // LED que indica que o receptor está pronto
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();
 
}

//=============

void loop() {
    getData();
    protocol();
}

//==============

void getData() {
  if ( radio.available() ) {
    radio.read( &dataReceived, sizeof(dataReceived) );  
    newData = true;
  }
}

void protocol() {
  if (newData == true) {
    if (dataReceived[0] == 'I') {  
      dataReceived[0] = 'N';
      newData = false;
      ignition();

      Serial.println("Restarting");
      delay(5000);
      digitalWrite(relaypin, LOW);
      Serial.println("Ready");
    }
  }
}


void ignition() {
  Serial.println("Igniting");
    digitalWrite(relaypin, HIGH);

    while(k < 10){
      digitalWrite(led2, HIGH); // acionando ignitor
      delay(300);
      digitalWrite(led2, LOW); 
      delay(300);
      k++;
  }
}
