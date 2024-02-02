#include "Matrix.h"
#include <BleKeyboard.h>

short Rows[NumRows] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8};
short Cols[NumCols] = {COL1, COL2, COL3, COL4, COL5, COL6, COL7, COL8};

// Array to check if a key is currently pressed - Initiate everything to false
bool KeyIsPressed[NumRows][NumCols] = { false };

//  ------------------- Key matrix -------------------  //
int KeyMatrix[NumRows][NumCols] =
{
    'X',           'X',    'X',    'X',    'X',    'X',    'X',    'X',
    KEY_ESC,       '1',    '2',    '3',    '4',    '5',    'X',    'X',
    KEY_TAB,       'q',    'w',    'e',    'r',    't',    ' ',    ' ',
    KEY_LEFT_SHIFT,'a',    's',    'd',    'f',    'g',    'X',    'X',
    KEY_LEFT_CTRL, 'z',    'x',    'c',    'v',    'b',    'X',    'X',
    ' ',           ' ',    ' ',    ' ',    ' ',    ' ',    'X',    'X'
};

// ------------------- Macros -------------------  //

//send_string_as_keyreps(&Kbd, "a");