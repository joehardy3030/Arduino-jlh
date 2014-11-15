  /*
  Toggle Relay over WiFi
  
 */
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

int LEDPIN = 13; // your LED PIN
YunServer server;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
   Bridge.begin();  

  // Show a fancy flash pattern once connected
  digitalWrite(13,LOW); 
  delay(1500);
  digitalWrite(13,HIGH); 
  delay(1500);
  digitalWrite(13,LOW); 
  delay(1500);
  digitalWrite(13,HIGH); 
  delay(1500);
  digitalWrite(13,LOW); 
  delay(1500);
  
  // Disable for some connections:
  // Start listening for connections  
  
  // server.listenOnLocalhost();
  server.begin();
 
}

void loop() {
  // Listen for clients
  YunClient client = server.accept();
  // Client exists?
  if (client) {
    // Lets process the request!
    process(client);
    client.stop();
  }
  delay(50);
}

void process(YunClient client) {
  // Collect user commands
  String command = client.readStringUntil('\\'); // load whole string
  
  // Enable HTML
  client.println("Status: 200");
  client.println("Content-type: text/html");
  client.println();
  
  // Show UI
  client.println("<B><Center>");
  client.println("<a href='http://yun1.local/arduino/on\\'>Turn ON 13</a><br>");
  client.println("<a href='http://yun1.local/arduino/off\\'>Turn OFF 13</a><br>");
  client.print("Command: ");
  client.println(command);
  client.println("</B></Center>");
  
  // Check what the user entered ...
  
  // Turn on
  if (command == "on") {
    digitalWrite(13,HIGH);
    client.println("<a href='http://yun1.local/arduino/on\\'>Turn on 13</a><br>");
    
}
  
  // Turn off
  if (command == "off") {
    digitalWrite(13,LOW);
  }
}
