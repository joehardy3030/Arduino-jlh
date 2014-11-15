//refactor this to accept analog temp input
//and write that out to a google doc

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <dht.h>  
#include <Time.h>

#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

int heaterOn = 0;
int heaterOff = 1;
int relayPin = 2;
dht DHT;
YunServer server;

void setup() {
  YunClient client;
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  server.listenOnLocalhost();
  server.begin();
  delay(3000);
  setTime(22,35,0,18,9,2014);
  delay(3000);
  intializeRelay(client, relayPin); //set the relay pin to OUTPUT and turn it off
}

void loop() {
  YunClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }

  delay(3000);//Don't try to access too frequently... in theory

}

void process(YunClient client) {
  String command = client.readStringUntil('/');

  if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
  if (command == "heater") {
    heaterCommand(client);
  }
  if (command == "thermo") {
    thermoCommand(client);
  }

}

void digitalCommand(YunClient client) {
  int pin, value;
  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    writePin(client,pin,value);
  } 
  else {
    value = digitalRead(pin);
  }
}

void analogCommand(YunClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    value = analogRead(pin);

    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;
  pin = client.parseInt();

if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    intializeRelay(client, pin);
    return;  
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}

void thermoCommand(YunClient client) {
  int pin, value;
 
  DHT.read11(dht_dpin);
  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
      value = analogRead(pin);
      time_t t = now();
      client.print(hour(t));
      client.print(F(":"));
      client.print(minute(t));
      client.print(F(", "));
      client.print((DHT.temperature*9/5)+32);
      client.print(F(", "));
      client.print(DHT.humidity);
      client.println(F("%"));
      String key = "A";
      key += pin;
      Bridge.put(key, String(value));
  }
}

void writePin(YunClient client, int pin, int value) {
    time_t t = now();
    digitalWrite(pin, value);

    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to "));
    client.println(value);

    client.print(F("At "));
    client.print(hour(t));
    client.print(F(":"));
    client.println(minute(t));
   
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));

 //   time_t t = now();
}

//initialize the relay pin to output and turn it off
void intializeRelay(YunClient client, int pin) { 
    pinMode(pin, OUTPUT);
    delay(1000);
    digitalWrite(pin, 1); //turn it off
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT and turned OFF"));
   // return;
}

void heaterCommand(YunClient client) {
  time_t t = now();
  int pin, value;
  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    writePin(client,pin,value);
    //client.println(second()-second(t));
  } 
  else {
    value = digitalRead(pin);
  }
    
 //   time_t t = now();
}

