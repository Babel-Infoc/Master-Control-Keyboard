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

// Keyboard files
#include "pin_definitions.h"
#include "Matrix.h"

// GC9A01 display libraries
#include <SPI.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
AnimatedGIF gif;

// Choose LCD Animation
#include "ISS.h"
#define GIF_IMAGE ISS
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
Adafruit_NeoPixel strip3(LED3_NUM, LED3_PIN, NEO_GRB + NEO_KHZ800);

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
  strip1.setBrightness(50);
  strip2.begin();
  strip2.show();
  strip2.setBrightness(50);
  strip3.begin();
  strip3.show();
  strip3.setBrightness(50);

  // Initialise GC9A01
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
}

int RowCnt = 0; // Keep track of scanned row - needs to be outside of the void loop so it isn't reset

//////////////////////////////////////
//             Void Loop            //
//////////////////////////////////////

void loop()
{
    if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    tft.startWrite();
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
    tft.endWrite();
  }

  // Check if the keyboard is connected, if so, scan the matrix
  if (Kbd.isConnected())
  {
    // Initialize new Row to scan
    digitalWrite(Rows[RowCnt], HIGH);

    // Check columns
    int ColCnt = 0;
    // Repeat until all columns are scanned
    while (ColCnt <= (NumCols - 1))
    {
      // Check state of current position, and make sure it was previously off
      bool isPressed = (digitalRead(Cols[ColCnt]) == HIGH && KeyIsPressed[RowCnt][ColCnt]);
      if (isPressed)
      {
        // Switch based on the key pressed, allows for unique functions other than alphanumerics
        switch (KeyMatrix[RowCnt][ColCnt])
        {
        // Change the ID of the bluetooth, so you can connect to another device
        default:
          Kbd.press(KeyMatrix[RowCnt][ColCnt]);
        }
        // Assign the current key as ON, so it doesn't constantly press the button
        KeyIsPressed[RowCnt][ColCnt] = true;
        // Serial.print( KeyMatrix[a][ColCnt] );
      }
      // Otherwise, check if the switch was released
      else if (digitalRead(Cols[ColCnt]) == LOW && KeyIsPressed[RowCnt][ColCnt])
      {
        // Switch based on the switch released
        switch (KeyMatrix[RowCnt][ColCnt])
        {
        default:
          Kbd.release(KeyMatrix[RowCnt][ColCnt]);
        }
        // Let the keyboard know it's off, and to not constantly release keys that aren't released
        KeyIsPressed[RowCnt][ColCnt] = false;
      }
      // Increase column to scan
      ColCnt++;
    }

    // Reset back to original row to scan
    digitalWrite(Rows[RowCnt], LOW);
    // Increase row outputted
    RowCnt++;
    // Loop back to original row if out of the number of rows
    if (RowCnt >= (NumRows))
    {
      RowCnt = 0;
    }
  }

  Background();
}

// LED background color
void Background()
{
  for (int i = 0; i < LED1_NUM; i++)
  {
    strip1.setPixelColor(i, strip1.Color(25, 25, 25));
    strip1.show();
  }
  for (int i = 0; i < LED2_NUM; i++)
  {
    strip2.setPixelColor(i, strip2.Color(25, 25, 25));
    strip2.show();
  }
  for (int i = 0; i < LED3_NUM; i++)
  {
    strip3.setPixelColor(i, strip3.Color(25, 25, 25));
    strip3.show();
  }
}