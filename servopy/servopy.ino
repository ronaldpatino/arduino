#include <SoftwareServo.h> 

SoftwareServo servoLR;  // create servo object to control a servo 
SoftwareServo servoUD;  // create servo object to control a servo 

#define pinServoLR A0
#define pinServoUD A1

int speedLR = 1;
int speedUD = 1;
int limits[2] = {2,180};  // set limitations (min/max: 0->180)
boolean refresh = false;  // toggle refresh on/off


void setup() 
{ 
  Serial.begin(9600);

  // attaches the servo on pin to the servo object
  servoLR.attach(pinServoLR);  
  servoUD.attach(pinServoUD);  

  // init angle of servo inbetween two limitations
  servoLR.write((limits[1]-limits[0])/2); 
  servoUD.write((limits[1]-limits[0])/2); 
   
} 


void loop() 
{ 
  // refresh angle
  int angleLR = servoLR.read();
  int angleUD = servoUD.read();

  // change direction when limits
  if (angleLR >= limits[1]){
     speedLR = speedLR;
     angleLR = limits[1];
  }
  
  if(angleLR <= limits[0]) { 
    speedLR = speedLR;
    angleLR = limits[0];
  }
  
  if (angleUD >= limits[1]){
    speedUD = speedUD;
    angleUD = limits[1];
  }

  if(angleUD <= limits[0]) { 
    speedUD = speedUD;
    angleUD = limits[0];
  }


  if (Serial.available() > 0){
    char inByte = Serial.read();    
    switch(inByte)
    {
      case 'X':
            servoLR.write(angleLR - speedLR);
            break;

      case 'x':
            servoLR.write(angleLR + speedLR);
            break;

      case  'Y':
            servoUD.write(angleUD + speedUD);
            break;

      case  'y':
            servoUD.write(angleUD - speedUD);
            break;

    }
    
  }

  Serial.println(angleLR);

  // set refresh one time / 2
  refresh = refresh ? false : true;
  if (refresh) SoftwareServo::refresh();

  
}
