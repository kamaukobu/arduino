float f1,f2,f3,f4;
String string1,string2,string3,string4,string5;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
f1 = 10.00;
f2 = 20.75;
f3 = 54.09;
f4 = 89.47;
}

void loop() {
  // put your main code here, to run repeatedly:

;

string1 = String(f1);
string2 = String(f2);
string3 = String(f3);
string4 = String(f4);
string5 = string1 + string2 + string3 + string4;

Serial.println("string1 is " + string1 + 'V');
Serial.println("string2 is " + string2 + 'A');
Serial.println("string3 is " + string3 + 'W' + 'h');
Serial.println("string4 is " + string4 );
Serial.println("string5 is " + string5 );
delay(2000);


}
