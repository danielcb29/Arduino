/*
 * Daniel Correa: Ejercicio Basico
 * Lectura de humedad y tempratura, al momento de superar los 25ª se informa por morse (mediante el pin 13) una alerta
*/


#include "DHT.h" //Libreria
#define DHTPIN 2 //Pin donde esta conectado el sensor
#define DHTTYPE DHT11 //Nuestro sensor es el DHT11

int morse[][5] = {{0,1},{1,0,0,0},{1,0,1,0},{1,0,0},{0},{0,0,1,0},{1,1,0},{0,0,0,0},{0,0},{0,1,1,1},{1,0,1},{0,1,0,0},{1,1},{1,0},{1,1,1},{0,1,1,0},{1,1,0,1},{0,1,0},{0,0,0},{1},{0,0,1},{0,0,0,1},{0,1,1},{1,0,0,1},{1,0,1,1},{1,1,0,0}}; //Diccionario morse
DHT dht(DHTPIN, DHTTYPE); //Inicializacion del sensor 
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600); //Se inicializa el puerto serial
   pinMode(13, OUTPUT); //Pin donde enviaremos el mensaje de alerta
   dht.begin(); //Se inicia el sensor
   while(!Serial); //Esperamos a que el puerto serial este activo
}
void writeDot(){
  //El punto es un encendido largo de 2000 milisegundos
  //El punto es 0
  digitalWrite(13, HIGH);   // Se prende el led 
  delay(2000);              // Se mantiene prendido por 2000 milisenguos
  digitalWrite(13, LOW);    // Se apaga el led
  delay(500);              // Se mantiene apagado durante 500 milisengundos
}
void writeLine(){
  //La raya es un retardo corto de 750 milisegundos
  //La raya es 1
  digitalWrite(13, HIGH);   // Se prende el led 
  delay(750);              // Se mantiene prendido por 750 milisenguos
  digitalWrite(13, LOW);    // Se apaga el led
  delay(500);              // Se mantiene apagado durante 500 milisengundos
}
void writeMorse(int letter[]){
  //Metodo que permite escribir el codigo morse a partir de una letra del diccionario
  //Se le pasa una letra (arreglo de 0s y 1s) y la imprime con los metodos writeDot() y writeLine()
  for(int i=0; i<sizeof(letter);i++){
    if(letter[i]){ //Si la letra es 1 imprime linea
      writeLine(); 
    }else{ //Si no es porque es 0 e imprime punto
      writeDot();
    }
  }
}
void alert(){
  //Metodo que permite alertar en codigo morse que la temperatura esta muy alta
  String mensaje = "Alerta temperatura muy alta"; //Mensaje de alerta
  for(int i=0;i<sizeof(mensaje)-1;i++){ //Se recorre el mensaje
        int letter = mensaje[i]; //Se convierte a ascii
        if (letter < 97){ //Se revisa si es mayuscula o minuscula
          letter-=65; //Se restarua el indice para saber la posicion de la letra , el diccionario va de 0 a 26, se debe convertir ascii a posicion del diccionario
        }else{
          letter-=97; //Si es mayuscula el indice es diferente , se restaura igual para obtener la letra en el diccionario
        }
        writeMorse(morse[letter]); //Escribimos la letra en morse
        delay(3000); //Retardo de 3000 milisengundos entre una letra y otra
    }
}
void loop() {
  // put your main code here, to run repeatedly:
   delay(2000); //Leer la temperatura y humedad puede tomar 250 milisengudos, sin embargo, al sensor le toma 2 segundos
  float h = dht.readHumidity(); //Leemos la humedad
  float t = dht.readTemperature(); //Leemos la tempratura en celcius
  
  // Verificamos si algo fallo durante la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error en la lectura del sensor!"); //Mensaje de fallo
    return; //Salida
  }
  //Imprimimos la humedad y la temperatura por el puerto serial
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");
  //Si la temperatura es mayor a 25 grados celcius enviamos alerta 
  if(t >25.00){
    Serial.println("Alerta temperatura sobrepasada!!"); //Enviamos alerta por puerto serial
    alert(); //Enviamos alerta por morse
  }
  
}
