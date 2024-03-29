#include<ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library
#include <DHT.h> // dht11 temperature and humidity sensor library
#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_AUTH "FIREBASE_AUTH"
#define WIFI_SSID "WIFI_NAME"
#define WIFI_PASSWORD "PASSWORD"                                    //password of wifi ssid 
#define DHTPIN D4                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);                                                     
int myFan = D8;
int myPir = D5;
int myLed =D2;
void setup() {
    pinMode(myPir,INPUT);
    pinMode(myLed,OUTPUT);
pinMode(myFan,OUTPUT);
  Serial.begin(9600);
  delay(1000);                

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
    digitalWrite(myLed,LOW);
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();                                                               //Start reading dht sensor

}

void loop() {
  int pirState = 0;

  pirState = digitalRead(myPir);
    Serial.println("Pir state : ");
  Serial.println(pirState);
  Serial.println();
    if(pirState == 1)
    {
      
      digitalWrite(myLed,HIGH);
      Serial.println("LOL");
    } 
    else
      digitalWrite(myLed,LOW);
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
      if(t > 31) {
        digitalWrite(myFan,HIGH);
      }
      else {
        digitalWrite(myFan,LOW);  
      }
  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("Â°C ");
  String fireTemp = String(t) + String("°C");                                                     //convert integer temperature to string temperature
    delay(1000);
  Firebase.setString("/DHT11/Humidity", fireHumid);                                  //setup path and send readings
  Firebase.setString("/DHT11/Temperature", fireTemp);                                //setup path and send readings
   
}
