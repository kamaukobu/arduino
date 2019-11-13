#include <SoftwareSerial.h>

//#include <EEPROM.h>

SoftwareSerial gsm(10,11);
char received;
void setup() {
  // put your setup code here, to run once:
//Serial.begin(9600);
//EEPROM.write(10,0);

Serial.begin(9600);
gsm.begin(9600);
delay(20000);

gsm.write("ATE0\r\n");
delay(2000);
gsm.write("AT+CMGF=1\r\n");
delay(2000);
gsm.write("AT+CMNI=2,2,0,0,0\r\n");
gsm.println("AT+CMGD=1,4\r\n");   //delate all sms
delay(200);
}

void loop() {
while(gsm.available()>0){
received = gsm.read();
  Serial.print(received);
}
}
