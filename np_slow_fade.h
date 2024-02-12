#include <Adafruit_NeoPixel.h>

// Setup LEDs
//Adafruit_NeoPixel strip1(LED1_NUM, LED1_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2(LED2_NUM, LED2_PIN, NEO_GRB + NEO_KHZ800);

struct slowfade_state {
  float interval;
  float brightness;
  Adafruit_NeoPixel strip;
};
