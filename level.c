// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <stdio.h>

#include "parsing.h"
#include "language.h"
#include "common.h"
#include "level.h"
#include "game.h"

Level lvl;
char buf[2][1024];


void loadLevel()
{
  int i, j, tmp;
  sprintf(buf[0],"levels/%d.hlf",lvl.nbr);
  loadFile(buf[0]);

  // Get the level introduction text
  readFromFile("fr",buf[0]);
  sscanf(buf[0],"\"%[^\"]\"",text.game.intro);

  // Set function highlighting
  readFromFile("highlight",buf[0]);
  for (i=0; i!=FUNC_NBR; i++) lvl.highlight[i] = 0; // Reset
  for (i=0; i!=strlen(buf[0]); i++) lvl.highlight[buf[0][i]-'0'] = 1;

  // Robot properties
  readFromFile("start_x",buf[0]);
  sscanf(buf[0],"%hd",&lvl.x);
  readFromFile("start_y",buf[0]);
  sscanf(buf[0],"%hd",&lvl.y);
  readFromFile("start_dir",buf[0]);
  sscanf(buf[0],"%hd",&lvl.dir);
  initRobot();

  // Heightmap properties
  for (j=0; j!=LEVEL_SIDE; j++)
  {
    sprintf(buf[1],"h%d",j);
    readFromFile(buf[1],buf[0]);
    for (i=0; i!=LEVEL_SIDE; i++)
    {
      tmp = *(buf[0]+i) - '0';
      if (tmp >= 0) lvl.map[i][j].h = tmp;
      else lvl.map[i][j].h = 0;
    }
  }

  // Square types
  for (j=0; j!=LEVEL_SIDE; j++)
  {
    sprintf(buf[1],"t%d",j);
    readFromFile(buf[1],buf[0]);
    for (i=0; i!=LEVEL_SIDE; i++)
    {
      tmp = *(buf[0]+i) - '0';
      if (tmp >= 0 && tmp <= 1) lvl.map[i][j].state = tmp;
      else lvl.map[i][j].state = 0;
    }
  }

  closeFile();
}


void resetLevel()
{
  int i, j;
  for (j=0; j!=LEVEL_SIDE; j++)
    for (i=0; i!=LEVEL_SIDE; i++)
      if (lvl.map[i][j].state == 2) lvl.map[i][j].state = 1;
}


short nextLevel()
{
  if (++lvl.nbr > lvl.nol) return -1;

  loadLevel();
  return 0;
}


void initLevel()
{
  // Define Number of Levels
  int i = 0;
  do {
    sprintf(buf[0],"levels/%d.hlf",++i);
  } while (!loadFile(buf[0]));
  lvl.nol = i-1;

  // Load the first one
  lvl.nbr = 1;
  loadLevel();
}

// EOF
