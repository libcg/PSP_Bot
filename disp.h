// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#ifndef DISP_H
#define DISP_H

#include "libraries/glib2d.h"
#include "libraries/intraFont.h"

#define LEVEL_Y 80
#define SQUARE_W 50
#define SQUARE_H 25
#define BLOCK_W SQUARE_W
#define BLOCK_H 18
#define BLOCK_IMG_H 31
#define ROBOT_W 42
#define ROBOT_H 37
#define ROBOT_FLY_H 4
#define FUNC_W 60
#define FUNC_H 60
#define S_FUNC_W 38
#define S_FUNC_H 38
#define FUNC_LIST_Y 10
#define FUNC_LIST_DECAL 3
#define MATRIX_DECAL 5
#define BACK_W 30
#define BACK_H 17

enum fade_mode { FADE_OUT = -1, FADE_IN = 1 };
enum fade_state { FADING, FADE_DONE };

typedef struct
{
  short x, y, z;
} Vertex;

typedef struct
{
  unsigned short u, v;
  short x, y, z;
} tVertex;

typedef struct
{
  unsigned short u, v;
  gColor color;
  short x, y, z;
} tcVertex;

typedef struct
{
  short mode;
  float alpha;
  float speed;
  short max, min; // Limits
  float progress; // [0;1]
  gColor color;
} Fade;

typedef struct
{
  gImage* back;
  gImage* square;
  gImage* block;
  gImage* robot;
  gImage* func;
  gImage* selector_big;
  gImage* selector_small;
  gImage* highlight;
  gImage* neo_compo;
} Images;

typedef struct
{
  short d[4]; // Alpha dir
  float s[4]; // Alpha speed
  float a[4]; // Alpha
} Background;

extern intraFont* font;
extern Images img;
extern Fade main_fade;
extern Background back;

// Common
void loadTex(gImage* tex, int filter, short blend);
void drawMovingBackground();
void setFadeMode(Fade* fade, short mode, short reset);
void setFadeColor(Fade* fade, short color);
short getFadeState(Fade fade);
void waitFadeDone();
void drawFade(Fade* fade);
void initDisp();

// Game
void dispGame();

// UI
void dispUI();
void dispCredits();
void dispSplash();
void dispGameIntro();
void dispOutro();

#endif
