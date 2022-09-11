// Flame Sensor Module
#include <SoftwareSerial.h> 
String agAdi = "Guest";                 // We write the name of our network here.    
String agSifresi = "anuj@98184"; 
int rxPin = 10;                                               //ESP8266 RX pini
int txPin = 11; 
String ip = "184.106.153.149";
int LED = 13; // Use the onboard Uno LED
int isFlamePin = 7;  // This is our input pin
int isFlame = HIGH;  // HIGH MEANS NO FLAME
int v,v1;
SoftwareSerial esp(rxPin, txPin);
void setup() {
  pinMode(LED, OUTPUT); // put onboard LED as output
  pinMode(isFlamePin, INPUT); //flame sensor should be input as it is giving data
  Serial.begin(9600); //begin Serial communication
  Serial.begin(9600);  // We are starting our communication with the serial port.
  Serial.println("Started");
  esp.begin(115200);                                          // We are starting serial communication with ESP8266.
  esp.println("AT");                                         // We do the module control with the AT command.
  Serial.println("AT  sent ");
  while(!esp.find("OK")){                                     // We wait until the module is ready.
    esp.println("AT");
    Serial.println("ESP8266 Not Find.");
  }
  Serial.println("OK Command Received");
  esp.println("AT+CWMODE=1");                                 // We set the ESP8266 module as a client.
  while(!esp.find("OK")){                                     // We wait until the setting is done.
    esp.println("AT+CWMODE=1");
    Serial.println("Setting is ....");
  }
  Serial.println("Set as client");
  Serial.println("Connecting to the Network ...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    // We are connecting to our network.
  while(!esp.find("OK"));                                     // We wait until it is connected to the network.
  Serial.println("connected to the network.");
  delay(1000);
  
}

void loop() {
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           // We connect to Thingspeak.
  if(esp.find("Error")){                                      // We check the connection error.
    Serial.println("AT+CIPSTART Error");
  }
  isFlame = digitalRead(isFlamePin);//Readd the data gien by the flame sensor
  if (isFlame== LOW)  //if it is low
  {
    v=0;
    Serial.println("FLAME, FLAME, FLAME"); //Print Flame Flame
    digitalWrite(LED, HIGH);  //LED on
  }
  else                           //if not
  {      
    v=1;                         
    Serial.println("no flame"); //print no  flame
    digitalWrite(LED, LOW);    //off the LED
  }
  String veri = "GET https://api.thingspeak.com/update?api_key=MHVH1EJHJTLC2WE2";   // Thingspeak command. We write our own api key in the key part.                                   
  veri += "&field1=";
  veri += String(v);// The temperature variable we will send
  veri += "&field2=";
  veri += String(v1);// The moisture variable we will send
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
  esp.println(veri.length()+2);
  delay(2000);
  if(esp.find(">")){                                          // The commands in it are running when ESP8266 is ready..
    esp.print(veri);                                          // We send the data.
    Serial.println(veri);
    Serial.println("Data sent.");
    delay(1000);
  }
  Serial.println("Connection Closed.");
  esp.println("AT+CIPCLOSE");                                // we close the link
  delay(1000);  
  
}
