#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 72

// Data pin that led data will be written out over
#define DATA_PIN 7

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
void setup() {
  Serial.begin(9600);
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(500);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip /*0xFFB090*/ );
  FastLED.setBrightness(64);

  // visual verification that setup has ended
  ledStartup(NUM_LEDS);
}

void loop() {
  
  if(Serial.available() > 0) // wait until info is ready
  {
    // first byte is the operation
    uint8_t opIn = Serial.read();

    // check the operation
    if(opIn == 0xFF) FastLED.show();
    else
    {

      /*
       * opIn == 0x1
       * Light an array of LEDs. 5 bytes are expected:
       *  1. Starting LED ( < NUM_LEDS)
       *  2. Length of LED Array ( <= NUM_LEDS - Starting LED)
       *  3. Red (0-255)
       *  4. Green (0-255)
       *  5. Blue (0-255)
       *  
       *
       */
      if(opIn == 0x1)
      {
        if(Serial.available() >= 5)
        {
          uint8_t arrayStart = Serial.read();
          uint8_t arrayLength = Serial.read();
          unsigned rgb = (Serial.read() << 16) | (Serial.read() << 8) | Serial.read();
//          uint8_t redIn = Serial.read();
//          uint8_t greenIn = Serial.read();
//          uint8_t blueIn = Serial.read();

          // only perform the operation if the array parameters are met
          if((arrayStart < NUM_LEDS) && (arrayLength <= (NUM_LEDS - arrayStart)))
          {
            for(int i = arrayStart; i < arrayStart + arrayLength; i++) leds[i] = rgb;
          }
        }
      }
      else
      {
        while(Serial.available() > 0)
        {
          char trash = Serial.read();
          Serial.write(trash);
          // blink red rapidly
          fill_solid(leds,NUM_LEDS,CRGB(5,0,0));
          FastLED.show();
          delay(50);
          fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
          FastLED.show();
          delay(50);
        }
      }
    }
  }
  
}

void ledStartup(int n)
{
  fill_solid(leds,n,CRGB(0,10,0));
  FastLED.show();
  delay(200);
  fill_solid(leds,n,CRGB(0,0,0));
  FastLED.show();
}

/*
void error(uint8_t code)
{
  Serial.print("There is an error. (Code 0x");
  Serial.print(code);
  Serial.println(")");
  while(Serial.available() > 0)
  {
    Serial.write(0);
    char trash = Serial.read();
  }
  Serial.println("Serial has been cleared.");
}

uint8_t readNextByte()
{
  while(!Serial.available()){}; // wait here until Serial is actually available
  return Serial.read();
}

uint8_t displaySerial(uint8_t input)
{
  Serial.print(count++);
  Serial.print(":\t");
  Serial.print(input);
  Serial.print("\t");
  Serial.println(micros());
  return input;
}
*/
