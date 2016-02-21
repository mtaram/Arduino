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
void getRGB(String hexRGB, int *rgb);
void updateEntireStrip(int red, int blue, int green);
int convertToInt(char upper,char lower);
const char* html = "<html><head><style></style></head>"
                   "<body><form action='/' method='GET'><input type='color' name='color' value='#000000'/>"
                   "<input type='submit' name='submit' value='Update RGB Strip'/></form></body></html>";

void handleRoot() {
  String toSend = html;
  if (server.hasArg("color")) {
    String rgbStr = server.arg("color");  //get value from html5 color element
    rgbStr.replace("%23","#"); //%23 = # in URI
    toSend.replace("#000000",rgbStr);     //replace our default black with new color, so when page loads our new color shows
    int rgb[3];                           //define rgb pointer for ws2812
    getRGB(rgbStr,rgb);                   //convert RGB string to rgb ints
    updateEntireStrip(rgb[0], rgb[1], rgb[2]);           //update strip
  }
  server.send(200, "text/html", toSend);
  delay(100);
  
  //digitalWrite(led, 1);
  //server.send(200, "text/plain", "hello from esp8266!");
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
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}


void getRGB(String hexRGB, int *rgb) {
  hexRGB.toUpperCase();
  char c[7]; 
  hexRGB.toCharArray(c,8);
  rgb[0] = convertToInt(c[1],c[2]); //red
  rgb[1] = convertToInt(c[3],c[4]); //green
  rgb[2] = convertToInt(c[5],c[6]); //blue  
}

int convertToInt(char upper,char lower)
{
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal >64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal >64 ? lVal - 55 : lVal - 48;
  return uVal + lVal;
}

void updateEntireStrip(int red, int blue, int green) {
  for(int i=0;i<NUMPIXELS;i++)
    {
     pixels.setPixelColor(i, pixels.Color(red,blue,green)); //setcolor
    }
    pixels.show();
    //RgbColor rgb = RgbColor(red, blue, green);     
    //for(int i=0; i < pixelCount; i++) {
    // strip.SetPixelColor(i,rgb);
    //}
    // strip.show();
}
