
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

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11

// include the library code:

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);
YunServer server;
Process date;
Process sendData;
int time_counter = 0;

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

void escribirInformacion(String id,int h, int t){
  
  String date = getDate();
  date.trim();
  File dataFile = FileSystem.open("/root/Proyecto/Logs/sensor.log", FILE_APPEND);
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
void envioDatosServer(){
  sendData.begin("python");
  sendData.addParameter("scripts/send_data.py");
  sendData.run();
  Serial.println(sendData.readString());
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
}

void loop() {
  delay(3000);
  time_counter++;
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
  String val_cont = "Valor contador de segundos:" + String(time_counter*3)+"s";
  Serial.println(val_cont);
  if(time_counter == 2){ //3 minutos, ejecucion del programa cada 3 segundos (60 iteraciones)
    time_counter = 0;
    envioDatosServer();
    Serial.println("Informacion enviada al servidor");
  }
  
}

