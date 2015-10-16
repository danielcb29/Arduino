/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

char var[5] = "Cali";
//char var[]="a";
int morse[][5] = {{0,1},{1,0,0,0},{1,0,1,0},{1,0,0},{0},{0,0,1,0},{1,1,0},{0,0,0,0},{0,0},{0,1,1,1},{1,0,1},{0,1,0,0},{1,1},{1,0},{1,1,1},{0,1,1,0},{1,1,0,1},{0,1,0},{0,0,0},{1},{0,0,1},{0,0,0,1},{0,1,1},{1,0,0,1},{1,0,1,1},{1,1,0,0}};
//int morse[][5]={{0,1},{1,0,0,0}};
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
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
// the loop function runs over and over again forever
void loop() {
  
  for(int i=0;i<sizeof(var)-1;i++){
      int letter = var[i];
      if (letter < 97){
        letter-=65;
      }else{
        letter-=97;
      }
      writeMorse(morse[letter]);
      delay(3000);
  }
  //delay(1000);
  
  
  
}
