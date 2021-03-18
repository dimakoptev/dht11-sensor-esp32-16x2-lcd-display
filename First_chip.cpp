#include "DHTesp.h"         //dht11 sensor + esp32 tugi
#include <LiquidCrystal.h>  //16x2 ekraan

#define DHTpin 13           //D13 of ESP32 DevKit
#define contrast 32         //Ekraani kontrast
#define ledPin  35          //Ekraani heledus
//TODO ekraani heledus saada automaatseks ehk pin 35 või mõni muu

DHTesp dht;
//ühendamise eesmärgil on kirjeldatud pin-id displayl olevate nimetuste järgi
const int rs = 33, en = 25, d4 = 26, d5 = 27, d6 = 14, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long startMillis;        //millisekundid, et vältida delay() funktsiooni
unsigned long currentMillis;
const unsigned long period = 2000;

void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);

  delay(1000);  //Delay, et ei kirjutaks jama esimese asjana
  Serial.println();
  Serial.println("DHT11 niiskuse ja temperatuuri Sensor \n\n");
  
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to pin 13
  pinMode(contrast, OUTPUT);        //set pin to OUTPUT
  digitalWrite(contrast, LOW);      //output low power
  pinMode(ledPin, OUTPUT);          //set pin to OUTPUT, hetkel ei tööta
  digitalWrite(ledPin, HIGH);       //output high power, hetkel ei tööta

  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();                 //Uuendame millisekundite aega
  delay(dht.getMinimumSamplingPeriod());    //Programm loeb millisekundeid default 2000 sellepärast on ülevalpool ka sama aeg määratud
  float humidity = dht.getHumidity();       //Küsime väärtused ja float, et saada komakohaga väärtusi
  float temperature = dht.getTemperature();
    
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.print("Status = ");            //Kirjutame endale konsoolile, et näha väärtust
    Serial.print(dht.getStatusString());
    
    Serial.print("   Heatindex = ");      //Kirjutame endale konsoolile, et näha väärtust
    Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
    
    Serial.print("   Current humidity = ");//Kirjutame endale konsoolile, et näha väärtust
    Serial.print(humidity);
    Serial.print("%  ");
    
    Serial.print("temperature = ");       //Kirjutame endale konsoolile, et näha väärtust
    Serial.print(temperature); 
    Serial.println("C  ");
  
    lcd.clear();
    //Choosing the first line and row
    lcd.setCursor(0,0);
    lcd.print("Temp: ");  //Kirjutame teksti float, et saada kaks nr peale komakohta
    lcd.print((float)temperature);
    lcd.print(" C");
    
    //Choosing the second line and first row
    lcd.setCursor(0,1);
    lcd.print("Niiskus: ");  //Kirjutame teksti
    lcd.print((int)humidity);
    lcd.print(" %");
    
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current state.
  }
}

 
