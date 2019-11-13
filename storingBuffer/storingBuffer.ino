#include <SoftwareSerial.h>
SoftwareSerial GPRS(10, 11); //11 = TX, 12 = RX
String buffer; //port
String parsed = "" ,sender = " ";
int count=0;
int i = 0; //if i = 0, send SMS.


void clearBufferArray(){
  // function to clear buffer array
  
    buffer = " ";
    parsed = " ";
    sender = " ";    // clear all index of array with command NULL
    
  }  


void setup() {
   pinMode(5, OUTPUT);
  GPRS.begin(9600); // the GPRS baud rate
  Serial.begin(9600); // the Serial port of Arduino baud rate.
  GPRS.write("AT+CMGF=1\r"); //sending SMS in text mode
    delay(3000);
    Serial.println("AT+CMGF=1\r");  
     delay(3000);     
     GPRS.write("AT+CMGD=1\r"); //sending SMS in text mode
    delay(3000);
    Serial.println("AT+CMGD=1\r");  
     delay(3000);     
    GPRS.write("AT+CNMI=2,2,0,0,0\r"); // phone number
     delay(3000);
    Serial.println("AT+CNMI=2,2,0,0,0\r");
    delay(3000);
  Serial.println("Meter ready");
   delay(3000);

}

void loop() {
  if (GPRS.available()) {
    // if date is coming from softwareserial port ==> data is coming from GPRS shield
    while(GPRS.available()) {
      // reading data into a string
   
     buffer = GPRS.readString();
      Serial.println("Message Received");

     
    
      
       delay(5000);

       parsed = buffer.substring(buffer.indexOf('*') + 1,buffer.indexOf('#') );
       sender =  buffer.substring(buffer.indexOf('"') + 1,buffer.indexOf(',') - 1 );
           
        break;
      
       
    }



  }

  Serial.println(parsed);
   Serial.println(sender);
   delay(2000);
  
    if (parsed == "connect" && sender == "+254701069036"){
      digitalWrite(5,HIGH);
      
    }

    else {
      digitalWrite(5,LOW);
    }
  
    // if no data transmission ends, write buffer to hardware serial port
  // clearBufferArray();
    // call clearBufferArray function to clear the stored data from the array
     
}

