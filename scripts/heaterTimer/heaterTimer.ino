// This program is designed to control a relay to turn a heater on and off
// joe$ ssh root@10.0.0.42 'telnet localhost 6571'

#include <Bridge.h>
#include <Console.h>
#include <YunServer.h>
#include <YunClient.h>

int heaterOn = 0;
int heaterOff = 1;
int relayPin = 13;
int m = millis();
int start_millis = m;
YunServer server;

void setup() {
  YunClient client;
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  server.listenOnLocalhost();
  server.begin();
  delay(3000);
  intializeRelay(client, relayPin); //set the relay pin to OUTPUT and turn it off
}

void loop() {
  YunClient client = server.accept();
  int running_millis;
  
  if (client) {
    process(client);
    client.stop();
  }
  
  if (digitalRead(relayPin) == heaterOn && millis()-start_millis > (3600000)) {
    offCommand(client);
  }

  Console.println(millis()-start_millis);
  Console.println(3600000);
  delay(3000);//Don't try to access too frequently... in theory
}

void process(YunClient client) {
  String command = client.readStringUntil('\r');

  if (command == "on") {
    onCommand(client);
  }
  if (command == "off") {
    offCommand(client);
  }
}

void onCommand(YunClient client) {
  client.println(F("on"));
  start_millis = millis();
  writePin(client,relayPin,heaterOn);

}

void offCommand(YunClient client) {
  client.println(F("off"));
  client.println(millis()-start_millis);  
  writePin(client,relayPin,heaterOff);
}

void writePin(YunClient client, int pin, int value) {
  digitalWrite(pin, value);

  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);
    
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

//initialize the relay pin to output and turn it off
void intializeRelay(YunClient client, int pin) { 
  pinMode(pin, OUTPUT);
  delay(1000);
  digitalWrite(pin, 1); //turn it off
}


