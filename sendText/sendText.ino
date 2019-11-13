#include <SoftwareSerial.h>

SoftwareSerial gsm(10,11);


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);

   
 gsm.begin(9600);  //initialize sim800l at 9600 baud
delay(15000);
gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
      delay(1000);
    Serial.println("AT+CMGF=1\r");       
    gsm.print("AT+CMGS=\""); // phone number******all my problems here!!!!!!!!!!
      gsm.print("+254701069036");
       gsm.print("\"\r");
      
      delay(1000);
    Serial.println("AT+CMGS=\"+254701069036\"\r");       
    gsm.print("hey."); // message
       gsm.write(0x1A);
     gsm.write(0x0D);
    
      gsm.write(0x0A);
    
    //send a Ctrl+Z (end of the message)
      delay(1000);
    Serial.println("SMS sent successfully");
    //lcd.setCursor(2,0);
    //lcd.print("Message sent");
      delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:

}
