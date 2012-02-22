// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspgu.h>
#include <math.h>

#include "libraries/glib2d.h"

#include "common.h"
#include "disp.h"
#include "level.h"
#include "game.h"


void drawLevel()
{
  tVertex* tVertB = sceGuGetMemory(2 * 128      * sizeof(tVertex));
  tVertex* tVertS = sceGuGetMemory(2 * LEVEL_SIDE*LEVEL_SIDE * sizeof(tVertex));

  int i, j, k, x, y, z, vnbrB=0, vnbrS=0;
  for (i=0; i!=LEVEL_SIDE; i++)
  {
    x = G_SCR_W/2 - (i+1)*(SQUARE_W/2-2);
    y = LEVEL_Y + i*(SQUARE_H/2-1);
    z = 2*LEVEL_SIDE - i;

    for (j=0; j!=LEVEL_SIDE; j++, x += SQUARE_W/2-2, y += SQUARE_H/2-1, z--)
    {
      // Blocks
      for (k=0; k!=lvl.map[j][i].h; k++)
      {
        tVertB[vnbrB].x = x;
        tVertB[vnbrB].y = y - k*(BLOCK_H-1) - 6;
        tVertB[vnbrB].z = z;
        tVertB[vnbrB].u = 0;
        tVertB[vnbrB].v = 0;   
        vnbrB++;
        tVertB[vnbrB].x = tVertB[vnbrB-1].x + BLOCK_W;
        tVertB[vnbrB].y = tVertB[vnbrB-1].y + BLOCK_IMG_H;
        tVertB[vnbrB].z = tVertB[vnbrB-1].z;
        tVertB[vnbrB].u = tVertB[vnbrB-1].u + BLOCK_W;
        tVertB[vnbrB].v = tVertB[vnbrB-1].v + BLOCK_IMG_H;   
        vnbrB++;
      }
      // Square on top
      tVertS[vnbrS].x = x;
      tVertS[vnbrS].y = y - k*(BLOCK_H-1);
      tVertS[vnbrS].z = z;
      tVertS[vnbrS].u = (lvl.map[j][i].state) * SQUARE_W;
      tVertS[vnbrS].v = 0;   
      vnbrS++;
      tVertS[vnbrS].x = tVertS[vnbrS-1].x + SQUARE_W;
      tVertS[vnbrS].y = tVertS[vnbrS-1].y + SQUARE_H;
      tVertS[vnbrS].z = tVertS[vnbrS-1].z;
      tVertS[vnbrS].u = tVertS[vnbrS-1].u + SQUARE_W;
      tVertS[vnbrS].v = tVertS[vnbrS-1].v + SQUARE_H;   
      vnbrS++;
    }
  }

  loadTex(img.block,GU_NEAREST,1);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TEXTURE_16BIT|
                 GU_TRANSFORM_2D,vnbrB,0,tVertB);
  loadTex(img.square,GU_NEAREST,1);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TEXTURE_16BIT|
                 GU_TRANSFORM_2D,vnbrS,0,tVertS);
}


void drawRobot()
{
  // Robot "breathing" cycle
  if ((robot.cycle += 3) >= 360) robot.cycle -= 360;
  short br_y = 1.9f * sinf(TO_RAD(robot.cycle));

  // Draw it
  gBeginRects(img.robot);
  gSetCoordXYZ(G_SCR_W/2 + (robot.mx-robot.my-1)*(SQUARE_W/2-2) +
               (SQUARE_W-ROBOT_W) / 2,
              
               LEVEL_Y + (robot.my+robot.mx+1)*(SQUARE_H/2-1) +
               ((robot.flying) ? br_y - ROBOT_FLY_H : 0) -
               ROBOT_H - SQU_ROBOT.h*(BLOCK_H-1) + 5,
              
               2*LEVEL_SIDE - robot.mx - robot.my);
  gSetCropWH(ROBOT_W,ROBOT_H);
  gSetCropXY(robot.dir*ROBOT_W,0);
  gSetScaleWH(ROBOT_W,ROBOT_H);
  gAdd();
  gEnd();
}


void dispGame()
{
  sceGuEnable(GU_DEPTH_TEST);
  drawLevel();
  drawRobot();
  drawFade(&reset_fade);
}

// EOF
