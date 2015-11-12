
#include "DHT.h"
#include <LiquidCrystal.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11

// include the library code:

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);
YunServer server;

int getHumedad(){
  float h = dht.readHumidity();
  if (isnan(h)){
    Serial.println("Error en la lectura del sensor DHT!");
    return 0;
  }
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");
  return int(h);
}

int getTemperatura(){
  float t = dht.readTemperature();
  if (isnan(t)){
    Serial.println("Error en la lectura del sensor DHT!");
    return 0;
  }
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C ");
  return int(t);
}

void escribirLCD(int h, int t, String idSensor){
  String primeraLinea = "Hum:" + String(int(h)) + "%," + "Tmp:" + String(int(t)) + "*C";
  String segundaLinea = "Sensor:"+ idSensor;
  lcd.setCursor(0,0);
  lcd.print(primeraLinea);
  lcd.setCursor(0, 1);
  lcd.print(segundaLinea);
}

void process_REST(YunClient client,int h, int t) {
   String command = client.readStringUntil('/');
   command.trim();
   Serial.println(command);
   if (command == "temperatura") {
      client.println(t);  
   }
   if (command == "humedad"){
      client.println(h);
   }
 
}

void iniciarAPIREST(int h, int t){
  YunClient client = server.accept();
  if (client) {
      Serial.println("API REST EN PROCESO");
      process_REST(client,h,t);
      client.stop();
   }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Proyecto Arduino");
  lcd.setCursor(0, 1);
  lcd.print("Daniel Correa");
  dht.begin();
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  delay(2000);
  int h = getHumedad();
  int t = getTemperatura();
  escribirLCD(h,t,"DHT11-01");
  iniciarAPIREST( h,  t);
  
}

