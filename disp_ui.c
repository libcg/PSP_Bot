// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspgu.h>

#include "disp.h"
#include "game.h"
#include "level.h"
#include "common.h"
#include "controls.h"
#include "language.h"

Fade ui_fade = {FADE_OUT,0.f,10.f,222,0,0.f,BLACK};
float x_scroll = 378.f;


void drawSquareMatrix(const char* text, int column, int x, int y, int w, int h)
{
  if (ui_fade.progress == 0.f) return;

  int tx, ty, sx=0, sy=0, i, j, vnbrS=0, vnbrI=0;

  // Draw rotated text
  sceGuEnable(GU_BLEND);
  intraFontSetStyle(font,1.f,G_MODULATE(GRAY,255,255 * ui_fade.progress),0,
                    90.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(font,x,(h*S_FUNC_H + (h-1)*MATRIX_DECAL)/2 + y,text);

  // Draw some squares (with logos)
  Vertex* VertS   = sceGuGetMemory(2 * w * h * sizeof(Vertex));
  tVertex* tVertI = sceGuGetMemory(2 * w * h * sizeof(tVertex));

  ty = y;
  for (j=0; j!=h; j++, ty += S_FUNC_H + MATRIX_DECAL)
  {
    tx = x + 3*MATRIX_DECAL;
    for (i=0; i!=w; i++, tx += S_FUNC_W + MATRIX_DECAL)
    {
      if (column+i == ui.matrix_x && j == ui.matrix_y) sx = tx, sy = ty;
      VertS[vnbrS].x = tx;
      VertS[vnbrS].y = ty;
      vnbrS++;
      VertS[vnbrS].x = VertS[vnbrS-1].x + S_FUNC_W;
      VertS[vnbrS].y = VertS[vnbrS-1].y + S_FUNC_H;
      vnbrS++;
      if (game.func_matrix[column+i][j] > 0)
      {
        tVertI[vnbrI].x = tx;
        tVertI[vnbrI].y = ty;
        tVertI[vnbrI].u = (game.func_matrix[column+i][j]-1) * FUNC_W;
        tVertI[vnbrI].v = 0;   
        vnbrI++;
        tVertI[vnbrI].x = tVertI[vnbrI-1].x + S_FUNC_W;
        tVertI[vnbrI].y = tVertI[vnbrI-1].y + S_FUNC_H;
        tVertI[vnbrI].u = tVertI[vnbrI-1].u + FUNC_W;
        tVertI[vnbrI].v = tVertI[vnbrI-1].v + FUNC_H;   
        vnbrI++;
      }
    }
  }

  sceGuDisable(GU_DEPTH_TEST);
  sceGuColor(G_MODULATE(0x3F606060,255,255 * ui_fade.progress));
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,vnbrS,0,VertS);

  loadTex(img.func,GU_LINEAR,1);
  sceGuColor(G_MODULATE(WHITE,255,255 * ui_fade.progress));
  sceGuEnable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TEXTURE_16BIT|
                 GU_TRANSFORM_2D,vnbrI,0,tVertI);
  sceGuColor(WHITE);

  // Selector
  if (!(sx==0 && sy==0))
  {
    gBeginRects(img.selector_small);
    gSetCoordXY(sx-1,sy-1);
    gSetAlpha(255*ui_fade.progress);
    gAdd();
    gEnd();
  }
}


void drawFuncSelector()
{
  if (ui_fade.progress == 0.f) return;

  int i, tx, ty, sx=0, sy=0, vnbr=0;
  tVertex* tVert = sceGuGetMemory(2 * FUNC_NBR * sizeof(tVertex));
  tx = G_SCR_W/2 - (FUNC_NBR*FUNC_W + (FUNC_NBR-1)*FUNC_LIST_DECAL)/2;
  ty = FUNC_LIST_Y;

  for (i=0; i!=FUNC_NBR; i++, tx += FUNC_W+FUNC_LIST_DECAL)
  {
    if (i == ui.func_selected) sx = tx, sy = ty;
    tVert[vnbr].x = tx;
    tVert[vnbr].y = ty;
    tVert[vnbr].u = i * FUNC_W;
    tVert[vnbr].v = 0; 
    vnbr++;  
    tVert[vnbr].x = tVert[vnbr-1].x + FUNC_W;
    tVert[vnbr].y = tVert[vnbr-1].y + FUNC_H;
    tVert[vnbr].u = tVert[vnbr-1].u + FUNC_W;
    tVert[vnbr].v = tVert[vnbr-1].v + FUNC_H;
    vnbr++;

    // Highlight
    if (lvl.highlight[i])
    {
      gBeginRects(img.highlight);
      gSetCoordXY(tx,ty);
      gSetAlpha(255*ui_fade.progress);
      gAdd();
      gEnd();
    }
  }

  sceGuColor(G_MODULATE(WHITE,255,255 * ui_fade.progress));
  loadTex(img.func,GU_NEAREST,1);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TEXTURE_16BIT|
                 GU_TRANSFORM_2D,2*i,0,tVert);

  // Selector
  gBeginRects(img.selector_big);
  gSetCoordXY(sx-1,sy-1);
  gSetAlpha(255*ui_fade.progress);
  gAdd();
  gEnd();

  // Description
  intraFontSetStyle(font,1.f,G_MODULATE(GRAY,255,255 * ui_fade.progress),0,
                    0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(font,G_SCR_W/2,sy+FUNC_H+20,
                 text.game.func.list[ui.func_selected]);
  sceGuDisable(GU_DEPTH_TEST);
}


void drawControlInfos()
{
  if (ui_fade.progress == 0.f) return;
  gColor TRED = G_MODULATE(RED,255,127);

  // L
  intraFontSetStyle(font,1.f,G_MODULATE(GRAY,255,255 * ui_fade.progress),
                    (testButton(PSP_CTRL_LTRIGGER,0) && ui_fade.progress == 1.f
                    ? TRED : 0), 0.f,INTRAFONT_ALIGN_LEFT);
  intraFontPrint(font,1,45,"<L");

  // R
  intraFontSetStyle(font,1.f,G_MODULATE(GRAY,255,255 * ui_fade.progress),
                    (testButton(PSP_CTRL_RTRIGGER,0) && ui_fade.progress == 1.f
                    ? TRED : 0), 0.f,INTRAFONT_ALIGN_RIGHT);
  intraFontPrint(font,G_SCR_W-2,45,"R>");

  // Select & Start
  intraFontSetStyle(font,1.f,G_MODULATE(GRAY,255,255 * ui_fade.progress),
                    (testButton(PSP_CTRL_START,0) &&
                    ui_fade.progress == 1.f ? TRED : 0),
                    0.f,INTRAFONT_SCROLL_THROUGH);
  x_scroll = intraFontPrintColumn(font,x_scroll,G_SCR_H-10,
                                  90,text.game.func.ctrl);

  sceGuDisable(GU_DEPTH_TEST);
}


void drawInGameInfos()
{
  if (ui_fade.progress == 1.f) return;

  intraFontSetStyle(font,1.f,G_MODULATE(BLACK,255,255 * (1.f-ui_fade.progress)),0,
                    0.f,INTRAFONT_ALIGN_LEFT);
  intraFontPrint(font,10,G_SCR_H-10,text.game.title);

  intraFontSetStyle(font,1.f,G_MODULATE(BLACK,255,255 * (1.f-ui_fade.progress)),0,
                    0.f,INTRAFONT_ALIGN_RIGHT);
  intraFontPrint(font,G_SCR_W-10,G_SCR_H-10,text.game.setup_robot);

  if (intrp.running) intraFontPrint(font,G_SCR_W-10,G_SCR_H-25,text.game.stop_robot);

  intraFontPrintf(font,G_SCR_W-10,20,text.game.instructions,game.instructions_count);

  sceGuDisable(GU_DEPTH_TEST);
}


void dispUI()
{
  sceGuDisable(GU_DEPTH_TEST);
  sceGuColor(WHITE);

  drawInGameInfos();

  setFadeMode(&ui_fade,(game.state == GAME_FUNC_SETTING)?FADE_IN:FADE_OUT,0);
  drawFade(&ui_fade);

  drawFuncSelector();
  drawSquareMatrix(text.game.func.f[0],0,10,100,F_MAIN_W,MATRIX_H);
  drawSquareMatrix(text.game.func.f[1],F_MAIN_W,160,100,F_FUNC_W,MATRIX_H);
  drawSquareMatrix(text.game.func.f[2],F_MAIN_W+F_FUNC_W,270,100,F_FUNC_W,MATRIX_H);
  drawControlInfos();
}


void dispCredits()
{
  sceGuDisable(GU_DEPTH_TEST);

  intraFontSetStyle(font,1.f,WHITE,0,0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(font,G_SCR_W/2,100,text.credits);
}


void dispSplash()
{
  sceGuDisable(GU_DEPTH_TEST);

  gBeginRects(img.neo_compo);
  gSetCoordXY(G_SCR_W/2-img.neo_compo->w/2,5);
  gAdd();
  gEnd();
  intraFontSetStyle(font,1.f,WHITE,0,0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(font,480/2,264,"presented by Meastnt and Geecko");
}


void dispGameIntro()
{
  sceGuDisable(GU_DEPTH_TEST);

  intraFontSetStyle(font,1.f,WHITE,0,0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrintf(font,G_SCR_W/2,100,text.game.intro_title,lvl.nbr);

  intraFontSetStyle(font,1.f,WHITE,0,0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(font,G_SCR_W/2,150,text.game.intro);
}


void dispOutro()
{
  sceGuDisable(GU_DEPTH_TEST);

  intraFontSetStyle(font,1.f,WHITE,0,0.f,INTRAFONT_ALIGN_CENTER);
  intraFontPrintf(font,G_SCR_W/2,100,text.outro,game.instructions_count);
}

// EOF
