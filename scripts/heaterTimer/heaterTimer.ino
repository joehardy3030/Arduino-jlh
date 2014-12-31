// This program is designed to control a relay to turn a heater on and off
// joe$ ssh root@10.0.0.42 'telnet localhost 6571'

#include <Temboo.h>
#include <Bridge.h>
#include <Console.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Process.h>

#include "TembooAccount.h"
#include "DHT.h"

// DHT11 sensor pins
#define DHTPIN A0 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);
                                         
// Your Google Docs data
const String GOOGLE_USERNAME = "jlhardyphd@gmail.com";
const String GOOGLE_PASSWORD = "azjfsgddtpzuawcv";
const String SPREADSHEET_TITLE = "heaterTimer";

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

  delay(3000);
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
    runAppendRow();
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
  if (command == "append") {
    appendCommand(client);
  }
  if (command == "off") {
    offCommand(client);
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
  temperature = dht.readTemperature();
  String timeString = date.readString(); 
  client.print(timeString);
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
  client.print("Duration: ");
  client.println(duration);
  m = millis();
  writePin(client, relayPin, heaterOn);
  client.stop();
//  while(client.connected());
  //delay(3000);
//  runAppendRow();
}

void appendCommand(YunClient client) {
  client.println(F("append"));
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  String timeString = date.readString(); 
  client.print(timeString);
  client.print("Temp: ");
  client.print(temperature);
  client.print("; Humidity: ");
  client.println(humidity);
  client.println(millis() - m);
  client.stop();
  while(client.connected());
  runAppendRow();
}

void offCommand(YunClient client) {
  client.println(F("off"));
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
    if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  String timeString = date.readString(); 
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
  pinMode(pin, OUTPUT); //configure the pin for output
  digitalWrite(pin, heaterOff); //turn it off
}

// Function to add data to Google Docs
void runAppendRow() {
  TembooChoreo AppendRowChoreo; 

  // Invoke the Temboo client
  AppendRowChoreo.begin();

  // Set Temboo account credentials
  AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
  
  // Identify the Choreo to run
  AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

  // your Google username (usually your email address)
  AppendRowChoreo.addInput("Username", GOOGLE_USERNAME);

  // your Google account password
  AppendRowChoreo.addInput("Password", GOOGLE_PASSWORD);
  
  // the title of the spreadsheet you want to append to
  AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);
  
  // Restart the date process:
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  
  // Get date
  String timeString = date.readString(); 
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  String heater = "off";
  if (digitalRead(relayPin) == heaterOn) {
    heater = "on";
  }
  
  // Format data
  String data = "";
  
  data = data + timeString + "," + temperature + "," + humidity + "," + heater;
  
  // Set Choreo inputs
  AppendRowChoreo.addInput("RowData", data);

  // Run the Choreo
  unsigned int returnCode = AppendRowChoreo.run();

  // A return code of zero means everything worked
  if (returnCode == 0) {
      Serial.println("Completed execution of the /Library/Google/Spreadsheets/AppendRow Choreo.\n");
      Console.println("Completed execution of the /Library/Google/Spreadsheets/AppendRow Choreo.\n");
    }
  else {
    // A non-zero return code means there was an error
    // Read and print the error message
    while (AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      Serial.print(c); 
      Console.print(c); 
    }
    Serial.println(); 
    Console.println();   
  }
  
  AppendRowChoreo.close(); 
}

