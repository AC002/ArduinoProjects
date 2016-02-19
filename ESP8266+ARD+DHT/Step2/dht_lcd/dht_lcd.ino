// Software Serial is needed for communication with WIFI ESP
#include <SoftwareSerial.h>
// First is rx, 2nd tx! Do not change! Watch out for connections
// ------------- DONT CHANGE! ---------------
SoftwareSerial mySerial(2,3);
// ------------------------------------------

// Include dht22, our temperature and humidity logger
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;

// For interrupts, every 60s check Temp and Hum
int timer1_counter;
int secondsOverload = 0;

/*
 * "Serial" is only FOR DEBUGING!
 * To send commands to ESP8266 send them using baud 9600
 */
 
void setup() {  

  // For interrupt
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
  
  Serial.begin(9600);
  Serial.println("Wifi is starting!");
  mySerial.begin(9600);
  while(!wyslij("AT","OK",1000)){
    Serial.println("Error talking to ESP");
    delay(1000);
  }
  Serial.println("Talking to ESP...");
  if(wyslij("AT+CWMODE=1","OK",300))
    Serial.println("CWMODE COMMAND OK");
  if(wyslij("AT+CIPMODE=0","OK",300))
    Serial.println("CIPMODE COMMAND OK");
  if(wyslij("AT+CIPMUX=1","OK",300))
    Serial.println("CIPMUX COMMAND OK");
  if(wyslij("AT+CWJAP=\"AlfaSqD24GHz\",\"5407417askug5tw\"","OK",5000))
    Serial.println("Connected to AlfaSqD!");
  if(wyslij("AT+CIPSERVER=1,80","OK",1000))
    Serial.println("Server online");

  mySerial.println("AT+CIFSR");
  Serial.println(mySerial.readString());
  
  mySerial.println("AT");
  dht.begin();
  if(!checkTemperature()) Serial.print("Error reading DHT22");
}

// Array of clients
char client[5];
char buffor[50];
String webPage;

  void loop()
{
  // If you want to use commands from serial monitor
  // Uncomment lines below!
  // If serial info came, throw it for user to see
  /*if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
    */

  while(mySerial.available() > 0){
    if(mySerial.find("+IPD,")){
      mySerial.readBytesUntil(',',client,1);
      Serial.println();
      Serial.print("Zapytanie ID: ");
      Serial.println(client[0]);

      webPage = "<html><head><title>Temperature Readout</title></head><body><p>Humidity: "+ String(h) +"</p><p>Temperature: "+ String(t)+"</p></body></html>";
      sprintf(buffor,"AT+CIPSEND=%c,%d",client[0],webPage.length());

      if(wyslij(buffor,">",100))
      Serial.println("Sending request");
      if(wyslij(webPage,"OK",100))
      Serial.println("Data send");

      sprintf(buffor,"AT+CIPCLOSE=%c",client[0]);
      if(wyslij(buffor,"OK",100))
      Serial.println("Connection closed!");
    }
  }
}


ISR(TIMER1_COMPA_vect)      // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  secondsOverload += 1;
  
  // If we counted 60 seconds, check temperature and humidity
  if (secondsOverload >= 60) {
    secondsOverload = 0;
    if(!checkTemperature()) Serial.print("Error reading DHT22");
  }
}

boolean checkTemperature(){

    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();

  // Any read from DHT 22 failed?
    if (isnan(h) || isnan(t)) {
    return 0;
  } else{
    float hic = dht.computeHeatIndex(t, h, false); 
    Serial.print("H:");
    Serial.print(h);
    Serial.println(" %\t");
    Serial.print("T: ");
    Serial.print(t);
    Serial.println(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
    Serial.println("");
  }
return 1;
}

boolean wyslij(String Komenda_AT, char *Odpowiedz_AT, int czas_czekania) {

  mySerial.println(Komenda_AT);
  delay(czas_czekania);
  while (mySerial.available() > 0) {
    if (mySerial.find(Odpowiedz_AT))
      return 1;
  }
  return 0;

}

