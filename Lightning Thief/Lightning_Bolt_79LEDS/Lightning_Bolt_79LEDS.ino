#include <Adafruit_NeoPixel.h>          // Library for NeoPixels

#define pinPix 4                       // Pin driving NeoPixel Ring or String
#define numPix 79                       // Number of NeoPixels in the Ring or Strip

#define pinPix2 3                       // Pin driving NeoPixel Ring or String
#define numPix2 79                       // Number of NeoPixels in the Ring or Strip


// Setup NeoPixel Ring
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number driving the strip
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel myNeoPixels = Adafruit_NeoPixel(numPix, pinPix, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel myNeoPixels2 = Adafruit_NeoPixel(numPix, pinPix2, NEO_GRB + NEO_KHZ800);

uint32_t amber = myNeoPixels.Color(255, 100, 0);
uint32_t amber2 = myNeoPixels2.Color(255, 100, 0);


void setup() {

  myNeoPixels.setBrightness(255);
  myNeoPixels.begin();   // Initialize the NeoPixel array in the Arduino's memory,
  myNeoPixels.show();    // turn all pixels off, and upload to ring or string

  myNeoPixels2.setBrightness(255);
  myNeoPixels2.begin();   // Initialize the NeoPixel array in the Arduino's memory,
  myNeoPixels2.show();    // turn all pixels off, and upload to ring or string

  Serial.begin(9600);
}

void loop() {
  breathe(20, 100, 255,100,0);       // Pause; Number of steps; Full-on R,G,B values
  //Serial.println("Ha");
}


// Pause = delay between transitions
// Steps = number of steps
// R, G, B = Full-on RGB values
void breathe(int pause, int steps, byte R, byte G, byte B) { 

  int tmpR, tmpG, tmpB;         // Temp values
  int tmpR2, tmpG2, tmpB2;         // Temp values
  int s2;
  
  // Fade up
  for (int s=10; s<=steps; s++) {
    tmpR = (R * s) / steps;     // Multiply first to avoid truncation errors  
    tmpG = (G * s) / steps;
    tmpB = (B * s) / steps;

    s2 = abs(s-steps)+10;
    Serial.print(s);
    Serial.print(" ");
    Serial.println(s2);
    tmpR2 = (R * s2) / steps;     // Multiply first to avoid truncation errors  
    tmpG2 = (G * s2) / steps;
    tmpB2 = (B * s2) / steps;
    
    for (int i=0; i<40; i++) {
      myNeoPixels.setPixelColor(i,tmpR,tmpG,tmpB);
      myNeoPixels2.setPixelColor(i,tmpR2,tmpG2,tmpB2);

    }

    myNeoPixels.fill(amber, 41, 39);
    myNeoPixels2.fill(amber2, 41, 39);

    myNeoPixels.show();
    myNeoPixels2.show();
    delay(pause);
  }    

  // Fade down
  for (int s=steps; s>10; s--) {
    tmpR = (R * s) / steps;     // Multiply first to avoid truncation errors  
    tmpG = (G * s) / steps;
    tmpB = (B * s) / steps;

    s2 = abs(s-steps)+10;
    tmpR2 = (R * s2) / steps;     // Multiply first to avoid truncation errors  
    tmpG2 = (G * s2) / steps;
    tmpB2 = (B * s2) / steps;
    
    for (int i=0; i<40; i++) {
      myNeoPixels.setPixelColor(i,tmpR,tmpG,tmpB);
      myNeoPixels2.setPixelColor(i,tmpR2,tmpG2,tmpB2);
    }

    myNeoPixels.fill(amber, 41, 39);
    myNeoPixels2.fill(amber2, 41, 39);

    myNeoPixels.show();
    myNeoPixels2.show();
    delay(pause);
  }    

  delay(pause * 5); /*Pause before the beginning of new loop */
}