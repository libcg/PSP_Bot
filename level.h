// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"

typedef struct
{
  short h;
  short state;
} Block;

typedef struct
{
  Block map[LEVEL_SIDE][LEVEL_SIDE];
  short x, y;
  short dir;
  short nbr; // Actual lvl loaded
  short nol; // Number of levels
  short highlight[FUNC_NBR]; // Function selector
} Level;

extern Level lvl;

void resetLevel();
void initLevel();
short nextLevel();

#endif
