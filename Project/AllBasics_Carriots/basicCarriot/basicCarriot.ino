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
#include <SendCarriotsYun.h>  // You must include the library here in order to use it in the sketch

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11

const String APIKEY="cf4e352678bdb962b2d083303a9226b43bb142d4fbc1263e778d01596153a011";     // Replace with your Carriots apikey
const String DEVICE="testarduinodc@danielcb.danielcb";    // Replace with the id_developer of your device

const int numElements=4;    // Specify the number of rows in the array, in this case 3

SendCarriotsYun sender;     // Instatiate an instance of the SendCarriotsYun library
int time_counter = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);
YunServer server;
Process date_config;
Process date;

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



void setup() {
  // start serial port:
  Bridge.begin();
  Serial.begin(9600);
  date_config.begin("ntpd -u hora.rediris.es");
  date_config.run();  

}

void loop() {
  Serial.println("Ola");
  //String date = getDate();
  //date.trim();
  String info[4][2] = {{"date","9-9-11"},{"id","dht11-01"},{"temp",String(23)},{"hum",String(45)}};
  //Serial.println(date);
  Serial.println("Información enviada al servidor, respuesta:");
  Serial.println(sender.send(info, 4, APIKEY, DEVICE));
  //String array[numElements][2] = {{"date", "09/12/2015 09:12"}, {"id", "289172"}, {"temp", "23"}, {"hum", "45"}};    // Define the values of the array to be sent
  //Serial.println(sender.send(array, numElements, APIKEY, DEVICE));                  // Using the instance of the library, call the method send
  delay(3000);                                                           // Send this stream every 10 seconds
}
