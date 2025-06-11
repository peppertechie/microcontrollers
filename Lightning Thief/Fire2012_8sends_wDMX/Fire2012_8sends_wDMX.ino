/// @file    Fire2012.ino
/// @brief   Simple one-dimensional fire animation
/// @example Fire2012.ino
//Using libraries from https://github.com/jostlowe/Pico-DMX

#include <Arduino.h>
#include "DmxInput.h"
#include <FastLED.h>

bool core1_separate_stack = true;

#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    25

#define FRAMES_PER_SECOND 15

bool gReverseDirection = false;

CRGB led2[NUM_LEDS];
CRGB led3[NUM_LEDS];
CRGB led4[NUM_LEDS];
CRGB led5[NUM_LEDS];
CRGB led6[NUM_LEDS];
CRGB led7[NUM_LEDS];
CRGB led8[NUM_LEDS];
CRGB led9[NUM_LEDS];

//We are going to grab all DMX channels since the start channel parameter doesn't work
DmxInput dmxInput;
#define START_CHANNEL 1
#define NUM_CHANNELS 512
//what address should this listen on for intensity
#define DMX_ADDRESS 1
volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];


//This sets the brightness and needs to be readable by both cores
volatile int bright = 100;

//Core 1 is our DMX processing.  Even if we loose DMX the LED effect will continue
void setup1()
{
  delay(3000); // sanity delay
    Serial.begin(9600);
    // Setup our DMX Input to read on GPIO 0, from channel 1 to 3
    dmxInput.begin(16, START_CHANNEL, NUM_CHANNELS);

    // Setup the onboard LED so that we can blink when we receives packets
    pinMode(LED_BUILTIN, OUTPUT);
}


//Core 0 is for LEDs
void setup()
{
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, 2, COLOR_ORDER>(led2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 3, COLOR_ORDER>(led3, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 4, COLOR_ORDER>(led4, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 5, COLOR_ORDER>(led5, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(led6, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 7, COLOR_ORDER>(led7, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(led8, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(led9, NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  FastLED.show(); //clear initial LED state
  FastLED.setBrightness(bright);
}

void loop1()
{

  // Wait for next DMX packet
   dmxInput.read(buffer);

    // Print the DMX channels
    Serial.print("Received packet: ");
    for (uint i = 0; i < sizeof(buffer); i++)
    {
        Serial.print(buffer[i]);
        Serial.print(", ");
    }
    Serial.println("");

    // Blink the LED to indicate that a packet was received
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
    digitalWrite(LED_BUILTIN, LOW);
    if (buffer[0]==0) {
     bright = buffer[DMX_ADDRESS];
    }
}


void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  Fire2012(2); // run simulation frame
  Fire2012(3); // run simulation frame  
  Fire2012(4); // run simulation frame
  Fire2012(5); // run simulation frame
  Fire2012(6); // run simulation frame
  Fire2012(7); // run simulation frame
  Fire2012(8); // run simulation frame
  Fire2012(9); // run simulation frame

  //Take the bright variable from the DMX channel and use it to set the LED brightness
  FastLED.setBrightness(bright);
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  70

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50


void Fire2012(int leds)
{

// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }

    switch (leds)
      {
        case 2:
        led2[pixelnumber] = color;
        break;
        case 3:
        led3[pixelnumber] = color;
        break;
        case 4:
        led4[pixelnumber] = color;
        break;
        case 5:
        led5[pixelnumber] = color;
        break;
        case 6:
        led6[pixelnumber] = color;
        break;
        case 7:
        led7[pixelnumber] = color;
        break;
        case 8:
        led8[pixelnumber] = color;
        break;
        case 9:
        led9[pixelnumber] = color;
        break;
        default:
        break;
      }

    }
}