#include "FastLED.h"
//#include "global_def.h"
//#include "functions.h"
//#include <TimerOne.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    12
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   60
CRGB leds_ring[NUM_LEDS];

// Red dot leds Pin (body & cubes)
#define RED_PIN 6
uint8_t RED_BRIGHTNESS = 64;
uint8_t RED_FADEAMOUNT = 1;
// EL-Wire Pin (yellow light cable)
#define ELWIRE_PIN 5
uint8_t ELWIRE_BRIGHTNESS = 250;
uint8_t ELWIRE_FADEAMOUNT = 1;

void setup() {
  delay(1000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds_ring, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // red leds and el-wire
  pinMode(RED_PIN, OUTPUT);
  pinMode(ELWIRE_PIN, OUTPUT);
  //digitalWrite(ELWIRE_PIN, 1);

  /**
   * LED Matrix 5x11 Setup
   **/
  //configure_pinning(8,7,9);
  //matrix_init();  
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow };//, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds_ring' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds_ring' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  // do some periodic updates
  EVERY_N_MILLISECONDS( 2 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically

  // red leds updates
  EVERY_N_MILLISECONDS( 50 ) { red_fade(); }
  analogWrite(RED_PIN, RED_BRIGHTNESS);

  // el-wire updates
  EVERY_N_MILLISECONDS( 500 ) { elwire_fade(); }
  analogWrite(ELWIRE_PIN, ELWIRE_BRIGHTNESS);
  
  
  // led matrix 5x11 updates
  //displayMessage("ES KANN NUR EINEN GEBEN!  ",55);
  /*
  EVERY_N_MILLISECONDS(3){
    iteration++;
    displayMatrix();
  }
  /*for (int i = 0; i < 20; i++) {
    insertLetter(i, 100);
  }
  */
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds_ring, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds_ring[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds_ring, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds_ring[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_ring, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds_ring[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds_ring[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_ring, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds_ring[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

/**
 * Red led stuff
 **/
void red_fade()
{
  // change the brightness for next time through the loop:
  RED_BRIGHTNESS = RED_BRIGHTNESS + RED_FADEAMOUNT;

  // reverse the direction of the fading at the ends of the fade:
  if (RED_BRIGHTNESS == 0 || RED_BRIGHTNESS == 128) {
    RED_FADEAMOUNT = -RED_FADEAMOUNT ;
  }
}

/**
 * EL-Wire stuff
 **/
void elwire_fade()
{
  // change the brightness for next time through the loop:
  ELWIRE_BRIGHTNESS = ELWIRE_BRIGHTNESS + ELWIRE_FADEAMOUNT;

  // reverse the direction of the fading at the ends of the fade:
  if (ELWIRE_BRIGHTNESS == 180 || ELWIRE_BRIGHTNESS == 255) {
    ELWIRE_FADEAMOUNT = -ELWIRE_FADEAMOUNT ;
  }
}
