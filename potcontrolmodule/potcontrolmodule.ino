#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>



#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "images.h"


//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16
#define LED_AMOUNT 32
#define LED_PIN 1 //txd





int demoMode = 0;
int counter = 1;
int potPin[] = {5, 6, 7, 8};
int potValues[] = {17, 18, 19, 20};



CRGB leds[LED_AMOUNT];

void setup() {
 
  
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_AMOUNT);


  
  for (int i = 0; i < sizeof(potPin); i++)
    //pinMode(potPin[i], INPUT);

  for (int i = 0; i < LED_AMOUNT; i++)
    leds[i] = CHSV(i*4,255,255);

 
}



void loop() {
  // clear the display
  //display.clear();
  // draw the current demo method
  //drawButtons();
  // write the buffer to the display
  //display.display();

  for (int i = 0; i < 4; i++){
   // potValues[i] = analogRead(potPin[i]);
  }

  leds[1] = CRGB(0,0,255);
  delay(1000);
  FastLED.show();
  leds[1] = CHSV(128, 0, 0);
  delay(1000);
  //Serial.print("jj");
  FastLED.show();
}

