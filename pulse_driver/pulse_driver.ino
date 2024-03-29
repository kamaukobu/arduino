unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 500;  //the value is a number of milliseconds
const byte ledPin = 10;    //using the built in LED
int pulse = 0;

int period1 = 1000;
unsigned long time_now = 0;

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


     //time_now = millis();
   
    Serial.print("COUNT: ");
    Serial.println(pulse);
    
    delay(1000);

   
   // while(millis() < time_now + period1){
        //wait approx. [period] ms
   // }
  
}
