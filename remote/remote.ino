

#include <IRremote.h>


int SER_Pin = 8;   //pin 14 on the 75HC595
int RCLK_Pin = 9;  //pin 12 on the 75HC595
int SRCLK_Pin = 10; //pin 11 on the 75HC595

int RECV_PIN = 2; // pin 0 Infrarojo

int on[8] ;
int pinActual;
unsigned long last = millis();

IRrecv irrecv(RECV_PIN);
decode_results results;


//How many of the shift registers - change this
#define number_of_74hc595s 1 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];





void setup(){
  
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  //reset all register pins
  clearRegisters();
  writeRegisters();
  
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  
  for (int i=0; i<9; i++)
  {
    on[i]=0;  
  }
  
}               

//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

int pinEncencer(decode_results *results)
{
   Serial.print(results->value);
   switch (results->value)
   {
     case  16195807:     
       return 0;  
       
     case  16228447:     
       return 1;  

     case  16212127:     
       return 2;  

     case  16191727:     
       return 3;  

     case  16224367:     
       return 4;  

     case  16208047:     
       return 5;  

     case  16199887:     
       return 6;  

     case  16232527:     
       return 7; 

     case 1785214238: 
      return 8; //Prendemos la tele
   }
  
}

void loop(){

if (irrecv.decode(&results)) {


    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) {
      
      pinActual = pinEncencer(&results);
      if (pinActual >= 0 && pinActual <=8)
      {
        on[pinActual] = !on[pinActual];      
        setRegisterPin(pinActual, on[pinActual] ? HIGH : LOW);
        writeRegisters();          
      }
    }
    last = millis();      
    irrecv.resume(); // Receive the next value
  }

}
