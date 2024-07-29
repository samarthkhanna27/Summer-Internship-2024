#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);
#include "HX711.h"
#define calibration_factor 200000 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT 2
#define CLK  3
HX711 scale;
const int BUZZ=11;
const int KEY=7;
float vout=0;
int   flag1=0;
int   i=0;
void setup()  
{
  Serial.begin(9600);      
  mySerial.begin(9600);               // the GPRS baud rate    
  pinMode(BUZZ,OUTPUT);
  pinMode(KEY,INPUT);  
  digitalWrite(BUZZ,LOW);  
  digitalWrite(KEY,HIGH);    
  Serial.println("WELCOME");
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0  
wait:  
  if(digitalRead(KEY)==HIGH)
  {
   goto wait;
  }
 
}

void loop()                     // run over and over again
{
home:  
  vout=scale.get_units(); //scale.get_units() returns a float
  vout=(vout*454);
  Serial.println(vout);  
  delay(200);
  if(vout>=350)
  {
  flag1=0;  
  digitalWrite(BUZZ,LOW);  
  }
  if((vout>200)&&(vout<350))
  {
   if(flag1==0)
    {
    mySerial.begin(9600);    
    mySerial.print("AT+CMGF=1\n\r");
    delay(1000);
    mySerial.print("AT+CMGS="); // send the SMS number
    mySerial.print('"');
    mySerial.print("+918595617879");
    mySerial.println('"');
    delay(1000);    
    mySerial.print("ALERT PLS,SALINE IS ABOUT TO COMPLETE");  
    delay(500);
    mySerial.write(0x1A);
    mySerial.write(0x0D);
    mySerial.write(0x0A);  
     delay(3000);
     mySerial.begin(9600);    
    mySerial.print("AT+CMGF=1\n\r");
    delay(1000);
    mySerial.print("AT+CMGS="); // send the SMS number
    mySerial.print('"');
    mySerial.print("+919398835493");
    mySerial.println('"');
    delay(1000);    
    mySerial.print("ALERT PLS,SALINE IS ABOUT TO COMPLETE");  
    delay(500);
    mySerial.write(0x1A);
    mySerial.write(0x0D);
    mySerial.write(0x0A);  
     delay(3000);    
    flag1=1;
    }
          Send2things();
          if (mySerial.available())
          Serial.write(mySerial.read());  
          goto home;
  }
   if(vout<=200)
   {  
   digitalWrite(BUZZ,HIGH);      
   }

}
void Send2things()
{
  mySerial.println("AT");
  delay(1000);

  mySerial.println("AT+CPIN?");
  delay(1000);  

  mySerial.println("AT+CREG?");
  delay(1000);

  mySerial.println("AT+CGATT?");
  delay(1000);

  mySerial.println("AT+CIPSHUT");
  delay(1000);

  mySerial.println("AT+CIPSTATUS");
  delay(2000);

  mySerial.println("AT+CIPMUX=0");
  delay(2000);


 
  ShowSerialData();
 
  mySerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
 
 String str="GET http://api.thingspeak.com/update?api_key=CCIY0VZY41LS7LVG&field1=" + String(vout);
     
  mySerial.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();

  mySerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  mySerial.println();
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
 
}
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
