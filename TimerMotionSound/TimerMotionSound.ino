#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    25
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          75
#define FRAMES_PER_SECOND   60

const int buttonPin = 3;     // the number of the pushbutton pin
int buttonState = 0;

const int motionPin = 6;
int motionState = 0;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
//typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
//SimplePatternList gPatterns = { count };


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

unsigned long changemillis;
unsigned long totalTime; // in millis
unsigned long lastClickTime;
unsigned long ledLength = 2000;

int clickCount = 0;
int countMax = 25;

int inputDelay = 1000;

bool motionOff = true;
  
void loop()
{
  buttonState = digitalRead(buttonPin);
  motionState = digitalRead(motionPin);
  
  if (motionState == HIGH && motionOff) {
    motionOff = false;
    // turn LED on:
    if (millis() > lastClickTime + inputDelay) {
        if (clickCount <= NUM_LEDS) {
            clickCount++;
        }
        leds[clickCount] += CHSV( gHue, 255, 192);
        lastClickTime = millis();
        totalTime += ledLength;
    }
  }
  
  if (millis() > lastClickTime + inputDelay) {
      motionOff = true;
//      lastClickTime = millis();
  }

  unsigned long now = millis();
  if (now - lastClickTime > ledLength) {
    leds[clickCount] = CHSV(gHue,0,0);
    totalTime -= ledLength;
    lastClickTime = millis();
    if (clickCount >= 0) {
          clickCount--;
    }
    Serial.println(clickCount);
  }

  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}
