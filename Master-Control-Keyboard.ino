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
//#include <Adafruit_NeoMatrix.h>
//#include <Adafruit_NeoPixel.h>
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

// EmberGL Graphics lib
//#include <egl_device_lib.h>
#include <egl_math.h>
//#define mix lerp

// Define the display dimensions
#define TFT_WIDTH 240
#define TFT_HEIGHT 240

// Rotary encoder library
// #include <SimpleRotary.h>

//////////////////////////////////////
//          Component Setup         //
//////////////////////////////////////

// Setup keyboard
BleKeyboard Kbd("Master Control Keyboard", "Babel Infocalypse", 100);

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
#include "st_simple_circle.h"
#include "st_shader_art_coding_introduction.h"
#include "st_oscilloscope.h"
#include "st_sphere_of_cubes.h"




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


      //vec4f frag_color = simple_circle(frag_coord, ires, iTime);
      //vec4f frag_color = shader_art_coding_introduction(frag_coord, ires, iTime);
      vec4f frag_color = oscilloscope(frag_coord, ires, iTime);
      //vec4f frag_color = sphereOfCubes(frag_coord, ires, iTime);

      
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


/*
c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `egl::camera(egl::vec3<float>, egl::vec3<float>, float, egl::vec2<float>)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:31: multiple definition of `egl::camera(egl::vec3<float>, egl::vec3<float>, float, egl::vec2<float>)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:31: first defined here

c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `egl::launch(egl::Camera, egl::vec2<float>)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:44: multiple definition of `egl::launch(egl::Camera, egl::vec2<float>)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:44: first defined here

c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `egl::intersect_sphere(egl::Ray, egl::Sphere, float&, float&)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:58: multiple definition of `egl::intersect_sphere(egl::Ray, egl::Sphere, float&, float&)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:58: first defined here

c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `egl::step_forward(egl::Ray, float&, egl::vec3<float>&, int&, float, float, float)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:74: multiple definition of `egl::step_forward(egl::Ray, float&, egl::vec3<float>&, int&, float, float, float)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:74: first defined here

c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `egl::mysmoothstep(float)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:124: multiple definition of `egl::mysmoothstep(float)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:124: first defined here

c:/users/neuromancer/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32s3-elf-gcc/esp-2021r2-patch5-8.4.0/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\st_sphere_of_cubes.cpp.o: in function `sphereOfCubes(egl::vec2<float>, egl::vec2<float>, float)':
G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/st_sphere_of_cubes.cpp:134: multiple definition of `sphereOfCubes(egl::vec2<float>, egl::vec2<float>, float)'; C:\Users\Neuromancer\AppData\Local\Temp\arduino\sketches\2C7E8CCF49FA9CF80FF8172EF75C5AFE\sketch\sphere_of_cubes.cpp.o:G:\My Drive\Babel Cybernetics\-- Github LIVE\Master-Control-Keyboard/sphere_of_cubes.cpp:134: first defined here



collect2.exe: error: ld returned 1 exit status

exit status 1

Compilation error: exit status 1
*/