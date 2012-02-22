// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspkernel.h>
#include <pspdisplay.h>

#include "game.h"
#include "controls.h"

Robot robot;
Game game = {CREDITS,0,{{0}}};
UI ui = {0,0,0};


void wait(float seconds, unsigned int skip_btn)
{
  if (!skip_btn) sceKernelDelayThread(seconds * 1000000);
  else
  {
    int i;
    for (i=0; i!=seconds*1000; i++)
    {
      updateControls();
      if (testButton(skip_btn,0)) break;
      sceKernelDelayThread(1000);
    }
  }
}


int gameThread(SceSize args, void *argp)
{
  while (1)
  {
    updateControls();

    switch (game.state)
    {
      case GAME:
      case GAME_FUNC_SETTING:
        gameInGame();
      break;
      case CREDITS:
        gameCredits();
      break;
      case SPLASH:
        gameSplash();
      break;
      case GAME_INTRO:
        gameIntro();
      break;
      case OUTRO:
        gameOutro();
      break;
    }

    sceDisplayWaitVblankStart();
  }
  return 0;
}


void initGame()
{
  initInterpreter();

  // Start game thread
  SceUID thid = sceKernelCreateThread("game_thread",gameThread,0x11,0xFA0,0,0);
  sceKernelStartThread(thid,0,0);
}

// EOF
