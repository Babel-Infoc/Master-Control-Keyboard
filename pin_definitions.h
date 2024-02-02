#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

/*  Pin mapping
ROW1  4       COL1  35        SCL   21
ROW2  5       COL2  36        SDA   14
ROW3  6       COL3  37        RES   2
ROW4  7       COL4  38        DC    13
ROW5  15      COL5  39        CS    12
ROW6  16      COL6  40        BLK   1
ROW7  17      COL7  41        
ROW8  18      COL8  42        
*/


//Pin Definitions
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

//Rotary Pin Definitions
#define Rotary1 15
#define Rotary2 22

//Matrix setup
#define NumRows 8
#define NumCols 8

//LED definitions
#define LED2_PIN 48
#define LED2_NUM 6

#define FUNCTION_SW   256
#define NULL_CON      257
#define NEXT          258
#define PREV          259

// GC9A01 SPI pins
//#define DC   13     // Data/command
//#define CS   12     // Chip select
//#define BLK  1      // Backlight
//#define RES  2      // Reset
//#define SDA  14     // Data

#endif // PIN_DEFINITIONS_H
