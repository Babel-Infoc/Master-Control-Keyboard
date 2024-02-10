#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

/*  Pin mapping
Rows:         Columns:        GC9A01:                   LEDs:                   Module:
ROW1  4       COL1  35        TFT_MOSI 14   (SDO)       LED1  45 (main body)    Pin 3
ROW2  5       COL2  36        TFT_SCLK 21   (SCL)       LED2  48 (controller)   Pin 9
ROW3  6       COL3  37        TFT_CS   12               LED3  47 (module)       Pin 10
ROW4  7       COL4  38        TFT_DC   13                                       Pin 11
ROW5  15      COL5  39        TFT_RST  2       
ROW6  16      COL6  40        TFT_BL   1       
ROW7  17      COL7  41        
ROW8  18      COL8  42        
*/

// Key Matrix Pin Definitions
#define NumRows 8
#define NumCols 8

#define ROW1 4
#define ROW2 5
#define ROW3 6
#define ROW4 7
#define ROW5 15
#define ROW6 16
#define ROW7 17
#define ROW8 18

#define COL1 35
#define COL2 36
#define COL3 37
#define COL4 38
#define COL5 39
#define COL6 40
#define COL7 41
#define COL8 42

// Controller LED's, #LED1
#define LED1_PIN 48
#define LED1_NUM 6

// Main body LED's, #LED2
#define LED2_PIN 45
#define LED2_NUM 42

//#define FUNCTION_SW   256
//#define NULL_CON      257
//#define NEXT          258
//#define PREV          259

// GC9A01 SPI pins
// Edit "TFT_eSPI" library file named User_Setup.h

    // Section 1: Uncomment the lines:
    //      #define GC9A01_DRIVER
    //      #define TFT_HEIGHT 240 // GC9A01 240 x 240

    // Section 2: Uncomment and set Pin numbers:
    //      #define TFT_MOSI 14   // SDO
    //      #define TFT_SCLK 21   // SCL
    //      #define TFT_CS   12
    //      #define TFT_DC   13
    //      #define TFT_RST  2
    //      #define TFT_BL   1

#endif // PIN_DEFINITIONS_H
