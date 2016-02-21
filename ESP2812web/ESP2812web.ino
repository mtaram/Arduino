#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "Ray2";
const char* password = "megamachine@23";
#define NUMPIXELS      24
#define PIN            2
ESP8266WebServer server(80);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void handleRoot() {
  //digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  //digitalWrite(led, 0);
}

void handleNotFound(){
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void setup(void){
  //pinMode(led, OUTPUT);
  //digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  pixels.begin();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/off", [](){
    for(int i=0;i<NUMPIXELS;i++)
    {
     pixels.setPixelColor(i, pixels.Color(0,0,0)); //off
    }
    pixels.show();
    server.send(200, "text/plain", "red");
  });
  server.on("/red", [](){
    for(int i=0;i<NUMPIXELS;i++)
    {
     pixels.setPixelColor(i, pixels.Color(150,0,0)); //Red 
    }
    pixels.show();
    server.send(200, "text/plain", "red");
  });
  server.on("/blue", [](){
    for(int i=0;i<=NUMPIXELS;i++)
    {
     pixels.setPixelColor(i, pixels.Color(0,0,150)); //Red 
     pixels.show();
    }
    server.send(200, "text/plain", "green");
  });
  server.on("/green", [](){
    for(int i=0;i<=NUMPIXELS;i++)
    {
     pixels.setPixelColor(i, pixels.Color(0,150,0)); //Red 
     pixels.show();
    }
    server.send(200, "text/plain", "blue");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
