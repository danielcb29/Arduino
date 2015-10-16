
#include <FileIO.h>
int morse[][5] = {{0,1},{1,0,0,0},{1,0,1,0},{1,0,0},{0},{0,0,1,0},{1,1,0},{0,0,0,0},{0,0},{0,1,1,1},{1,0,1},{0,1,0,0},{1,1},{1,0},{1,1,1},{0,1,1,0},{1,1,0,1},{0,1,0},{0,0,0},{1},{0,0,1},{0,0,0,1},{0,1,1},{1,0,0,1},{1,0,1,1},{1,1,0,0}};
void setup() {
  // put your setup code here, to run once:
   Bridge.begin();
   Serial.begin(9600);
   FileSystem.begin();
   pinMode(13, OUTPUT);
   while(!Serial);
}
void writeDot(){
  //El punto es un encendido largo de 2000 milisegundos
  //El punto es 0
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}
void writeLine(){
  //La raya es un retardo corto de 750 milisegundos
  //La raya es 1
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(750);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}
void writeMorse(int letter[]){
  for(int i=0; i<sizeof(letter);i++){
    if(letter[i]){
      writeDot();
    }else{
      writeLine();
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
   File dataFile = FileSystem.open("/root/morse/mensaje.txt", FILE_READ);
   if (dataFile) {
      String mensaje = dataFile.readString();
      dataFile.close();
      Serial.println("Leido, a puerto serie:");
      Serial.println(mensaje);
      Serial.println("Leido, a morse serie:");
      for(int i=0;i<sizeof(mensaje)-1;i++){
          int letter = mensaje[i];
          if (letter < 97){
            letter-=65;
          }else{
            letter-=97;
          }
          writeMorse(morse[letter]);
          delay(3000);
      }
      
   }
   else {
      Serial.println("error opening mesaje.txt");
   }
   //delay(15000); 
}
