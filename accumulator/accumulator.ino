#include <EEPROM.h>




unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds
const byte ledPin = 10;    //using the built in LED
int pulse = 0,volt = 10,amp = 20;
double energy = 0.0;
int count = 0;
int period1 = 1000;
unsigned long time_now = 0;
String Buffer, message;

void setup()
{
  Serial.begin(9600);  //start Serial in case we need to print debugging info
  pinMode(ledPin, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    pulse += 1;
   
    
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
 
energy = volt * amp * pulse;
   
    //Serial.println(pulse);
    //Serial.println(energy);
    //delay(1000);

if (Serial.available() > 0) {
    // if date is coming from softwareserial port ==> data is coming from GPRS shield
    
      // reading data into char array

      while(Serial.available() < 0 ){
        
      }
      Buffer = Serial.read();  



       if (Buffer == '*'){

        Serial.println(energy);
       }
       else if (Buffer == '$'){
        Serial.println("disconnected");
       }
}


   
}







