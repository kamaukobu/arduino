
void setup() {
Serial.begin(9600);
}
void loop() {
double milfunc = millis();
Serial.print("millis function=");
Serial.println(milfunc);
delay(1000);
}
