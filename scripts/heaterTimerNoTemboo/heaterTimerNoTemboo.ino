// This program is designed to control a relay to turn a heater on and off
// joe$ ssh root@10.0.0.12 'telnet localhost 6571'

#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>
#include "DHT.h"
           
// DHT11 sensor pins
#define DHTPIN A0 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);
                          
const int heaterOn = 0;
const int heaterOff = 1;
const int relayPin = 13;

float humidity;
float temperature;
Process date;

unsigned long m = millis();
unsigned long duration = 0;
unsigned long one_hour = 3600000;
YunServer server;

void setup() {
  dht.begin();
  YunClient client;
  Serial.begin(115200);
  Bridge.begin();
  Console.begin();
  server.listenOnLocalhost();
  server.begin();
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
  }

  delay(1000);
  intializeRelay(client, relayPin); //set the relay pin to OUTPUT and turn it off
}

void loop() {
  YunClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }
  
  delay(100);//Don't try to access too frequently... in theory
  
  if (digitalRead(relayPin) == heaterOn && (millis() - m) > (duration)) {
    offCommand(client);
  }
}

void process(YunClient client) {
  String command = client.readStringUntil('\r');

  if (command == "on") {
    duration = one_hour;
    onCommand(client);
  }
  if (command == "on2") {
    duration = one_hour*2;
    onCommand(client);
  }
  if (command == "on3") {
    duration = one_hour*3;
    onCommand(client);
  }
  if (command == "off") {
    offCommand(client);
  }
  if (command == "temp") {
    tempCommand(client);
  }

}

void onCommand(YunClient client) {
    // Restart the date process:
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  // Get date
  client.println(F("on"));
  humidity = dht.readHumidity();
  temperature = (dht.readTemperature() *(9.0/5.0)) + 32.0;
  String timeString = date.readString(); 
  client.print(timeString);
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
  client.print("Duration: ");
  client.println(duration/60000);
  m = millis();
  writePin(client, relayPin, heaterOn);
  client.stop();
}


void offCommand(YunClient client) {
    if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  // get humidity and temp
  humidity = dht.readHumidity();
  temperature = (dht.readTemperature() *(9.0/5.0)) + 32.0;
  String timeString = date.readString(); 
  
  client.println(F("off"));
  client.print(timeString);
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
  client.print("Duration: ");
  client.print((millis() - m)/(60000));
  client.println(" min");
  writePin(client, relayPin, heaterOff);
  client.stop();
}

void tempCommand(YunClient client) {
  int value;
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  // get humidity and temp
  humidity = dht.readHumidity();
  temperature = (dht.readTemperature() *(9.0/5.0)) + 32.0;
  String timeString = date.readString(); 
  client.print(timeString);
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
  client.print("Duration: ");
  client.print((millis() - m)/(60000));
  client.println(" min");
  value = readPin(client, relayPin);
  if (value == 0) {
    client.println("Heater On");
  }
  if (value == 1) {
    client.println("Heater Off");
  }
  client.stop();
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

int readPin(YunClient client, int pin) {
  int value;
  value = digitalRead(pin);

  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
  return(value);
}


//initialize the relay pin to output and turn it off
void intializeRelay(YunClient client, int pin) {
  pinMode(pin, OUTPUT); //configure the pin for output
  digitalWrite(pin, heaterOff); //turn it off
}

