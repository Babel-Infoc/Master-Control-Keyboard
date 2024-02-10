//////////////////////////////////////
//           Library Setup          //
//////////////////////////////////////

// https://github.com/OfflineRocket/Sanctuary-Keyboard-Firmware
// https://docs.arduino.cc/learn/built-in-libraries/eeprom/

// BLE Keyboard libraries
#include <BleKeyboard.h>
#include <EEPROM.h>

// LED Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define SHADERSPEED 0.2 
// LED key pulse effects (circle pulse 2)
float speed = 1.0;
float offset = 0.1;
float size = 4.0;

// Keyboard files
#include "pin_definitions.h"
#include "Matrix.h"

// GC9A01 display libraries
#include <SPI.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
AnimatedGIF gif;

// Include the necessary libraries for the GC9A01 display
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

// Define the display dimensions
#define TFT_WIDTH 240
#define TFT_HEIGHT 240

// Choose LCD Animation
//#include "ISS.h"
//#define GIF_IMAGE ISS
// #include "terminal.h"
// #define GIF_IMAGE terminal

// Rotary encoder library
// #include <SimpleRotary.h>

//////////////////////////////////////
//          Component Setup         //
//////////////////////////////////////

// Setup keyboard
BleKeyboard Kbd("Master Control Keyboard", "Babel Infocalypse", 100);

// Setup LEDs
Adafruit_NeoPixel strip1(LED1_NUM, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED2_NUM, LED2_PIN, NEO_GRB + NEO_KHZ800);

/*
// Define the main body LED matrix
const short int LEDrows = 14, LEDcols = 19;
const short int LEDMatrix[LEDrows][LEDcols] = {
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 , 1 ,-1 , 2 ,-1 , 3 ,-1 , 4 ,-1 , 5 ,-1 , 6 ,-1 , 7 ,-1 , 8 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 , 9 ,-1 , 10,-1 , 11,-1 , 12,-1 , 13,-1 , 14,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 15,-1 , 16,-1 ,-1 },
  {-1 ,-1 , 17,-1 , 18,-1 , 19,-1 , 20,-1 , 21,-1 , 22,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 , 23,-1 , 24,-1 , 25,-1 , 26,-1 , 27,-1 , 28,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 29,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 , 31,-1 , 32,-1 , 33,-1 , 34,-1 , 35,-1 , 36,-1 ,-1 ,-1 , 30,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 37,-1 ,-1 ,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 38,-1 ,-1 },
  {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 }
};
*/

// GC9A01 display setup
TFT_eSPI tft = TFT_eSPI();

//////////////////////////////////////
//            Void Setup            //
//////////////////////////////////////

void setup()
{
  // Initialize Column output pin and set default state
  for (int i = 0; i < NumCols; i++)
  {
    pinMode(Cols[i], OUTPUT);
    digitalWrite(Cols[i], LOW);
  }

  // Initialize Row pins as input, with Pulldown
  for (int i = 0; i < NumRows; i++)
  {
    pinMode(Rows[i], INPUT_PULLDOWN);
  }

  // Initialise Bluetooth Keyboard
  Kbd.begin();

  // Initialise LEDs
  strip1.begin();
  strip1.show();
  //strip1.setBrightness(20);
  strip2.begin();
  strip2.show();
  //strip2.setBrightness(20);

  // Initialise GC9A01
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
}

int ColCnt = 0; // Keep track of scanned row - needs to be outside of the void loop so it isn't reset

//////////////////////////////////////
//             Void Loop            //
//////////////////////////////////////

void loop()
{
  //SlowFade( 1, 50, 1000 );
  //ripples_abberation(1); // brightness multiplier

  CirclePulse();
  //AnimatedGif();
  // Check if the keyboard is connected, if so, scan the matrix
  if (Kbd.isConnected())
  {
    // Initialize new Row to scan
    digitalWrite(Cols[ColCnt], HIGH);

    // Check columns
    int RowCnt = 0;

    // Repeat until all columns are scanned
    while (RowCnt <= (NumRows - 1))
    {
      bool isPressed = (digitalRead(Rows[RowCnt]) == HIGH && KeyIsPressed[ColCnt][RowCnt]); // Checks that isPressed is off
      if (isPressed)
      {
        switch (KeyMatrix[ColCnt][RowCnt]) // Find the switch matching Col and Row
        {
          // Assign the current key as ON, so it doesn't constantly press the button
          KeyIsPressed[ColCnt][RowCnt] = true;
          // Serial.print( KeyMatrix[a][ColCnt] );
        }
      }
      // Otherwise, check if the switch was released
      else if (digitalRead(Rows[RowCnt]) == LOW && KeyIsPressed[ColCnt][RowCnt])
      {
        // Switch based on the switch released
        switch (KeyMatrix[ColCnt][RowCnt])
        {
          default:
            Kbd.release(KeyMatrix[ColCnt][RowCnt]);
        }
        // Let the keyboard know it's off, and to not constantly release keys that aren't released
        KeyIsPressed[ColCnt][RowCnt] = false;
      }
      // Increase column to scan
      RowCnt++;
    }

    // Reset back to original col to scan
    digitalWrite(Cols[ColCnt], LOW);
    // Increase col outputted
    ColCnt++;
    // Loop back to original row if out of the number of cols
    if (ColCnt >= (NumCols))
    {
      ColCnt = 0;
    }
  }
}





// Slow fade
float interval = 0;
float brightness = 0;
void SlowFade(int min, int max, int time)
{
  float delta = ((max - min) / time) ;

  if (interval == 0){
    brightness = min;
  }
  if ( interval < ( time / 2 )) {
    brightness += delta;
    strip2.fill(strip2.Color(brightness, brightness, brightness));
  }
  if ( interval >= ( time / 2 )) {
    brightness -= delta;
    strip2.fill(strip2.Color(brightness, brightness, brightness));
  }
  strip2.show();

  if (interval < time) {
    interval++;
  } else {
    interval = 0;
  }
}










float exponentialIn(float t) {
  return t == 0.0 ? t : pow(2.0, 12.0 * (t - 1.0));
}

float map(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

float clamp(float value, float minVal, float maxVal) {
  if (value < minVal) {
    return minVal;
  } else if (value > maxVal) {
    return maxVal;
  } else {
    return value;
  }
}
/*
float smoothstep(float edge0, float edge1, float x) {
  float t = clamp(map(x, edge0, edge1, 0.0, 1.0), 0.0, 1.0);
  return t * t * (3.0 - 2.0 * t);
}
*/
void CirclePulse()
{
  float aspect = TFT_WIDTH / TFT_HEIGHT;
  float t = millis() * speed / 1000.0;
  float t2 = (millis() * speed + offset * 1000.0) / 1000.0;
  
  //t = fmod(t, 1.0);
  //t2 = fmod(t2, 1.0);
  //
  //t = clamp(exponentialIn(t), 0.0, 1.0);
  //t2 = clamp(exponentialIn(t2), 0.0, 1.0);
  
  float circle1 = smoothstep(0.0, 0.01, (1.0 - map(t, 0.0, 1.0, 0.0, 1.0)) * size);
  float circle2 = smoothstep(0.0, 0.01, (1.0 - map(t2, 0.0, 1.0, 0.0, 1.0)) * size);
  
  float pulse = circle2 - circle1;
  pulse = clamp(pulse, 0.0, 1.0);
  
  // Output to screen
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, map(pulse, 0.0, 1.0, 0.0, TFT_WIDTH / 2), TFT_WHITE);
}





