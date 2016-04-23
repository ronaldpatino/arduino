#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

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
#define BOUNCE_DELAY 200

const char LEAD[] PROGMEM  = {"LEAD"};
const char CLEAN[] PROGMEM  = {"CLEAN"};
const char CRUNCH[] PROGMEM  = {"CRUNCH"};
const char* const CHANNELS[] PROGMEM = {CLEAN, LEAD, CRUNCH};

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
int pageActive = 1;
int presetActive = 0;
int patchActive = 1;

char buffer[30];

int currentSwitch = 0;
int bypassState = LOW;     // state of bypass pedal
int bypassState2 = LOW;     // state of bypass pedal

int pedalActiveFlash = 10; // Delay for flash when pedal is pressed

// set the LCD address to 0x27 for a 16 chars 2 line display
// Set the pins on the I2C chip used for LCD connections:
//                      addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

void setup() {

  lcd.begin(16,2);
  lcd.setBacklightPin(3, POSITIVE);  
  lcd.setBacklight(HIGH);  
  lcd.setCursor(0,0); //Start at character 4 on line 0  
  lcd.print("*RPG MIDI STOMP*"); 
  lcd.setCursor(0,1); //Start at character 4 on line 0  
  lcd.print("**VERSION :1.0**");


  //  Set MIDI baud rate:
  Serial.begin(31250);
  
  
  // Setup Switches and activation LEDs
  for( currentSwitch = 0; currentSwitch < MAX_SWITCH; currentSwitch++ ) {   
    // Set pin for switch
    pinMode( switches[currentSwitch], INPUT_PULLUP );         
    
    pinMode( leds[currentSwitch], OUTPUT );             // Set pin for LED
    flashPin( leds[currentSwitch], 100 ); // Flash LED
  }
  delay(2000);  
  patchPreset(pageActive,presetActive, patchActive);
  midiProg( 0xC0, buttonA );
  turnLedOn(LEDA);


  
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
              pageActive++;
              patchPreset(pageActive, presetActive, patchActive);
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
              pageActive--;
              patchPreset(pageActive, presetActive, patchActive);
            }
            flashPin( leds[1], pedalActiveFlash );
          break;

          case BUTTON_A:            
            midiProg( 0xC0, buttonA );
            turnLedOn(LEDA);
            turnLedOff(LEDB);
            turnLedOff(LEDC);
            presetActive = 0;
            patchActive = pageActive;
            patchPreset(pageActive, presetActive, patchActive);
          break;

          case BUTTON_B:
            midiProg( 0xC0, buttonB );
            turnLedOff(LEDA);
            turnLedOn(LEDB);
            turnLedOff(LEDC);
            presetActive = 1;
            patchActive = pageActive;
            patchPreset(pageActive, presetActive, patchActive);
          break;

          case BUTTON_C:
            midiProg( 0xC0, buttonC );
            turnLedOff(LEDA);
            turnLedOff(LEDB);
            turnLedOn(LEDC);
            presetActive = 2;
            patchActive = pageActive;
            patchPreset(pageActive, presetActive, patchActive);
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


void patchPreset(int page, int preset, int patch){
  lcd.clear();
  delay(10);
  
  lcd.setCursor(0,0); //Start at character 4 on line 0  
  lcd.print("PATCH:");
  lcd.setCursor(6,0);
  lcd.print(patch); 


lcd.setCursor(8,0); //Start at character 4 on line 0  
  lcd.print("PAGE:");
  lcd.setCursor(13,0);
  lcd.print(page); 
  
  
  strcpy_P(buffer, (char*)pgm_read_word(&(CHANNELS[preset]))); // Necessary casts and dereferencing, just copy.
  
  lcd.setCursor(0,1); //Start at character 4 on line 0  
  lcd.print("PRESET:");
  lcd.setCursor(8,1);
  lcd.print(buffer); 
}

