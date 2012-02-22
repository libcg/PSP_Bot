// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspkernel.h>

#include "common.h"
#include "level.h"
#include "game.h"
#include "disp.h"
#include "controls.h"

Fade reset_fade = {FADE_OUT,255.f,25.f,255,0,1.f,WHITE};


void clearFuncMatrix()
{
  int i, j;
  for (j=0; j!=MATRIX_H; j++)
    for (i=0; i!=MATRIX_W; i++)
      game.func_matrix[i][j] = 0;

  ui.matrix_x = 0;
  ui.matrix_y = 0;
}


void nextInstruction()
{
  int f_id = (ui.matrix_x >= F_MAIN_W) + (ui.matrix_x >= F_MAIN_W+F_FUNC_W),
      f_x = F_MAIN_W * (f_id >= 1) + F_FUNC_W * (f_id == 2),
      f_w = (f_id > 0) ? F_FUNC_W : F_MAIN_W;

  if (++ui.matrix_x >= f_x + f_w)
  {
    ui.matrix_y++, ui.matrix_x = f_x;
    if (ui.matrix_y >= MATRIX_H)
      ui.matrix_x = f_x+f_w-1, ui.matrix_y = MATRIX_H-1;
  }  
}


void controlUI()
{
  if (game.state == GAME)
  {
    if (intrp.running && testButton(PSP_CTRL_CROSS,1)) stopProgram();
  }

  if (game.state != GAME_FUNC_SETTING) return;
  // Function selector
  ui.func_selected += testButton(PSP_CTRL_RTRIGGER,1) -
                      testButton(PSP_CTRL_LTRIGGER,1);
  ui.func_selected += FUNC_NBR * ((ui.func_selected <  0) -
                                  (ui.func_selected >= FUNC_NBR));

  // Matrix selector
  ui.matrix_x += testButton(PSP_CTRL_RIGHT,1) - testButton(PSP_CTRL_LEFT,1);
  ui.matrix_y += testButton(PSP_CTRL_DOWN,1) - testButton(PSP_CTRL_UP,1);
  ui.matrix_x += MATRIX_W * ((ui.matrix_x <  0) - (ui.matrix_x >= MATRIX_W));
  ui.matrix_y += MATRIX_H * ((ui.matrix_y <  0) - (ui.matrix_y >= MATRIX_H));

  if (testButton(PSP_CTRL_CROSS,1))
  {
    game.func_matrix[ui.matrix_x][ui.matrix_y] = ui.func_selected + 1;
    nextInstruction();
  }
  else if (testButton(PSP_CTRL_CIRCLE,1))
    game.func_matrix[ui.matrix_x][ui.matrix_y] = 0;
  else if (testButton(PSP_CTRL_SELECT,1))
    clearFuncMatrix();
}


void isLevelDone()
{
  int i, j;

  // Verify
  for (i=0; i!=LEVEL_SIDE; i++)
    for (j=0; j!=LEVEL_SIDE; j++)
      if (lvl.map[i][j].state == 1) return;

  // Increment the instructions counter
  for (i=0; i!=MATRIX_W; i++)
    for (j=0; j!=MATRIX_H; j++)
      if (game.func_matrix[i][j] > 0) game.instructions_count++;
  
  setFadeMode(&main_fade,FADE_IN,0);
  waitFadeDone();

  // This was the last level... outro time.
  if (nextLevel()) { game.state = OUTRO; return; }

  // Else, reset game state; this is intro time.
  resetGame();
  stopProgram();
  clearFuncMatrix();
  game.state = GAME_INTRO;
}


void initRobot()
{
  robot.mx = lvl.x;
  robot.my = lvl.y;
  robot.dir = lvl.dir;
  robot.flying = 0;
}


void resetGame()
{
  resetLevel();
  initRobot();

  setFadeMode(&reset_fade,FADE_OUT,1);
}


void gameInGame()
{
  if (testButton(PSP_CTRL_SQUARE,2))
  {
    if      (game.state == GAME) game.state = GAME_FUNC_SETTING;
    else if (game.state == GAME_FUNC_SETTING) game.state = GAME;
  }
  else if (testButton(PSP_CTRL_START,1))
    runProgram();

  controlUI();
  isLevelDone();
}


void gameCredits()
{
  setFadeMode(&main_fade,FADE_OUT,0);
  waitFadeDone();

  wait(6.f,PSP_CTRL_CROSS);

  setFadeMode(&main_fade,FADE_IN,0);
  waitFadeDone();

  game.state = SPLASH;
  setFadeMode(&main_fade,FADE_OUT,0);
}


// NEO Summer Retro Coding Contest 2010
void gameSplash()
{
  setFadeMode(&main_fade,FADE_OUT,0);
  waitFadeDone();

  wait(2.f,0);

  setFadeMode(&main_fade,FADE_IN,0);
  waitFadeDone();

  gTexFree(&img.neo_compo);

  game.state = GAME_INTRO;
  setFadeMode(&main_fade,FADE_OUT,0);
}


void gameIntro()
{
  setFadeMode(&main_fade,FADE_OUT,0);
  waitFadeDone();

  wait(5.f,PSP_CTRL_CROSS);

  setFadeMode(&main_fade,FADE_IN,0);
  waitFadeDone();

  game.state = GAME;

  setFadeMode(&main_fade,FADE_OUT,0);
  waitFadeDone();
}


void gameOutro()
{
  setFadeMode(&main_fade,FADE_OUT,0);
  waitFadeDone();

  wait(30.f,0);

  setFadeMode(&main_fade,FADE_IN,0);
  waitFadeDone();

  sceKernelExitGame();
}

// EOF
