
/* code written and compiled  by: Kamau Paul
 *                                Electrical and Electronics Engineering(with Telecommunications)
 *
 */
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial gsm(10,11);  //declaration for gsm communication at port
LiquidCrystal lcd(9,8,7,6,5,4);  //pinouts for the lcd ( RS,EN,D4,D5,D6,D7 )

String buffer = " "; //port
String parsed = " " ,sender = " " , recharge = " ";
int count=0;
int adc1 = 0 ,adc2 = 0 ;  //sensor converted values
const int addr = 0, addr2 = 25,addr3 = 500;           //EEPROM address for storing saved energy
float  savedEnergy = 0.0, units = 0.0 ;   //
volatile float power = 0.0, remainingUnits = 0.0;
float requestedEnergy = 0.0,requestedEnergy1 = 0.0;
float measuredVoltage = 0.0,realVoltage = 0.0,realCurrent = 0.0,measuredCurrent = 0.0, OriginalEnergy = 0.0;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000 ;  //the value is a number of milliseconds
const byte ledPin = 13;    //using the LED connected at PMW 3
const byte relayPin = 12;  //using the relay connected at PMW 2
const byte buzzer = 3;
int pulse = 0;



void clearBufferArray(){
  // function to clear buffer array
  
    buffer = " ";
    parsed = " ";
    sender = " ";
    recharge = " ";   
    
  }  
  

void setup() {
  
   Serial.begin(9600);
   gsm.begin(9600);  //initialize sim800l at 9600 baud
   lcd.begin(16,2);  //initialize 16x2 lcd
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

 

//lcd.setCursor(1,0);
//lcd.println("GSM meter...");
lcd.clear();
lcd.setCursor(0,0);
lcd.println("Initializing...");
Serial.println("Initializing....");  
delay(5000);
  
     gsm.write("ATE0\r");
     delay(2000);
     gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
      delay(3000);    
     gsm.write("AT+CNMI=2,2,0,0,0\r"); // routing all messages to serial port
     //Serial.println("AT+CNMI=2,2,0,0,0\r");
      delay(3000);
    Serial.println("Initialized.");
      delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Initialized");
      delay(2000);
    lcd.clear();
  // EEPROM.put(addr3, 100.00);
    startMillis = millis();
    EEPROM.get(addr, OriginalEnergy);   //Acquire stored energy value.
   // EEPROM.get(addr3, remainingUnits);
    
    clearBufferArray();
    

}

void loop() {
  

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    pulse += 1;
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

float displayEnergy = 0.0 ,displayPower = 0.0 ;
EEPROM.get(addr,displayEnergy);
EEPROM.get(addr2,displayPower);
lcd.setCursor(0,0);
lcd.print(displayEnergy);
lcd.print("kWh");
lcd.print(",");
lcd.print((displayPower / 1000));
lcd.print("kW");
lcd.setCursor(0,1);
lcd.print(realVoltage);
lcd.print("V");
lcd.print(", ");
lcd.print(realCurrent);
lcd.print("A");





float serialDisplay = 0.0;
EEPROM.get(addr3, serialDisplay);

Serial.print("Energy:");

EEPROM.get(addr,displayEnergy);
Serial.print(displayEnergy);
Serial.println(" kWh");
Serial.print("COUNT: ");
Serial.println(pulse);
Serial.print("units: ");
Serial.println(serialDisplay);
   
   
  }

  adc1 = analogRead(A0);
  adc2 = analogRead(A1);
  adc2 = adc2 - 407;
adc2 = float(adc2);
measuredVoltage = (adc1 * 5.0) / 1024.0;  //convert adc converted value to physical parameter
realVoltage = measuredVoltage / 0.01;   //sensitivity of voltage sensor is 10mV for every Volt(based on the voltge divider circuit)

measuredCurrent = (adc2 * 5.0) / 1024.0;   //convert adc converted value to physical parameter
realCurrent = (measuredCurrent - 0.5) / 0.066;  // in Amps...sensitivity of current sensor is 100mV for every Ampere.(Read from the datasheet)
if (realCurrent < 0 )
{
  realCurrent = realCurrent * -1;
}

  if ( pulse == 10 ){

    power = realVoltage * realCurrent;

    savedEnergy += power / 3600  * 0.010;   // in Wh...meter updates reading energy every one and a half minutes
    
    EEPROM.put(addr, (savedEnergy + OriginalEnergy));
    
    EEPROM.put(addr2, power);
   float remainingUnits1 = 0.0;
    EEPROM.get(addr3, remainingUnits);
    remainingUnits1 = remainingUnits - (realVoltage * realCurrent * 0.010 / 3600) ;
    
    EEPROM.put(addr3, remainingUnits1);

    
 float alarm = 0.0 ;
 EEPROM.get(addr3, alarm);
 
    pulse = 0;  //reset the timer to start update time.
  }
  

if (remainingUnits = 0.0){
   digitalWrite(relayPin,HIGH);
                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                    gsm.print("Dear customer,your tokens have depleted...please top up."); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);
    
                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sent!");
    delay(2000);
      
    clearBufferArray(); 
}


    if (gsm.available()) {
    // if data is coming from softwareserial port,  data is coming from gsm module
      while(gsm.available()) {
      // reading data into a string
   
                  buffer = gsm.readString();
                  delay(3000);
                  Serial.println("Message Received");
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.println("Received");
                  delay(2000);

       parsed = buffer.substring(buffer.indexOf('*') + 1,buffer.indexOf('#') );
       sender =  buffer.substring(buffer.indexOf('"') + 1,buffer.indexOf(',') - 1 );
       recharge = buffer.substring(buffer.indexOf('$') + 1,buffer.indexOf('%') );
           
        break;
    }
Serial.print("Message:");
Serial.println(parsed);
Serial.print("Sender:");
Serial.println(sender);
Serial.print("Units:");
Serial.println(recharge);
    delay(1000);
  }



  
    if (parsed == "connect" && sender == "+254701069036" ){    //try a while(1) loop to see if you can dynamically display values on lcd for the stated time then go back to idle state.
     
       digitalWrite(relayPin,LOW);
                
                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                    gsm.print("Dear customer,your Smart meter has been connected"); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);
    
                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sent!");
    delay(2000);
      
    clearBufferArray(); 
      
    }

    else if(parsed == "disconnect" && sender == "+254701069036" )
    {
    digitalWrite(relayPin,HIGH);
   
    digitalWrite(buzzer,!digitalRead(buzzer));
    delay(50);
    digitalWrite(buzzer,!digitalRead(buzzer));
    delay(950);
      
                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                    gsm.print("Dear customer,your Smart meter has been disconnected. Contact customercare for enquiries"); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);
    
                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
    lcd.clear();
    lcd.setCursor(0,0);    
    lcd.print("sent!");
    delay(2000);
      
    
       
    clearBufferArray();
    }

    else if (parsed == "status" && sender == "+254701069036" && recharge != NULL)
    {

                            EEPROM.get(addr,requestedEnergy1);
                            

                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                           
                          
                     
                    gsm.print("Energy report from meter:\nSystem Voltage: ");
                    gsm.print(realVoltage);
                     gsm.print("V,\nCurrent: ");
                    gsm.print(realCurrent);
                     gsm.print("A,\nPower: ");
                    gsm.print(realVoltage * realCurrent / 1000);
                     gsm.print("kW,\nEnergy consumed:  ");
                    gsm.print(requestedEnergy1);
                    gsm.print("kWh,\nRemaining tokens: ");
                    gsm.print(remainingUnits);
                     gsm.print("\nvisit www.powerabc.com");
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
      
                    Serial.print("Energy consumed is: ");
                    Serial.print(requestedEnergy1);
                    Serial.println("Wh");
                    Serial.print("voltage is: ");
                    Serial.print(realVoltage); 
                    Serial.println("V");
                    Serial.print("current is: ");
                    Serial.print(realCurrent);
                    Serial.println("A");
                     Serial.print("Token remaining: ");
                    Serial.println(remainingUnits);
                    
                    
     clearBufferArray();

    }

  else if (parsed == "status" && sender == "+254723089064")
    {

                            EEPROM.get(addr,requestedEnergy1);
                            

                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254723089064");
                    gsm.print("\"\r");
                    delay(1000);
                           
                          
                     
                    gsm.print("Energy report from meter:\nSystem Voltage: ");
                    gsm.print(realVoltage);
                     gsm.print("V,\nCurrent: ");
                    gsm.print(realCurrent);
                     gsm.print("A,\nPower: ");
                    gsm.print((power / 1000));
                     gsm.print("kW,\nEnergy consumed:  ");
                    gsm.print(requestedEnergy1);
                    gsm.print("kWh,\nRemaining tokens: ");
                    gsm.print(remainingUnits);
                     gsm.print("\nvisit www.powerabc.com");
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
      
                    Serial.print("Energy consumed is: ");
                    Serial.print(requestedEnergy1);
                    Serial.println("Wh");
                    Serial.print("voltage is: ");
                    Serial.print(realVoltage); 
                    Serial.println("V");
                    Serial.print("current is: ");
                    Serial.print(realCurrent);
                    Serial.println("A");
                    Serial.print("Token remaining: ");
                    Serial.println(remainingUnits);
                    
                    
    }



    

     else if( parsed == "reset" && sender == "+254701069036" )
    {
float hold = 0.0;
      EEPROM.get(addr3, hold);
      
        for( int i = 0; i<=EEPROM.length();i++ ){
                EEPROM.read(i) !=0 ;
              }
              
      OriginalEnergy = 0.0 ;
      EEPROM.put(addr3, hold);

                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode   
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);      
                    gsm.print("Admin:customer meter has been reset."); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode   
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254723089664");
                    gsm.print("\"\r");
                    delay(1000);      
                    gsm.print("Dear customer,your meter has been reset."); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);
    Serial.println("SMS sent successfully");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sent");
    delay(2000);
    Serial.println("Smart meter has been reset");
 
    clearBufferArray();
    }
  
  else if (parsed == "load" && sender == "+254701069036" )
    {

      int Recharge = 0;
      float fRecharge = 0.0, remainingUnits = 0.0 ,remainingUnits2 = 0.0,requestedUnits = 0.0;
      Recharge = recharge.toInt();
      EEPROM.get(addr3, remainingUnits);
      fRecharge = float(Recharge);

      remainingUnits2 = remainingUnits + fRecharge ;

      EEPROM.put(addr3, remainingUnits2);

      
      
                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                        
                    gsm.print("Dear customer, your meter has been recharged with "); // message
                    gsm.print(Recharge);
                    gsm.print(" tokens. Your new token balance is ");
                    EEPROM.get(addr3, requestedUnits);
                    gsm.print(requestedUnits);
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                    Serial.println("sending message to +254701069036"); 
                    delay(3000);    
                    Serial.println("SMS sent successfully");
                    delay(2000);
                    
      
                    
     clearBufferArray();
     Recharge = 0;
     

    }
    
 else{
  
  clearBufferArray();
 }
}
