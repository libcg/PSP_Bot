// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "common.h"

#define ENCRYPTED_CREDITS "iENO\nHS\nmOOIAE  kX^\nHS\niKZ^KCDaCFF\nmOOIAE\nKDN\naK\\OF\ncDER   eXCMCDKF\nIEDIOZ^\nHS\ngK^^\niBKYO\nhCFF\n^BO\nxEHE^\nMKGO  FCMB^\7hE^\nLFKYB\nMKGO\nGKNO\nHS\niEEFCE\7dCK^E\nnKDDS\nsKXEYFK\\YAC  cNOK\nEL\n^BO\nzyz\nZEX^\nHS\nsEDKHK\nyOKDzK_F\nKDN\ng\7iCN   YCDM\nCD^XKlED^\nm\7YZOI\nHS\nhODb_X\n"
#define ENCRYPTED_CREDITS_LEN 268

#define ENCRYPTED_INGAME_TITLE "zyzuhE^\nHS\nmOOIAE"
#define ENCRYPTED_INGAME_TITLE_LEN 17

typedef struct
{
  char list[FUNC_NBR][32];
  char ctrl[64];
  char f[3][32];
} Text_Game_Func_Setting;

typedef struct
{
  Text_Game_Func_Setting func;
  char intro_title[64];
  char intro[1024];
  char title[32];
  char setup_robot[64];
  char stop_robot[64];
  char instructions[64];
} Text_Game;

typedef struct
{
  Text_Game game;
  char credits[1024];
  char outro[1024];
} Text;

extern Text text;


void initLanguage();

#endif
