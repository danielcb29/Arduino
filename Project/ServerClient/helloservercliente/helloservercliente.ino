#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

void setup() {
   Serial.begin(9600);
   Bridge.begin();
   server.listenOnLocalhost();
   server.begin();
}
void loop() {
   YunClient client = server.accept();
   if (client) {
    Serial.println("loop");
     process(client);
     client.stop();
   }
   delay(50);
}
void process(YunClient client) {
 String command = client.readStringUntil('/');
 command.trim();
 Serial.println(command);
 if (command == "hello") {
    client.println("Hello World!");
 }else{
  client.println("Incorrecto"); 
 }
 
}
