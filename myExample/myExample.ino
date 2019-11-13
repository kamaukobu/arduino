#include <LiquidCrystal.h>
#define num_samples 10
int sum = 0;
unsigned char sample_count=0;
float voltage=0.0;
LiquidCrystal lcd(12,11,10,9,8,7);
const int PT = A0;
const int CT = A1;
float current;
float power;

void setup() {
  // put your setup code here, to run once:
lcd.begin(20,4);
lcd.setCursor(5,1);
lcd.println("Welcome To");
lcd.setCursor(5,2);
lcd.println("Energy Meter Pro");
delay(5000);
lcd.clear();

constants();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setCursor(0,2);
ShowVoltage(9, 0, PT);
float vol = voltage;
ShowVoltage(9, 1, CT);
float Cur = voltage;
power = vol * Cur;
lcd.setCursor(7,2);
lcd.print(power); 
}

void constants(){
  lcd.setCursor(0,0);
  lcd.print("Voltage: ");
  lcd.setCursor(0,1);
  lcd.print("Current: ");
  lcd.setCursor(0,2);
  lcd.print("Power: ");
  lcd.setCursor(14,0);
  lcd.print("V");
  lcd.setCursor(14,1);
  lcd.print("A");
  lcd.setCursor(14,2);
  lcd.print("W");
} 

void ShowVoltage(int x,int y,unsigned int value){
  while(sample_count< num_samples)
  {
    sum += analogRead(value);
    sample_count++;
    delay(10);
  }
  voltage = ((float)sum / (float)num_samples * 5.00) / 1024.0;
  lcd.setCursor(x,y);
  lcd.print(voltage);
  sample_count = 0;
  sum = 0;
}

