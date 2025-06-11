#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1: 

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 30

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip1(LED_COUNT, PIN_CLOCK, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, PIN_DATA, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//eye color 
uint32_t color = strip1.Color(255, 0, 0);

void setup() {
  strip2.begin();
  strip2.setBrightness(255);
  strip2.show();
  strip1.begin();
  strip1.setBrightness(255);
  strip1.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:
  strip1.fill(color, 0, 29);
  strip1.show();

  strip2.fill(color, 0, 29);
  strip2.show();

}
