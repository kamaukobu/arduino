#include <SoftwareSerial.h>
SoftwareSerial mySerial(7,8);
char inchar; //Will hold the incoming character from the Serial Port.
boolean text=false;
const int pirOutput = 5;   
const int LED1 = 9;
int LED2 = 11;

void setup()
{
  pinMode(pirOutput, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(pirOutput, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
 Serial.begin(9600);
 mySerial.begin(9600);
 delay(3000); // give time for GSM module to register on network etc.
 Serial.println("AT+CMGF=1\r"); // set SMS mode to text
 mySerial.println("AT+CMGF=1\r"); 
 delay(200);
 Serial.println("AT+CNMI=2,2,0,0,0\r"); // set module to send SMS data to serial out upon receipt 
 mySerial.println("AT+CNMI=2,2,0,0,0\r");
 mySerial.println("AT+CMGD=1,4"); // delete all SMS
 delay(200);
}
void loop(){
  if (digitalRead(pirOutput)== HIGH)
   {
    digitalWrite(LED1, HIGH);
    mySerial.println("OK");
    delay(1000);
    mySerial.print("AT+CMGF=1\r\n");
    delay(500);
    mySerial.print("AT+CMGS=\"");
    mySerial.print("+254719753154");
    mySerial.print("\"\r\n");
    delay(2000);
    mySerial.print("Alert!! Intruder has been Detected.");
    mySerial.write(0x1A);
    mySerial.write(0x0D);  // Carriage Return in Hex
    mySerial.write(0x0A);  // Line feed in Hex
    digitalWrite(LED1,LOW);
    for (int x=0; x<30000; x++) {
      delay(1);
     if (x<30000)
     REPLY();    
    if(x>=30000 && text==false)
    {Serial.println("Yay.");
    }
     else 
     {
       digitalWrite(LED2, LOW);
     }
  }
   }
 }

 void REPLY()
{
  //If #a1b1c1d1 comes as sms, all led's should light up.
     if(mySerial.available() >0)
  {
text=!text;
 inchar= mySerial.read(); 
 if (inchar=='#')
   {
   delay(10);
   inchar= mySerial.read(); 
 //first led
   if (inchar=='a')
     {
   delay(10);
   inchar= mySerial.read();
 if (inchar=='0')
   {
  digitalWrite(LED2, LOW);
   } 
 else if (inchar=='1')
   {
   digitalWrite(LED2, HIGH);
   delay (15000);
   digitalWrite(LED2, LOW);
   }
     }
}
  }
}

