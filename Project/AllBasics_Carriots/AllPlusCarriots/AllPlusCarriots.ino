
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
#include <SendCarriotsYun.h>

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11

////////////////////////////
// Initial State Streamer //
////////////////////////////
// URL to IS Bucket API
String ISBucketURL = "https://groker.initialstate.com/api/buckets";
// URL to IS Event API
String ISEventURL = "https://groker.initialstate.com/api/events";
// Access key (the one you find in your account settings):
String accessKey = "g7ewgXNOeJpaizQaCLJ7kwtswBNKmUl9";
// Bucket key (hidden reference to your bucket that allows appending):
String bucketKey = "arduinodc_stream";
// Bucket name (name your data will be associated with in Initial State):
String bucketName = "Arduino DC Stream";
// How many signals are in your stream? You can have as few or as many as you want
const int NUM_SIGNALS = 4;
// What are the names of your signals (i.e. "Temperature", "Humidity", etc.)
String signalName[NUM_SIGNALS] = {"Date", "Id", "Temperature","Humidity"};
// This array is to store our signal data later
String signalData[NUM_SIGNALS];


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
  
  //String date = getDate();
  String date = "09/09/2015 15:06";
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

void envioDatosServer(int h, int t,String id){
  //String date = getDate();
  String date = "09/09/2015 15:06";
  date.trim();
  signalData[0] = date;
  signalData[1] = id;
  signalData[2] = String(t);
  signalData[3] = String(h);
  Serial.println(signalData[0]+" : "+signalData[1]+" : "+signalData[2]+" : "+signalData[3]);
  Serial.println("Posting Data!");
  postData();

}

void postBucket()
{
  // Initialize the process
  Process isbucket;

  isbucket.begin("curl");
  isbucket.addParameter("-k");
  isbucket.addParameter("-v");
  isbucket.addParameter("-X");
  isbucket.addParameter("POST");
  isbucket.addParameter("-H");
  isbucket.addParameter("Content-Type:application/json");
  isbucket.addParameter("-H");
  isbucket.addParameter("Accept-Version:0.0.1");

  // IS Access Key Header
  isbucket.addParameter("-H");
  isbucket.addParameter("X-IS-AccessKey:" + accessKey);

  // IS Bucket Key Header
  isbucket.addParameter("-d");
  isbucket.addParameter("{\"bucketKey\": \"" + bucketKey + "\", \"bucketName\": \"" + bucketName + "\"}");
  
  isbucket.addParameter("https://groker.initialstate.com/api/buckets");
  
  // Run the process
  isbucket.run();

  Serial.flush();
}
void postData()
{
  // Initialize the process
  Process isstreamer;

  isstreamer.begin("curl");
  isstreamer.addParameter("-k");
  isstreamer.addParameter("-v");
  isstreamer.addParameter("-X");
  isstreamer.addParameter("POST");
  isstreamer.addParameter("-H"); 
  isstreamer.addParameter("Content-Type:application/json");
  isstreamer.addParameter("-H");
  isstreamer.addParameter("Accept-Version:0.0.1");

  // IS Access Key Header
  isstreamer.addParameter("-H");
  isstreamer.addParameter("X-IS-AccessKey:" + accessKey);

  // IS Bucket Key Header
  // Note that bucketName is not needed here
  isstreamer.addParameter("-H");
  isstreamer.addParameter("X-IS-BucketKey:" + bucketKey);

  isstreamer.addParameter("-d");

  // Initialize a string to hold our signal data
  String jsonData;

  jsonData = "[";

  for (int i=0; i<NUM_SIGNALS; i++)
  {
    jsonData += "{\"key\": \"" + signalName[i] + "\", \"value\": \"" + signalData[i] + "\"}";

    if (i != NUM_SIGNALS - 1)
    {
      jsonData += ",";
    }
  }

  jsonData += "]";

  isstreamer.addParameter(jsonData);

  isstreamer.addParameter("https://groker.initialstate.com/api/events");

  // Print posted data for debug
  Serial.print("Sending data: ");
  Serial.println(jsonData);

  // Run the process
  isstreamer.run();

  Serial.flush();
}

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Proyecto Arduino");
  lcd.setCursor(0, 1);
  lcd.print("Daniel Correa");
  dht.begin();
  FileSystem.begin();
  server.listenOnLocalhost();
  server.begin();
  //date_config.begin("ntpd -u hora.rediris.es");
  //date_config.run();
  postBucket();
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
  if(time_counter == 60){ //3 minutos, ejecucion del programa cada 3 segundos
    time_counter = 0;
    envioDatosServer(h,t,"DHT11-01");
  }
  
}



