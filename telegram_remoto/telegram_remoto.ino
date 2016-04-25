#include <IRremote.h>

IRsend irsend;

int khz = 38;
unsigned int turnOn[]  = {3900,4050,400,2050,450,2000,450,2000,450,2000,500,1000,450,1050,450,2000,450,1050,450,2000,450,1050,450,2000,450,1050,450,1000,500,1000,450,1050,450,1050,450,2000,450,2000,450,1050,450,2000,450,1050,450,2000,450,1050,450,2000,450};
unsigned int turnOff[] = {3900,4050,400,2000,450,2050,400,2050,450,2000,450,1050,450,1050,400,2050,400,1100,400,2050,450,1050,400,2050,400,1100,450,1000,400,1100,400,1100,400,1050,550,1900,450,2100,350,1150,350,2100,400,1050,400,2100,400,1050,400,2050,400};

unsigned long roku_home = 0x5743C03F;
unsigned long roku_netflix = 0x5743D22D;
unsigned long roku_ok = 0x574354AB;



void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
 
}

void loop() {
  
  serialEvent();
}


void serialEvent() {

  if (Serial.available() > 0){
    char inByte = Serial.read();
    switch(inByte)
    {
      case 'o':
            digitalWrite(13, HIGH);
            break;

      case 'a':
            digitalWrite(13, LOW);
            break;

      case  't':
            irsend.sendRaw(turnOn, sizeof(turnOn) / sizeof(turnOn[0]), khz);  
            break;

      case  'v':
            irsend.sendRaw(turnOff, sizeof(turnOff) / sizeof(turnOff[0]), khz);  
            break;

      case  'n':
            irsend.sendNEC(roku_home, 32);            
            delay(5000);            
            irsend.sendNEC(roku_netflix, 32);            
            delay(10000);            
            irsend.sendNEC(roku_ok, 32);
            delay(5000);                                    
            break;

      case  'h':
            irsend.sendNEC(roku_home, 32);
            delay(10000);            
            break;


    }

  }
}

