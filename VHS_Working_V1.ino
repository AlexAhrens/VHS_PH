#include <Wire.h>
#include <AS_BH1750.h>
AS_BH1750 sensor;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 7

OneWire ourWire(ONE_WIRE_BUS); 

DallasTemperature DS18B20(&ourWire);

#include "DHT.h"                     
#define DHTPIN 4                     
#define DHTTYPE DHT11                 
DHT dht11(DHTPIN, DHTTYPE);          

#define Pumpe 8
#define Sensor 2

float botemp = 0.00;
float lux = 0.00;
int input = 0;
float bodenfeuchte = 0.00;
float p_feucht = 0.00;
float temp = 0.00;
float hum = 0.00;

int Pause = 1000;
int Pumpzeit = 10000;

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Protohaus &");
  lcd.setCursor(0,1);
  lcd.print("VHS Braunschweig");
  DS18B20.begin();
  sensor.begin();
  dht11.begin();
  pinMode(8,OUTPUT);
  pinMode(2,OUTPUT);
  delay(3000);
  lcd.clear();
}

void loop() {
  Bodentemp();
  Licht();
  Bodenfeuchte();
  Raum();
  Ausgabe();
  delay(Pause);
}
void Bodentemp()
{
  DS18B20.requestTemperatures();
  botemp = DS18B20.getTempCByIndex(0);  
}
void Licht()
{
  lux = sensor.readLightLevel();
}
void Bodenfeuchte()
{
  digitalWrite(Sensor,HIGH);
  delay(1000);
  input = analogRead(A0);
  bodenfeuchte = map(input,0,1023,0,100);
  p_feucht = (bodenfeuchte/75)*100;
  delay(500);
  digitalWrite(Sensor,LOW);
  
  if (p_feucht <= 50.00)
      {
        digitalWrite(Pumpe,HIGH);
        lcd.setCursor(15,1);
        lcd.print("1");
        delay(Pumpzeit);
        digitalWrite(Pumpe,LOW);
        lcd.setCursor(15,1);
        lcd.print("0");
      }
}
void Raum()
{
  temp = dht11.readTemperature();     
  hum = dht11.readHumidity();          
}
void Ausgabe()
{
  lcd.setCursor(0,0);
  lcd.print("R: ");
  lcd.print(temp,0);
  lcd.print("C ");
  lcd.print(hum,0);
  lcd.print("% ");
  lcd.print(lux,0);
  lcd.print("Lx");
  lcd.setCursor(0,1);
  lcd.print("P: ");
  lcd.print(botemp,0);
  lcd.print("C ");
  lcd.print(p_feucht,0);
  lcd.print("% ");
  lcd.print("P:");
}


