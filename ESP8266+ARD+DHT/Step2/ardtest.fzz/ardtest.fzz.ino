#include <SoftwareSerial.h>

SoftwareSerial module(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  module.begin(9600);
}

void loop() {
 if (module.available()) { //ESP has data to send thru software serial
    while(module.available()) {
     Serial.write((char)module.read());     //Write data from ESP to Serial port or COM port
   }
 }

 if(Serial.available()) { //USer is entering data into COM port window
   delay(1000); //Wait so that buffer fills up before the data is sent to ESP
   String command="";
   while(Serial.available()) { 
     command+=(char)Serial.read();
   }
   module.println(command); // write data to ESP thru software serial
 }
}
