#define BLYNK_TEMPLATE_ID "TMPL3Tw8L1Ig1"
#define BLYNK_TEMPLATE_NAME "IoT based Home"
#define BLYNK_AUTH_TOKEN   "uJBUa7KoOOPz7aRqhAFPggA9c3JTu5hQ"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#include <DHT.h>

char AUTH[] = "uJBUa7KoOOPz7aRqhAFPggA9c3JTu5hQ"; //Auth code sent via Email
char WIFI_SSID[] = "vivo T2x 5G"; //Wifi name
char WIFI_PASS[] = "anime09812";  //Wifi Password

#define Buzzer D3 //Buzzer    
#define DHTPIN D4          // D4
#define DHTTYPE DHT11     // DHT 11
// define the GPIO connected with Relays and switches
#define Relay1  D2 //GPIO 16
#define Relay2  D1 //GPIO 5
//#define Relay3  D2 //GPIO 4
//#define Relay4  D3 //GPIO 0

//#define Switch3 10 //SD3
//#define Switch4 9  //SD2
#define Switch1 14 //D5
#define Switch2 12 //D6

//#define wifiLed D4 //GPIO 2

int load1, load2;
int wifiFlag = 1;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer
  bool isconnected = Blynk.connected();
  if (isconnected == false) {wifiFlag = 1;
   // digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
  if(wifiFlag==1){wifiFlag = 0;
  update_blynk();
  }  
   // digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 //Serial.println(h);
 //Serial.println(t);
 if(t>35){
  
digitalWrite(Buzzer,HIGH);
delay(300);
 }
 else{
  digitalWrite(Buzzer,LOW);
delay(300);
 }

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}
 
void setup() {  
Serial.begin(9600);

pinMode(Switch1, INPUT_PULLUP);
pinMode(Switch2, INPUT_PULLUP);
pinMode(Buzzer,OUTPUT);
digitalWrite(Buzzer,LOW);

dht.begin();
EEPROM.begin(512);
load1 = EEPROM.read(1);
load2 = EEPROM.read(2);
//load3 = EEPROM.read(3);
//load4 = EEPROM.read(4);

pinMode(Relay1, OUTPUT); 
pinMode(Relay2, OUTPUT); 

digitalWrite(Relay1,LOW);
digitalWrite(Relay2,LOW);
//pinMode(Relay3, OUTPUT); 
//pinMode(Relay4, OUTPUT);

Relays();

//pinMode(wifiLed, OUTPUT);

WiFi.begin(WIFI_SSID, WIFI_PASS);
timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
timer.setInterval(1000L, sendSensor);
Blynk.config(AUTH);
delay(100);
}

void loop() {

if (WiFi.status() != WL_CONNECTED){
    //Serial.println("WiFi Not Connected");
  }
  else{
    //Serial.println("WiFi Connected");
    Blynk.run();
  }
 
if(wifiFlag==0){with_internet();}
           else{without_internet();}

 timer.run(); // Initiates SimpleTimer
 
}

BLYNK_WRITE(V0){
load1 = param.asInt();
Relays();
}

BLYNK_WRITE(V1){
load2 = param.asInt(); 
Relays();
}


void with_internet(){
     if(digitalRead(Switch1) == HIGH){
      load1 = !load1;
      Relays(); 
      Serial.println("s1  pressed");
      update_blynk();
      delay(300); 
    }
else if(digitalRead(Switch2) == HIGH){
      load2 = !load2;
      Relays(); 
      Serial.println("s2  pressed");
      update_blynk();
      delay(300);
    }

}

void without_internet(){
     if(digitalRead(Switch1) == LOW){

      load1 = !load1;
      Relays(); 
      delay(300); 
    }
else if(digitalRead(Switch2) == LOW){
      load2 = !load2;
      Relays(); 
      delay(300);
    }

}

void update_blynk(){
Blynk.virtualWrite(V0, load1);  
Blynk.virtualWrite(V1, load2);
}

void Relays(){
digitalWrite(Relay1, load1);  
digitalWrite(Relay2, load2);
write_eeprom();
}

void write_eeprom(){
EEPROM.write(1, load1);
EEPROM.write(2, load2); 
EEPROM.commit(); 
}
