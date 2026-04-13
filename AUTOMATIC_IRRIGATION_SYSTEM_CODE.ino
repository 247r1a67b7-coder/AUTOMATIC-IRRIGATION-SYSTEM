#define BLYNK_TEMPLATE_ID "TMPL3OlM1vtEV"
#define BLYNK_TEMPLATE_NAME "IRRIGATION SYSTEM"
#define BLYNK_AUTH_TOKEN "i-Ey_ePjpJTqO-pnVtXi2C3-7y-jrCyy"

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

#define BLYNK_PRINT Serial
 
char auth[] = "i-Ey_ePjpJTqO-pnVtXi2C3-7y-jrCyy";    // authuntifaction id
char ssid[] = "project02";   //wifi name
char pass[] = "project02";   //wifi password

#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include<DHT.h>
DHT dht(15,DHT11);
float t,h;
int a=0,b=0;

#define soil 35
int soil_sense;

#define buzzer 19
#define pump 18

String status="NORMAL";

void setup()
{
  Serial.begin(9600);

  pinMode(soil,INPUT);

  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);

  pinMode(pump,OUTPUT);
  digitalWrite(pump,LOW);
  
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.print("CONNECTING TO...");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(1000);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  Blynk.config(auth);
  delay(500);

  Serial.print(WiFi.localIP());
  lcd.clear();
  lcd.print(" WIFI CONNECTED ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1500);

  lcd.clear();
  lcd.print("   IRRIGATION   ");
  lcd.setCursor(0,1);
  lcd.print("     SYSTEM     ");
  delay(1000);
  
  dht.begin();
}
void loop()
{
  Blynk.run();

  b=b+1;

  if(b>50)
  {
    b=0;
  }

  soil_sense=analogRead(soil);
  soil_sense=map(soil_sense,0,4096,100,0);
  Serial.print("SOIL : ");Serial.println(soil_sense);

  t=dht.readTemperature();
  h=dht.readHumidity();
  Serial.print("T : ");Serial.println(t);
  Serial.print("H : ");Serial.println(h);

  lcd.clear();
  lcd.print("TEMP: ");lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("HUMI: ");lcd.print(h);
  digitalWrite(2,HIGH);delay(300);digitalWrite(2,LOW);delay(200);
  lcd.clear();
  lcd.print("SOIL : ");lcd.print(soil_sense);
  lcd.setCursor(0,1);
  lcd.print("STATUS: ");lcd.print(status);
  digitalWrite(2,HIGH);delay(300);digitalWrite(2,LOW);delay(200);

  if(soil_sense <= 30)
  {
    status="LOW";
    a=a+1;

    if(a>=10)
    {
      a=10;
    }
    digitalWrite(pump,HIGH);
  }
  else
  {
    status="NORMAL";
    digitalWrite(pump,LOW);
  }
  if(a==2)
  {
    Blynk.logEvent("soil_alert");
    Blynk.virtualWrite(V0,t);
    Blynk.virtualWrite(V1,h);
    Blynk.virtualWrite(V2,soil_sense);
  }

  if(b==50)
  {
    Blynk.virtualWrite(V0,t);
    Blynk.virtualWrite(V1,h);
    Blynk.virtualWrite(V2,soil_sense);
  }
  
  digitalWrite(2,HIGH);delay(300);digitalWrite(2,LOW);delay(200);
}
