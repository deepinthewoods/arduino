#include <FastLED.h>

#define LED_PIN  1

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 12

// Helper functions for an two-dimensional XY matrix of pixels.
// Simple 2-D demo code is included as well.
//
//     XY(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             No error checking is performed on the ranges of x and y.
//
//     XYsafe(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             Error checking IS performed on the ranges of x and y, and an
//             index of "-1" is returned.  Special instructions below
//             explain how to use this without having to do your own error
//             checking every time you use this function.  
//             This is a slightly more advanced technique, and 
//             it REQUIRES SPECIAL ADDITIONAL setup, described below.


// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;

// This function will return the right 'led index number' for 
// a given set of X and Y coordinates on your matrix.  
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.  
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < kMatrixWidth; x++) {
//      for( uint8_t y = 0; y < kMatrixHeight; y++) {
//      
//        // Here's the x, y to 'led index' in action: 
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//      
//      }
//    }
//
//
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define NUM_POTS 4
#define HUE_ODD 200
#define HUE_EVEN 140
#define PARTIAL_HUE_CHANGE 20
#define POT_JITTER_THRESHOLD 3

CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}



int potPin[] = {23, 25, 24, 22};
int potValues[] = {0, 0, 0, 0};
int potStates[] = {0, 0, 0, 0};

void loop()
{
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
    
    for (int i = 0; i < NUM_POTS; i++){
      int newVal = 1023 - analogRead(potPin[i]);
      if (abs(newVal - potValues[i]) > POT_JITTER_THRESHOLD)
        potValues[i] = newVal;
    }
    //Serial.print(potValues[1]);
    DrawOneFrame(potStates);
    FastLED.show();
    
    //Serial.println();
}

void DrawOneFrame( int sizes[])
{
  //kMatrixWidth kMatrixHeight 
  
  //leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
  clearMatrix();
  //TODO clear
  byte x = kMatrixWidth/4;
  byte y = kMatrixHeight/4;
  byte siz = 2;
  drawBox(x, y, siz, potValues[0], HUE_EVEN);
  x *= 3;
  drawBox(x, y, siz, potValues[1], HUE_ODD);
  y *= 3;
  drawBox(x, y, siz, potValues[2], HUE_EVEN);
  x /= 3;
  drawBox(x, y, siz, potValues[3], HUE_ODD);
  
}

void clearMatrix(){
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
}

void drawBox(byte x, byte y, byte siz, int value, int hue){
  // x = max(siz, x);
  //x = min(kMatrixWidth-siz-1, x);
  if (siz > kMatrixHeight || siz > kMatrixWidth) return;
  int totalPixels = siz * 8 - 4;
  int divisor = 1023 / totalPixels;
  
  int totalFullPixels = value / divisor;
  int partialPixel = PARTIAL_HUE_CHANGE - map(value % divisor, 0, divisor, 0, PARTIAL_HUE_CHANGE);
  int partialPixelBrightness =  map(value % divisor, 0, divisor, 200, 255);
  
  int totalDrawn = 0;
  for (int xx = x - siz ; xx < x + siz; xx++){
    if (totalDrawn >= totalFullPixels){
      leds[ XY(xx, y - siz )]  = CHSV(hue + partialPixel, 255, partialPixelBrightness);
      return;
    } else
      leds[ XY(xx, y - siz)]  = CHSV(hue, 255, 255);
    totalDrawn++;
    
  }
  
  for (int yy = y - siz+1; yy < y + siz-1; yy++){
     if (totalDrawn >= totalFullPixels){
      leds[ XY(x + siz-1, yy)]  = CHSV(hue + partialPixel, 255, partialPixelBrightness);
      return;
    } else
      leds[ XY(x + siz-1, yy)]  = CHSV(hue, 255, 255);
    totalDrawn++;
  }
  
  for (int xx = x + siz-1 ; xx >= x - siz; xx--){
     if (totalDrawn >= totalFullPixels){
      leds[ XY(xx, y + siz-1)]  = CHSV(hue + partialPixel, 255, partialPixelBrightness);
      return;
    } else
      leds[ XY(xx, y + siz-1)]  = CHSV(hue, 255, 255);
    totalDrawn++;
  }
  for (int yy = y + siz-2; yy >= y - siz+1; yy--){
     if (totalDrawn >= totalFullPixels){
      leds[ XY(x - siz, yy)]  = CHSV(hue + partialPixel, 255, partialPixelBrightness);
      return;
    } else
      leds[ XY(x - siz, yy)]  = CHSV(hue, 255, 255);
    totalDrawn++;
  }
    
}



void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  for (int i = 0; i < sizeof(potPin); i++)
    pinMode(potPin[i], INPUT);
  //Serial.begin(115200);
}
