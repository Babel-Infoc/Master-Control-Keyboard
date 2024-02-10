#ifndef MATRIX_H
#define MATRIX_H

#include "pin_definitions.h"

// extern keyword tells the compiler that the actual
// values will be available at link time

extern short Rows[NumRows];
extern short Cols[NumCols];

extern bool KeyIsPressed[NumCols][NumRows];

extern int KeyMatrix[NumCols][NumRows];

#endif // MATRIX_H