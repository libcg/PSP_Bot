// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspkernel.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <time.h>
#include <stdlib.h>

#include "libraries/glib2d.h"
#include "libraries/intraFont.h"

#include "disp.h"
#include "game.h"

intraFont* font;
Images img = {0};
Fade main_fade = {FADE_OUT,255.f,3.5f,255,0,0.f,BLACK};
Background back;


void loadTex(gImage* tex, int filter, short blend)
{
  sceGuEnable(GU_TEXTURE_2D);
  if (blend) sceGuEnable(GU_BLEND);
  else       sceGuDisable(GU_BLEND);
  sceGuTexFilter(filter,filter);
  sceGuTexMode(GU_PSM_8888,0,0,tex->swizzled);
  sceGuTexImage(0,tex->tw,tex->th,tex->tw,tex->data);
}


void initBackground()
{
  srand(time(NULL));
  int i;
  for (i=0; i!=4; i++)
  {
    back.d[i] = (rand()%2  ) * 2 - 1; // dir: -1 or 1
    back.s[i] = (rand()%100) / 100.f; // speed: [0;1]
    back.a[i] = (rand()%255);         // alpha: [0;255]
  }
}


void drawMovingBackground()
{
  int i;
  for (i=0; i!=4; i++)
  {
    if (back.a[i] < 0.f  ) back.d[i] =  1, back.a[i] = 0.f;
    if (back.a[i] > 255.f) back.d[i] = -1, back.a[i] = 255.f;
    back.a[i] += back.d[i] * back.s[i];
  }

  tcVertex* v = sceGuGetMemory(8 * sizeof(tcVertex));
  v[0].x = 0,        v[0].y = 0;
  v[0].u = 0,        v[0].v = 0;
  v[0].color = G_MODULATE(WHITE,255,back.a[0]);
  v[1].x = G_SCR_W,  v[1].y = 0;
  v[1].u = BACK_W-1, v[1].v = 0;
  v[1].color = G_MODULATE(WHITE,255,back.a[1]);
  v[2].x = G_SCR_W,  v[2].y = G_SCR_H;
  v[2].u = BACK_W-1, v[2].v = BACK_H-1;
  v[2].color = G_MODULATE(WHITE,255,back.a[2]);
  v[3].x = 0,        v[3].y = G_SCR_H;
  v[3].u = 0,        v[3].v = BACK_H-1;
  v[3].color = G_MODULATE(WHITE,255,back.a[3]);

  v[4].x = 0,          v[4].y = 0;
  v[4].u = BACK_W,     v[4].v = 0;
  v[4].color = G_MODULATE(WHITE,255,255-back.a[0]);
  v[5].x = G_SCR_W,      v[5].y = 0;
  v[5].u = 2*BACK_W-1, v[5].v = 0;
  v[5].color = G_MODULATE(WHITE,255,255-back.a[1]);
  v[6].x = G_SCR_W,      v[6].y = G_SCR_H;
  v[6].u = 2*BACK_W-1, v[6].v = BACK_H-1;
  v[6].color = G_MODULATE(WHITE,255,255-back.a[2]);
  v[7].x = 0,          v[7].y = G_SCR_H;
  v[7].u = BACK_W,     v[7].v = BACK_H-1;
  v[7].color = G_MODULATE(WHITE,255,255-back.a[3]);

  sceGuDisable(GU_DEPTH_TEST);
  loadTex(img.back,GU_LINEAR,1);

  sceGuDrawArray(GU_TRIANGLE_FAN,GU_COLOR_8888|GU_TEXTURE_16BIT|GU_VERTEX_16BIT|
                 GU_TRANSFORM_2D,4,0,v);
  sceGuDrawArray(GU_TRIANGLE_FAN,GU_COLOR_8888|GU_TEXTURE_16BIT|GU_VERTEX_16BIT|
                 GU_TRANSFORM_2D,4,0,v+4);
}


void setFadeMode(Fade* fade, short mode, short reset)
{
  fade->mode = mode;
  if (!reset) return;

  if      (mode == FADE_OUT) fade->alpha = fade->max, fade->progress = 1.f;
  else if (mode == FADE_IN ) fade->alpha = fade->min, fade->progress = 0.f;
}


void setFadeColor(Fade* fade, short color)
{
  fade->color = color;
}


short getFadeState(Fade fade)
{
  return ((fade.mode == FADE_IN  && fade.alpha == fade.max) ||
          (fade.mode == FADE_OUT && fade.alpha == fade.min))
         ? FADE_DONE : FADING;
}


void waitFadeDone()
{
  while (getFadeState(main_fade) != FADE_DONE) sceKernelDelayThread(1000);
}


void drawFade(Fade* fade)
{
  fade->alpha += fade->mode * fade->speed;
  if (fade->alpha > fade->max) fade->alpha = fade->max;
  else if (fade->alpha < fade->min) fade->alpha = fade->min;
  fade->progress = (float)fade->alpha / (fade->max - fade->min);

  if (fade->alpha != 0)
  {
    gBeginRects(NULL);
    gSetScaleWH(G_SCR_W,G_SCR_H);
    gSetColor(fade->color);
    gSetAlpha(fade->alpha);
    gAdd();
    gEnd();
  }
}


int dispThread(SceSize args, void *argp)
{
  while (1)
  {
    switch (game.state)
    {
      case GAME:
      case GAME_FUNC_SETTING:
        gClear(WHITE);
        drawMovingBackground();
        dispGame();
        dispUI();
      break;
      case CREDITS:
        gClear(BLACK);
        dispCredits();
      break;
      case SPLASH:
        gClear(BLACK);
        dispSplash();
      break;
      case GAME_INTRO:
        gClear(BLACK);
        dispGameIntro();
      break;
      case OUTRO:
        gClear(BLACK);
        dispOutro();
      break;
    }

    drawFade(&main_fade);

    gFlip(G_TRUE);
  }
  return 0;
}


void initDisp()
{
  // Load images
  img.back           = gTexLoad("graphics/back.png",G_TRUE);
  img.square         = gTexLoad("graphics/square.png",G_TRUE);
  img.block          = gTexLoad("graphics/block.png",G_TRUE);
  img.robot          = gTexLoad("graphics/robot.png",G_TRUE);
  img.func           = gTexLoad("graphics/functions.png",G_TRUE);
  img.selector_big   = gTexLoad("graphics/selector_big.png",G_TRUE);
  img.selector_small = gTexLoad("graphics/selector_small.png",G_TRUE);
  img.highlight      = gTexLoad("graphics/highlight.png",G_TRUE);
  img.neo_compo      = gTexLoad("graphics/neo_compo.png",G_TRUE);
  // Init libraries
  intraFontInit();
  font = intraFontLoad("flash0:/font/ltn8.pgf",INTRAFONT_CACHE_MED);
  intraFontSetEncoding(font,INTRAFONT_STRING_UTF8);
  initBackground();
  // Start display thread
  SceUID thid = sceKernelCreateThread("disp_thread",dispThread,0x11,0xFA0,0,0);
  sceKernelStartThread(thid,0,0);
}

// EOF
