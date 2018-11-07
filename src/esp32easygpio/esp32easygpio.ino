/**
 * @imports
 */
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <U8x8lib.h>
#include "FS.h"
#include "SPIFFS.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include "esp_system.h"
#include "types.h"


/**
 * @locals
 */
WiFiServer server(80);
APConfiguration apconfig;
String header;


String ReadFile(String path);
bool LoadConfiguration(APConfiguration& apconfig);

/**
 * @setup
 */
void setup() {
  Serial.begin(115200);

  // SPIFFS
  Serial.print("Mounting FS ");
  if (!SPIFFS.begin()) {
    Serial.println("[FAILED]");
    return;
  }
  Serial.println("[OK]");

  // CONFIG
  Serial.print("Loading Configuration ");
    if(!LoadConfiguration(apconfig)) {
   Serial.println("[FAILED]");
   return;
  }
  Serial.println("[OK]");

  // WIFI
  Serial.print("Setting up Wifi AP ");
  WiFi.softAP(apconfig.ssid, apconfig.password);
  Serial.println("[OK]");

  // IP
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}

/**
 * @loop
 */
void loop(){


  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {        
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                  
          if (currentLine.length() == 0) {
     
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
    
            
            // serve default
            if (header.indexOf("GET /ui.css") >= 0) {
              client.println(ReadFile("/ui.css"));
            } else if (header.indexOf("GET /ui.js") >= 0) {
              client.println(ReadFile("/ui.js"));
            } else {
              client.println("LOADED"); 
              client.println(ReadFile("/ui.html"));
            }
            
            break;
            
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
