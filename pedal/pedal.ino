//Define foot switches
#define MAX_SWITCH 5
#define SWITCH1 2
#define SWITCH2 3
#define SWITCH3 4
#define SWITCH4 5
#define SWITCH5 6

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_C 4

//Define Leds
#define LEDUP 11
#define LEDDOWN 12
#define LEDA 10
#define LEDB 9
#define LEDC 8


#define MIDI_CHANNEL 1

#define PAGE_JUMP 3
#define MAX_PRESETS 128
#define MIN_PRESETS 0
#define BOUNCE_DELAY 25


// Variables: 
int switches[5] = { SWITCH1,SWITCH2,SWITCH3,SWITCH4,SWITCH5 };
int switchState[5] = { HIGH, HIGH, HIGH, HIGH, HIGH}; 
int leds[5] = { LEDUP, LEDDOWN, LEDA, LEDB, LEDC};
int pageUp = 0; 
int pageDown = 2;
int buttonA = 0;
int buttonB = 1;
int buttonC = 2;
int tmpPageUp = 0; 
int tmpPageDown = 0; 

int currentSwitch = 0;
int bypassState = LOW;     // state of bypass pedal
int bypassState2 = LOW;     // state of bypass pedal

int pedalActiveFlash = 50; // Delay for flash when pedal is pressed


void setup() {

  //  Set MIDI baud rate:
  Serial.begin(31250);

  // Setup Switches and activation LEDs
  for( currentSwitch = 0; currentSwitch < MAX_SWITCH; currentSwitch++ ) {   
    // Set pin for switch
    pinMode( switches[currentSwitch], INPUT_PULLUP );         
    
    pinMode( leds[currentSwitch], OUTPUT );             // Set pin for LED
    flashPin( leds[currentSwitch], 100 ); // Flash LED
  }
  
  
  
}

void loop() {

  for( currentSwitch = 0; currentSwitch < MAX_SWITCH; currentSwitch++ ) {

    if((digitalRead(switches[currentSwitch]) != switchState[currentSwitch] )&&
        (switchState[currentSwitch] == HIGH)){

        switch( currentSwitch ) {

          case BUTTON_UP:
            tmpPageUp = PAGE_JUMP + pageUp;
            tmpPageDown = PAGE_JUMP + pageDown;

            if (tmpPageDown < MAX_PRESETS){
              pageUp = tmpPageUp;
              pageDown = tmpPageDown;
              buttonA = pageUp;
              buttonB = pageUp + 1;
              buttonC = pageDown;              
            }
            flashPin( leds[0], pedalActiveFlash );                    
          break;

          case BUTTON_DOWN:
            tmpPageUp = pageUp - PAGE_JUMP;
            tmpPageDown = pageDown - PAGE_JUMP;

            if (tmpPageUp >= MIN_PRESETS){
              pageUp = tmpPageUp;
              pageDown = tmpPageDown;
              buttonA = pageUp;
              buttonB = pageUp + 1;
              buttonC = pageDown;              
            }
            flashPin( leds[1], pedalActiveFlash );
          break;

          case BUTTON_A:            
            midiProg( 0xC0, buttonA );
            turnLedOn(LEDA);
            turnLedOff(LEDB);
            turnLedOff(LEDC);
            
          break;

          case BUTTON_B:
            midiProg( 0xC0, buttonB );
            turnLedOff(LEDA);
            turnLedOn(LEDB);
            turnLedOff(LEDC);
          break;

          case BUTTON_C:
            midiProg( 0xC0, buttonC );
            turnLedOff(LEDA);
            turnLedOff(LEDB);
            turnLedOn(LEDC);
          break;
                     
        } //Switch            
        delay( BOUNCE_DELAY );
    }//if
    switchState[currentSwitch] = digitalRead( switches[currentSwitch] );
  }//for
}//loop

void flashPin( int ledPin, int flashDelay ) {
  digitalWrite( ledPin, HIGH );
  delay( flashDelay );
  digitalWrite( ledPin, LOW );
}

void turnLedOn(int ledPin){
  digitalWrite( ledPin, HIGH );  
}

void turnLedOff(int ledPin){
  digitalWrite( ledPin, LOW );  
}


//  Send a two byte midi message  
void midiProg(char stat, int data ) {
  Serial.write(0xC0);
  Serial.write(data);
}

