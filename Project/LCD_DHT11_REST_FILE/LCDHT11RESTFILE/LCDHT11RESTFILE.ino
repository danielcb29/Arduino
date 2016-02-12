
/**
 * Protyecto Dispositivos Ubicuos 
 * Daniel Correa Barrios
 * Escuela Politecnica
 * Universidad de Extremadura
 * 2015/16
 */

#include "DHT.h"
#include <LiquidCrystal.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <FileIO.h>
#include <Process.h>
//#include <SPI.h>
//#include <WiFi.h>

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11

// include the library code:

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);
YunServer server;
Process date_config;
Process date;
//Process mac; 
//char ssid[] = "HackingMadness";     // the name of your network
//int status = WL_IDLE_STATUS;     // the Wifi radio's status
//byte mac[6]; 

int getHumedad(){
  float h = dht.readHumidity();
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");
  return int(h);
}

int getTemperatura(){
  float t = dht.readTemperature();
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
   if (command == "temp") {
      Serial.println("Temperatura consultada por API REST");
      client.println(t);  
   }
   if (command == "hum"){
      Serial.println("Humedad consultada por API REST");
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

bool isError(int h,int t){
  if (isnan(h) || isnan(t)) {
    Serial.println("Error en la lectura del sensor DHT!");
    lcd.setCursor(0,0);
    lcd.print("Error Lectura Sensor");
    lcd.setCursor(0,1);
    lcd.print("Sensor DHT11");
    return true;
  }
  return false;
}
String getDate(){
  date.begin("date");
  date.addParameter("+%D +%T");
  date.run();
  return date.readString();
}
/*String getSMAC(){
  Process getmac; 
  getmac.begin("ifconfig wlan0 ");
  getmac.run();
  return getmac.readString();
}*/
/*void getMAC(){
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
  } else{
    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[5],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.println(mac[0],HEX);
  }
}*/
void escribirInformacion(String id,int h, int t){
  
  String date = getDate();
  date.trim();
  File dataFile = FileSystem.open("/root/Proyecto/Test/sensor.log", FILE_APPEND);
  if (dataFile) {
      String outTemp = date+","+id+",Temperatura,"+String(t);
      String outHum = date+","+id+",Humedad,"+String(h);
      dataFile.println(outTemp);
      dataFile.println(outHum);
      dataFile.close();
      Serial.println("Informacion escrita");
   }
   else {
      Serial.println("Error escribiendo datos en el log");
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
  FileSystem.begin();
  server.listenOnLocalhost();
  server.begin();
  date_config.begin("ntpd -u hora.rediris.es");
  date_config.run();

  //status = WiFi.begin(ssid);
}

void loop() {
  delay(3000);
  int h = getHumedad();
  int t = getTemperatura();
  Serial.println("Lectura de temperatura y humedad realizada");
  if(isError(h,t)){
    return;
  }
  Serial.println("Lectura de temperatura y humedad sin errores");
  escribirLCD(h,t,"DHT11-01");
  iniciarAPIREST(h,t);
  escribirInformacion("DHT11-01",h,t);
  //Serial.println(getSMAC().substring(20));
  
  
}

