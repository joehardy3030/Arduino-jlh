#include <Bridge.h>
#include <Console.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Time.h>

int heaterOn = 0;
int heaterOff = 1;
int relayPin = 13;
int m = millis();
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
  Serial.println("Setup complete. Waiting for sensor input...\n");
  Console.println("Setup complete. Waiting for sensor input...\n");
  //joe$ ssh root@10.0.0.42 'telnet localhost 6571'
}

void loop() {
  YunClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }

  delay(3000);//Don't try to access too frequently... in theory
 // Console.println("Loop\n");
}

void process(YunClient client) {
  String command = client.readStringUntil('/');

  if (command == "digital") {
    digitalCommand(client);
    client.println(F("digi"));
    }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
  if (command == "on") {
    onCommand(client);
  }

}

void onCommand(YunClient client) {
  int pin, value;
     
  client.println(F("on"));

 // pin = client.parseInt();
  pin = relayPin; 
  //eg http://10.0.0.42/arduino/digital/13/0
  
//  if (client.read() == '/') {
//    value = client.parseInt();
    value = 0;
    writePin(client,pin,value);
 // } 
 //else {
 //   value = digitalRead(pin);
 // }
}

void digitalCommand(YunClient client) {
  int pin, value;
  pin = client.parseInt();

  //eg http://10.0.0.42/arduino/digital/13/0
  
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


