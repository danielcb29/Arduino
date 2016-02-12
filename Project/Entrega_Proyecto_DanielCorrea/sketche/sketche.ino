
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

#define DHTPIN 2  //Pin al cual va a ser conectado el sensor
#define DHTTYPE DHT11   // Tipo de sensor : DHT11

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Asignacion de pines de pantalla lcd
DHT dht(DHTPIN, DHTTYPE); //Inicializacion del sensor
YunServer server; //Servidor para API REST
Process date; //Proceso para consultar la fecha
Process sendData; //Proceso para ejecutar envio de datos
int time_counter = 0; //Contador de tiempo para ejecucion de script de envio de datos

/**
 * Metodo que permite obtener el valor de humedad del sensor
 */
int getHumedad(){
  float h = dht.readHumidity();
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");
  return int(h);
}

/**
 * Metodo que permite obtener el valor de temperatura del sensor
 */
int getTemperatura(){
  float t = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C ");
  return int(t);
}
/**
 * Metodo que permite escribir los valores de temperatura, humedad e id del sensor en la pantalla lcd
 */
void escribirLCD(int h, int t, String idSensor){
  String primeraLinea = "Hum:" + String(int(h)) + "%," + "Tmp:" + String(int(t)) + "*C";
  String segundaLinea = "Sensor:"+ idSensor;
  lcd.setCursor(0,0); //Primera fila
  lcd.print(primeraLinea);
  lcd.setCursor(0, 1); //Segunda fila
  lcd.print(segundaLinea);
}
/**
 * Metodo encargado de procesar una petcion realizada por la API REST
 */
void process_REST(YunClient client,int h, int t) {
   String command = client.readStringUntil('/');
   command.trim();
   if (command == "temp") { //Caso de peticion de temperatura
      Serial.println("Temperatura consultada por API REST");
      client.println(t);  
   }
   if (command == "hum"){ //Caso de peticion de humedad
      Serial.println("Humedad consultada por API REST");
      client.println(h);
   }
 
}
/**
 * Metodo encargado de verificar peticiones realizadas por la API REST
 */
void iniciarAPIREST(int h, int t){
  YunClient client = server.accept(); 
  if (client) { //Si existe un cliente realizando una peticion
      Serial.println("API REST EN PROCESO");
      process_REST(client,h,t);
      client.stop();
   }
}
/**
 * Metodo encargado de verificar errores en la lectura del sensor
 */
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
/**
 * Metodo encargado de consultar la fecha en el SO del Arduino
 */
String getDate(){
  date.begin("date");
  date.addParameter("+%D +%T");
  date.run();
  return date.readString();
}
/**
 * Metodo que permite escribir en el archivo log
 */
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
/**
 * Metodo que ejecuta el script encargado de enviar los datos al servidor web
 */
void envioDatosServer(){
  sendData.begin("python");
  sendData.addParameter("/root/Proyecto/Produccion/scripts/send_data.py");
  sendData.run();
  Serial.println(sendData.readString());
}
/**
 * Metodo que inicializa la configuracion del sketch en el Arduino
 */
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
/**
 * Metodo principal
 */
void loop() {
  delay(3000);
  time_counter++; //Nueva iteracion
  int h = getHumedad();
  int t = getTemperatura();
  Serial.println("Lectura de temperatura y humedad realizada");
  if(isError(h,t)){ //Verificacion de errores
    return;
  }
  Serial.println("Lectura de temperatura y humedad sin errores");
  escribirLCD(h,t,"DHT11-01"); //Escritura de datos en la pantalla LCD
  iniciarAPIREST(h,t); //Verificacion de peticiones REST
  escribirInformacion("DHT11-01",h,t); //Escritura en log
  String val_cont = "Valor contador de segundos:" + String(time_counter*3)+"s"; //Cuantos segundos de ejecucion han transcurrido
  Serial.println(val_cont); 
  if(time_counter == 60){ //3 minutos, ejecucion del programa cada 3 segundos (60 iteraciones)
    time_counter = 0; //Contador vuelve a comenzar
    envioDatosServer(); //Ejecucion de script de envio de datos al servidor
    Serial.println("Informacion enviada al servidor");
  }
  
}

