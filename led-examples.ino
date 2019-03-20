#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 72

// Data pin that led data will be written out over
#define DATA_PIN 7

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
uint8_t opIn, redIn, greenIn, blueIn;
void setup() {
  Serial.begin(9600);
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  // delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( 0xFFB090 );
  FastLED.setBrightness(64);
  fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
}


void loop() {
  if(Serial.available() > 0) // wait until info is ready
  {
    // first byte is the operation
    opIn = (uint8_t)Serial.read();

    switch(opIn){
      /////////////////////////
      case 0xFF:  // update LEDs
      {
        fill_solid(leds,NUM_LEDS,CRGB(255,255,255));
        FastLED.show();
        break;
      }
      /////////////////////////
      case 0x1:  // fill an array of LEDs (Starting Pixel | Length of Array | RGB)
      {
        if(Serial.available() >= 5)
        {
          uint8_t arrayStart = Serial.read();
          uint8_t arrayLength = Serial.read();
          redIn = Serial.read();
          greenIn = Serial.read();
          blueIn = Serial.read();
  
          for(int i = arrayStart; i < arrayStart + arrayLength; i++)
          {
            leds[i].red = redIn;
            leds[i].green = greenIn;
            leds[i].blue = blueIn;
          }
        }
        break;
      }

      default:
      {
        break;
      }
      /////////////////////////
    }
   
  }
  delay(500);
  //FastLED.delay(1);
}
