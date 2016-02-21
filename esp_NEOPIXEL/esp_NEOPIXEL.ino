*/
Example ESP8266 with neopixel Adafruit
Andres Sabas - The Inventor's House Hackerpsace
4 de Julio 2015
*/

#include <Adafruit_NeoPixel.h>

// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN            13

// How many NeoPixels are attached to the ESP8266?
#define NUMPIXELS      16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  // put your setup code here, to run once:
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  // put your main code here, to run repeatedly:
   for(int i=0;i<NUMPIXELS;i++){
   for (int j=0;j<=100;j++)
   {
    
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(j,j,j)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
   }
  }

}
