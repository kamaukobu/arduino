#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,7);


void setup() {
Serial.begin(9600);
while(!Serial);
// put your setup code here, to run once:
mySerial.begin(9600);
delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
if(mySerial.available()>0)
{
  Serial.write(mySerial.read());
}
if(Serial.available()>0)
{
  mySerial.write(Serial.read());
}
}



