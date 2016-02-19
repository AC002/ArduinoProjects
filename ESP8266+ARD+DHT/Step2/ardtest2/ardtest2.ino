#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3);

void setup()
{
  Serial.begin(9600);
  esp8266.begin(4800); // your esp's baud rate might be different
}

void loop()
{
 esp8266.println("AT");
 if(esp8266.available()) // check if the esp is sending a message 
 {
 while(esp8266.available())
  {
    // The esp has data so display its output to the serial window 
    char c = esp8266.read(); // read the next character.
    Serial.write(c);
  }  
 }
}
