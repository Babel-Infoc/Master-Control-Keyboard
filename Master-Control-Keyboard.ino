//////////////////////////////////////
//           Library Setup          //
//////////////////////////////////////

// https://github.com/OfflineRocket/Sanctuary-Keyboard-Firmware
// https://docs.arduino.cc/learn/built-in-libraries/eeprom/

// BLE Keyboard libraries
#include <BleKeyboard.h>
#include <EEPROM.h>

// LED Libraries
//#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
//#define SHADERSPEED 0.2

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
//#include <AnimatedGIF.h>
//AnimatedGIF gif;

// EmberGL Graphics lib
//#include <egl_device_lib.h>
#include <egl_math.h>
//#define mix lerp

// Define the display dimensions
#define TFT_WIDTH 240
#define TFT_HEIGHT 240

// Choose LCD Animation
// #include "ISS.h"
// #define GIF_IMAGE ISS
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
//         Shader animation         //
//////////////////////////////////////

using namespace egl;
vec3f hsb2rgb(vec3f c)
{
    vec3f rgb = clamp(abs(mod(c.x*6.0+vec3f(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    //return c.z * mix( vec3f(1.0), rgb, c.y);
    return c.z * lerp( vec3f(1.0), rgb, c.y);
}

using namespace egl;
vec4f mainImage(vec2f fragCoord , vec2f iResolution, float iTime)
{
  vec4f fragColor;
  vec2f p = (2.0*fragCoord-iResolution)/iResolution.y;

  //float r = length(p) * 0.9;
  float r = norm(p) * 0.9;
	vec3f color = hsb2rgb(vec3f(0.24, 0.7, 0.4));

  float a = pow(r, 2.0);
  float b = sin(r * 0.8 - 1.6);
  float c = sin(r - 0.010);
  float s = sin(a - iTime * 3.0 + b) * c;

  color *= abs(1.0 / (s * 10.8)) - 0.01;
  fragColor = clamp(vec4f(color, 1.), 0.0, 1.0);
  return fragColor;
}

uint16_t frame_buffer[TFT_WIDTH*TFT_HEIGHT] = {0};


using namespace egl; // Essential for EmberGL, do not remove
void render_shader(float iTime){
  // update the screen
  vec2f ires(TFT_WIDTH,TFT_HEIGHT);
  vec2f frag_coord(0,0);

  for(int x = 0; x < TFT_WIDTH; x++){
    for(int y = 0; y < TFT_HEIGHT; y++){
      frag_coord.x = (float)x;
      frag_coord.y = (float)y;
      //compute the pixel
      //vec4f frag_color = mainImage(frag_coord, ires, iTime);
      vec4f frag_color((sin(iTime)+1.0)/2.0,(cos(iTime)+1.0)/2.0,0,0);
      // set pixel
      frame_buffer[x + TFT_WIDTH*y] = tft.color565(255*frag_color.x, 255*frag_color.y, 255*frag_color.z);
    }
  }
  uint16_t *ptr = frame_buffer;
  //tft.pushImageDMA(0, 0, TFT_WIDTH, TFT_HEIGHT, ptr);
  //tft.setSwapBytes(false);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, TFT_WIDTH, TFT_HEIGHT, ptr);
}




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
  /*

  // Initialise Bluetooth Keyboard
  Kbd.begin();

  // Initialise LEDs
  strip1.begin();
  strip1.show();
  // strip1.setBrightness(20);
  strip2.begin();
  strip2.show();
  // strip2.setBrightness(20);

  */
  // Initialise GC9A01
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  //tft.initDMA();
  //gif.begin(BIG_ENDIAN_PIXELS);
}

int ColCnt = 0; // Keep track of scanned row - needs to be outside of the void loop so it isn't reset



//////////////////////////////////////
//             Void Loop            //
//////////////////////////////////////

void loop()
{
  float iTime = ((float)millis())/(float)1e3;
  render_shader(iTime);

  // AnimatedGif();
  //  Check if the keyboard is connected, if so, scan the matrix
  
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
  float delta = ((max - min) / time);

  if (interval == 0)
  {
    brightness = min;
  }
  if (interval < (time / 2))
  {
    brightness += delta;
    strip2.fill(strip2.Color(brightness, brightness, brightness));
  }
  if (interval >= (time / 2))
  {
    brightness -= delta;
    strip2.fill(strip2.Color(brightness, brightness, brightness));
  }
  strip2.show();

  if (interval < time)
  {
    interval++;
  }
  else
  {
    interval = 0;
  }
}



// https://github.com/EmberGL-org/EmberGL
// https://github.com/JarkkoPFC/meshlete/

// https://github.com/leonyuhanov/WOWPixelDriver


/*
