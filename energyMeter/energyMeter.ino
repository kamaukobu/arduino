/***
code written and compiled  by: Kamau Paul
                               Electrical and Electronics Engineering(Telecommunications)
***/
#include <TimeLib.h>

#include <time.h>

#include <LiquidCrystal.h>

#include <SoftwareSerial.h>

#include <EEPROM.h>

SoftwareSerial gsm(0,1);  //declaration for gsm communication

LiquidCrystal lcd(7,6,5,4,3,2);  //pinouts for the lcd

//initialisation of global variables

int adc1 = 0 ,adc2 = 0 ;  //sensor converted values
volatile double dataIn ;  //variables for working with EEPROM
float power = 0.0 ,measuredVoltage = 0.0 ,measuredCurrent = 0.0 ,realVoltage = 0.0 ,realCurrent = 0.0,powerplus = 0.0 ;
unsigned long int timer,seconds= (timer/1000000);
double energy;
 
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);  //initialize serial monitor at 9600 baud
  gsm.begin(9600);  //initialize sim800l at 9600 baud
  
  lcd.begin(20,4);  //initialize 20x4 lcd

  

while(!Serial){
  
}

delay(100);

lcd.setCursor(2,0);

lcd.println("Energy Meter Pro");


delay(5000);

lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
realVoltage = 0.0;
realCurrent = 0.0;

for(int i=0;i<1000;i++){
  adc1 += analogRead(A0);
  adc2 += analogRead(A1);
  delay(0.9);
}

adc1 = adc1/1000;
adc2 = adc2/1000;

measuredVoltage = adc1 * 5.0 / 1024.0;  //convert adc converted value to physical parameter

realVoltage= measuredVoltage / 0.05;   //sensitivity of voltage sensor is 50mV for every Volt(read from the datasheet)
measuredCurrent = adc2 * 5.0 / 1024.0;   //convert adc converted value to physical parameter

realCurrent = measuredCurrent / 0.06;  //sensitivity of current sensor is 60mV for every Ampere.(Read from the datasheet)

timer = micros();

power = (realVoltage * realCurrent ) / 1000;

powerplus += power;

energy =(realVoltage * realCurrent * seconds)  / 3600;  //in Wh 

dataIn = energy;

//EEPROM.update(0,dataIn);
//EEPROM.get(0,dataOut);


Serial.print("voltage : ");
Serial.print(realVoltage);
Serial.print("V");
Serial.print("\r\n");


Serial.print("current : ");
Serial.print(realCurrent);
Serial.print("A");
Serial.print("\r\n");


Serial.print("power : ");
Serial.print(powerplus);
Serial.print("KW");
Serial.print("\r\n");



Serial.print("timer : ");
Serial.print(seconds);
Serial.print("s");
Serial.print("\r\n");

Serial.print("Accumulative Energy Consumed : ");
Serial.print(dataIn);
Serial.print("Wh");
Serial.print("\r\n");
delay(2000);


  lcd.setCursor(0,0); // set the cursor at 1st col and 1st row
  lcd.print("Mains:");
  lcd.print(realVoltage);
  lcd.print("V,");
  lcd.print(realCurrent);
  lcd.print("A");
  
  lcd.setCursor(0,1);
  lcd.print("Power : ");
  lcd.print(power);
  lcd.print("W. ");
  
  lcd.setCursor(0,2);
  lcd.print("Energy : ");
  lcd.print(dataIn);
  lcd.print("W.");

  
  lcd.setCursor(0,3);
  lcd.print("Status : Online.");
  
}
