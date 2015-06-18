// This program is designed to control a relay to turn a heater on and off
// joe$ ssh root@10.0.0.20 'telnet localhost 6571'

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Process.h>
#include <IRremote.h>
#include "DHT.h"

//The panasonic protocol is 48-bits long, all the commands has the same address (or pre data) and then followed by a 32 bit code
#define PanasonicAddress      0x4004     // Panasonic address (Pre data) 
#define PanasonicPower        0x100BCBD  // Panasonic Power button
#define PanasonicYellow       0x1004E4F  // Yellow
#define PanasonicBlue         0x100CECF  // Blue button
#define PVMinus               0x1008485  // Volume down
#define PVPlus                0x1000405  // Volume up

IRsend irsend;

// DHT11 sensor pins
#define DHTPIN A0 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);
                                         
const int buttonDown = 0;
const int buttonUp = 1;
const int relayOn = 0;
const int relayOff = 1;
const int outletRelayPin = 12;
const int doorRelayPin = 8;

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
  server.listenOnLocalhost();
  server.begin();
  delay(1000);
  intializeRelay(client, doorRelayPin); //set the relay pin to OUTPUT and turn it off
  intializeRelay(client, outletRelayPin); //set the relay pin to OUTPUT and turn it off
}

void loop() {
  YunClient client = server.accept();
  if (client) {
    process(client);
    client.stop();
  }
  delay(100);//Don't try to access too frequently... in theory
  if (digitalRead(outletRelayPin) == relayOn && (millis() - m) > (duration)) {    
    outletOffCommand(client);
  }
}

void process(YunClient client) {
  String command = client.readStringUntil('\r');
  
  if (command == "press") {
    pressCommand(client);
  }
  if (command == "outletOn") {
    duration = one_hour * 4;
    outletOnCommand(client);
  }
  if (command == "outletOff") {
    outletOffCommand(client);
  }
  if (command == "TVon") {
    tvOnCommand(client);
  }
  if (command == "TVvolUp") {
    tvVolumeUp(client);
  }
  if (command == "TVvolDown") {
    tvVolumeDown(client);
  }
}

void writePin(YunClient client, int pin, int value) {
  digitalWrite(pin, value);
}

//initialize the relay pin to output and turn it off
void intializeRelay(YunClient client, int pin) {
  pinMode(pin, OUTPUT); //configure the pin for output
  digitalWrite(pin, buttonUp); //turn it off
}

void datePrint(YunClient client) {
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
  }
  String timeString = date.readString(); 
  client.print(timeString);
}  

void tempPrint(YunClient client) {
  humidity = dht.readHumidity();
  temperature = (dht.readTemperature() *(9.0/5.0)) + 32.0;
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
}  

void tvOnCommand(YunClient client) {
  client.println(F("TV on/off"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  irsend.sendPanasonic(PanasonicAddress,PanasonicPower); // This should turn your TV on and off
  client.stop();
 }

void tvVolumeUp(YunClient client) {
  client.println(F("TV vol up"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  irsend.sendPanasonic(PanasonicAddress,PVPlus); // This should turn your TV on and off
  client.stop();
 }

void tvVolumeDown(YunClient client) {
  client.println(F("TV vol down"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  irsend.sendPanasonic(PanasonicAddress,PVMinus); // This should turn TV volume down
  client.stop();
 }
 
void pressCommand(YunClient client) {
  writePin(client, doorRelayPin, buttonDown);
  delay(1000);
  writePin(client, doorRelayPin, buttonUp);  
  client.println(F("Garage remote pressed"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  client.stop();
}

void outletOnCommand(YunClient client) {
  client.println(F("Stereo power on"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  client.print("Duration: ");
  client.println(duration/(60000));
  m = millis();
  writePin(client, outletRelayPin, relayOn);
  client.stop();
}

void outletOffCommand(YunClient client) {
  client.println(F("Stereo power off"));
  datePrint(client); // Print date
  tempPrint(client); // Temp date
  client.print("Duration: ");
  client.print((millis() - m)/(60000));
  client.println(" min");
  writePin(client, outletRelayPin, relayOff);
  client.stop();
}

