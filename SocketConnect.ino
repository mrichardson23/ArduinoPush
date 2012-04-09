/*
  Telnet client
 
 This sketch connects to a a telnet server (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.  You'll need a telnet server 
 to test this with.
 Processing's ChatServer example (part of the network library) works well, 
 running on port 10002. It can be found as part of the examples
 in the Processing application, available at 
 http://processing.org/
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 14 Sep 2010
 by Tom Igoe
 
 */

#define LED_PIN 7

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,1,150);
int port = 8082;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use  port 10002):
EthernetClient client;

void setup() {
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // start the serial library:
  Serial.begin(9600);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  // if there are incoming bytes available 
  // evaluate if they're H or L chars
  if (client.available()) {
    char c = client.read();
    if (c == 'H') {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Received HIGH");
    }
    if (c == 'L') {
      digitalWrite(LED_PIN, LOW); 
      Serial.println("Received LOW");
    }
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar); 
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("disconnected.");
    // may not need?:
    //client.stop(); 
    // Keep trying until we get through:
    while (!client.connected()) {
      Serial.println("reconnecting...");
      if (client.connect(server, port)) {
        Serial.println("reconnected.");
      } 
      else {
        // if you didn't get a connection to the server:
        Serial.println("connection failed. Waiting 10 secs.");
        delay(10000);
      } 
    }
  }
}





