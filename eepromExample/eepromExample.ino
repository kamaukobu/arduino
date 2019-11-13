/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 100;
float  savedEnergy = 0;
volatile float energy = 0;
volatile float requestedEnergy;
volatile int voltage;
unsigned long timer;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 500;  //the value is a number of milliseconds
const byte ledPin = 5;    //using the built in LED
int pulse = 0;

int period1 = 1000;
unsigned long time_now = 0;

void setup() {
  /** Empty setup. **/
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(ledPin, OUTPUT);
  startMillis = millis();

  for(int i = 0; i<=EEPROM.length();i++){

  EEPROM.write(i,0);
  }
  
}

void loop() {

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    pulse += 1;
   
    
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }


     //time_now = millis();
   
    Serial.print("COUNT: ");
    Serial.println(pulse);
    
    delay(1000);
  

  voltage = analogRead(A0) / 5;
 

  if (pulse == 100 ){

    energy = voltage * 100;

    savedEnergy += energy;
    
    EEPROM.put(addr, savedEnergy);
    
    pulse = 0;
  }

  
  /***
    Need to divide by 4 because analog inputs range from
    0 to 1023 and each byte of the EEPROM can only hold a
    value from 0 to 255.
  ***/
EEPROM.get(addr, requestedEnergy);
Serial.print("Energy consumed is: ");
Serial.println(requestedEnergy);

 
}
