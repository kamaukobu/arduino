
/* code designed and written by: Eng. Kamau Paul
 *                               Electrical and Electronics Engineering(Telecommunications)
 *                               on: 22nd July 2018(completion)
 */
#include <LiquidCrystal.h> //driver header file for display
#include <EEPROM.h> //driver header file for memory access
#include <SoftwareSerial.h> //driver header file for serial communication

SoftwareSerial gsm(10,11);  //declaration for gsm communication at port
LiquidCrystal lcd(9,8,7,6,5,4);  //pinouts for the lcd ( RS,EN,D4,D5,D6,D7 )

volatile String sms_buffer = " "; //port monitor
String parsed = " " ,sender = " " , recharge = " ";
int count=0;
int adc1 = 0 ,adc2 = 0 ;  //initialization of sensor converted values
const int addr = 0, addr2 = 25,addr3 = 500;           //EEPROM addresses for storing program parameters
float  savedEnergy = 0.0, units = 0.0 ;   //
volatile float power = 0.0, remainingUnits = 0.0;
float requestedEnergy = 0.0,requestedEnergy1 = 0.0;
float measuredVoltage = 0.0,realVoltage = 0.0,realCurrent = 0.0,measuredCurrent = 0.0, OriginalEnergy = 0.0;
unsigned long startMillis;  // timer declaration
unsigned long currentMillis;
const unsigned long period = 1000 ;  //the value is a number of milliseconds
const byte ledPin = 13;    //using the LED connected at PMW 3
const byte relayPin =  12;  //using the relay connected at PMW 2
const byte buzzer = 3;
int pulse = 0;

void clearBufferArray(void);

void setup() {

   Serial.begin(9600); //default baud for the serial monitor is 9600(can be changed at the console)
   gsm.begin(9600);  //initialize sim800l at 9600 baud
   lcd.begin(16,2);  //initialize 16x2 lcd
   lcd.clear();
  
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(relayPin, OUTPUT);
    pinMode(buzzer, OUTPUT);

 

lcd.setCursor(0,0);
lcd.println("SMARTCoM Co. LTD");
lcd.setCursor(0,1);
lcd.println("Welcome customer");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.println("Initializing....");
Serial.println("Initializing..");  
delay(5000);
  
     gsm.write("ATE0\r");
     //delay(2000);
     gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
    //  delay(3000);     
     gsm.write("AT+CNMI=2,2,0,0,0\r"); // routing all messages to serial port
     //Serial.println("AT+CNMI=2,2,0,0,0\r");
     // delay(3000);
    Serial.println("Initialized!!!");
     
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Unit Initialized");
      delay(4000);
    lcd.clear();
  // EEPROM.put(addr3, 100.00);    //topping up tokens manually
    startMillis = millis();
    EEPROM.get(addr, OriginalEnergy);   //Acquire stored energy value.
    EEPROM.get(addr3, remainingUnits);  //Acquire storedtokens value.
}

void loop() {
  

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed...Set to one second(frequency of the energy meter)
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    pulse += 1;
    startMillis = currentMillis;  //VERY IMPERATIVE!!! to save the start time of the current LED state. This section is referred to as a Finite State Machine or FSM ....  A Timestamp

float displayEnergy = 0.0 ,displayPower = 0.0 ; 
EEPROM.get(addr,displayEnergy);
EEPROM.get(addr2,displayPower);
lcd.setCursor(0,0);
lcd.print(displayEnergy);
lcd.print("kWh");
lcd.print(",");
lcd.print((power / 1000));
lcd.print("kW");
lcd.setCursor(0,1);
lcd.print(realVoltage);
lcd.print("V");
lcd.print(", ");
lcd.print(realCurrent);
lcd.print("A");





float serialMonitorDisplay = 0.0;   // declaration of a LOCAL variable
EEPROM.get(addr3, serialMonitorDisplay);

Serial.print("Energy:");

EEPROM.get(addr,displayEnergy);
Serial.print(displayEnergy);
Serial.println(" kWh");
Serial.print("voltage:");
Serial.print(realVoltage);
Serial.println(" V");
Serial.print("current:");
Serial.print(realCurrent);
Serial.println(" A.");
Serial.print("PULSE COUNT: ");
Serial.println(pulse);
Serial.print("units: ");
Serial.println(serialMonitorDisplay);
   
   
  }

  adc1 = analogRead(A0);
  adc2 = analogRead(A1);
  
adc2 = float(adc2);
measuredVoltage = (adc1 * 5.0) / 1024.0;  //convert adc converted value to physical parameter
realVoltage = measuredVoltage / 0.02;   //sensitivity of voltage sensor is 10mV for every Volt(based on the voltge divider circuit)

//measuredCurrent = (adc2 * 5.0) / 1024.0;   //convert adc converted value to physical parameter
realCurrent = 0.564 * (adc2 - 512.0);  // in Amps...sensitivity of current sensor is 40mV for every Ampere.(Read from the datasheet)
if (realCurrent < 0.00 )
{
  realCurrent *= -1 ; //getting rid of negative current readings
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("No  Load");
}

  if ( pulse == 10 ){

    power = realVoltage * realCurrent * 100;

    savedEnergy += power / 3600  * 0.010;   // in Wh...meter updates reading energy every TEN SECONDS
    
    EEPROM.put(addr, (savedEnergy + OriginalEnergy));
    
    EEPROM.put(addr2, power);
   float remainingUnits1 = 0.0;   //declaration of a LOCAL vaariable
    EEPROM.get(addr3, remainingUnits);
    remainingUnits1 = remainingUnits - (realVoltage * realCurrent * 0.010 / 3600) ;
    
    EEPROM.put(addr3, remainingUnits1);

    

 
    pulse == 0;  //reset the timer to start update time.
  }
  
       float alarm = 0.0 ;
       EEPROM.get(addr3, alarm);
       


/*if (remainingUnits < 0.0){
                    digitalWrite(relayPin,HIGH);
                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254701069036");
                    gsm.print("\"\r");
                    delay(1000);
                    gsm.print("Dear customer,your tokens have depleted,and the house disconnected...please top up."); // message
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);
    
                    Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
    Serial.println("Smart meter has been disconnected");
   
                    delay(10000);
      
 
    
   
}
else{
  digitalWrite(relayPin,HIGH);
}

*/

    if (gsm.available()) {
    // if data is coming from softwareserial port,  data is coming from gsm module
      while(1) {
      // reading data into a string
   
                  sms_buffer = gsm.readString();
                  delay(2000);

       parsed = sms_buffer.substring(sms_buffer.indexOf('*') + 1,sms_buffer.indexOf('#') );
       sender =  sms_buffer.substring(sms_buffer.indexOf('"') + 1,sms_buffer.indexOf(',') - 1 );
       recharge = sms_buffer.substring(sms_buffer.indexOf('$') + 1,sms_buffer.indexOf('%') );
                   Serial.println("Receiving...");
                      lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Receiving SMS...");
                    delay(5000);
           
        break;
    }
     Serial.println("Received SMS.");
                      lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.println("Received SMS...");
                    delay(5000);
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
    
                    Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
                      Serial.println("Smart meter has been connected.");
                       lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Welcome back.");
                    delay(10000);
      
  
   clearBufferArray(); 
    return 0;
      
    }

    else if(parsed == "disconnect" && sender == "+254775849462" )
    {
    digitalWrite(relayPin,HIGH);
   
      
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
    
                    Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
    Serial.println("Smart meter has been disconnected");
     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Disconnected!!");
                    delay(10000);
      
    
       clearBufferArray(); 
    return 0;
    }

    else if (parsed == "status" && sender == "+254775849462" )
    {
float serialMonitorDisplay = 0.0;
                            EEPROM.get(addr,requestedEnergy1);
                            EEPROM.get(addr3, serialMonitorDisplay);

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
                    gsm.print(serialMonitorDisplay);
                     gsm.print("\nvisit www.powerabc.com");
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                   Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
    Serial.println("Smart meter report has been sent");
                               lcd.clear();
                               lcd.setCursor(0,0);
                               lcd.print("SMARTCoM Report:");
                                delay(10000);
                                 lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Voltage:");
                     lcd.setCursor(0,1);
                      lcd.print(realVoltage);
                       lcd.println("V");
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Current:");
                     lcd.setCursor(0,1);
                      lcd.print(realCurrent);
                       lcd.println("A");
                    delay(10000);
                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Energy:");
                     lcd.setCursor(0,1);
                      lcd.print(requestedEnergy1);
                       lcd.println("kWh");
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Remaining units:");
                     lcd.setCursor(0,1);
                      lcd.print(serialMonitorDisplay);
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("End of Report.");
                     lcd.setCursor(0,1);
                      lcd.print("SMARTCoM Co. LTD");
                    delay(10000);

                    
                    
                    
      
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
    //return 0;

    }

  else if (parsed == "status" && sender == "+254715073471")
    {
float  serialMonitorDisplay = 0.0;
                          
                             EEPROM.get(addr,requestedEnergy1);
                            EEPROM.get(addr3, serialMonitorDisplay);
                            

                    gsm.write("AT+CMGF=1\r"); //sending SMS in text mode
                    delay(1000);
                    gsm.print("AT+CMGS=\""); // phone number*****all my problems gone.
                    gsm.print("+254715073471");
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
                    gsm.print(serialMonitorDisplay);
                     gsm.print("\nvisit www.powerabc.com");
                    gsm.write(0x1A);
                    gsm.write(0x0D);
                    gsm.write(0x0A);
                    delay(1000);

                    Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
    Serial.println("Smart meter report has been sent");
                               lcd.clear();
                               lcd.setCursor(0,0);
                               lcd.print("SMARTCoM Report:");
                                delay(10000);
                                 lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Voltage:");
                     lcd.setCursor(0,1);
                      lcd.print(realVoltage);
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Current:");
                     lcd.setCursor(0,1);
                      lcd.print(realCurrent);
                    delay(10000);
                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Energy:");
                     lcd.setCursor(0,1);
                      lcd.print(requestedEnergy1);
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Remaining units:");
                     lcd.setCursor(0,1);
                      lcd.print(serialMonitorDisplay);
                    delay(10000);

                          lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("End of Report.");
                     lcd.setCursor(0,1);
                      lcd.print("SMARTCoM Co. LTD");
                    delay(10000);

                    
                  
                    
      
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
    return 0;    
                    
    }



    

     else if( parsed == "reset" && sender == "+254775849462" )
    {
float hold = 0.0;
      EEPROM.get(addr3, hold);
      
        //for( int i = 0; i <= EEPROM.length(); i++ ){
         //      EEPROM.write(i) = 0 ;
          //    }
              
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
Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
    Serial.println("Smart meter has been reset");
     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("reset success!!");
                    delay(10000);
 
    clearBufferArray(); 
    return 0;
    }
  
  else if (parsed == "load" && sender == "+254775849462" )
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
                  

                    Serial.print("sending message to ");
                    Serial.println(sender);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending SMS to");
                      lcd.setCursor(0,1);
                       lcd.print(sender);
                        delay(10000);    
                    Serial.println("SMS sent successfully");
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("sending success.");
                    delay(10000);
                     lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Recharged with ");
                      lcd.setCursor(0,1);
                      lcd.print(Recharge);
                    delay(10000);
                    lcd.clear();
                     lcd.setCursor(0,0);
                      lcd.print("Remaining Units");
                      lcd.setCursor(0,1);
                      lcd.print(requestedUnits);
                    delay(10000);
                   
                    
      
                    
    clearBufferArray();
     Recharge = 0;
     
return 0;
    }
    
 else{
                       
  
   clearBufferArray();
   return 0;
 }
}

void clearBufferArray(void){
// function to clear buffer array
    sms_buffer = "\0";
    parsed =  "\0";
    sender =  "\0";
    recharge =  "\0";   

    return void;
  }  
