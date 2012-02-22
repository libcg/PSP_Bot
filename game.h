// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "pspthreadman.h"
#include "disp.h"

#define STACK_SIZE 32

#define STACK_H intrp.stack[intrp.stack_h]

enum game_state { SPLASH, CREDITS, GAME_INTRO, GAME, GAME_FUNC_SETTING, OUTRO };
enum robot_dir { N, E, S, W};

typedef struct
{
  short mx, my; // Position on the map
  short dir; // 0=North, 1=East, 2=South, 3=West
  short cycle; // For the "breathing" animation
  short flying; // Boolean
} Robot;

typedef struct
{
  int func_selected;
  int matrix_x;
  int matrix_y;
} UI;

typedef struct
{
  int state;
  int instructions_count;
  short func_matrix[MATRIX_W][MATRIX_H];
} Game;

typedef struct
{
  short f_id; // 0 = main, 1 = f1, 2 = f2
  short instruction;
} Call;

typedef struct
{
  Call stack[STACK_SIZE]; // Call stack
  short stack_h; // Call stack height
  void (*f[FUNC_NBR])(); // An array of pointers to the robot functions
  SceUID thid; // To control the interpreter
  short running;
} Interpreter;

extern Robot robot;
extern Game game;
extern UI ui;
extern Interpreter intrp;
extern Fade reset_fade;

// Common
void wait(float seconds, unsigned int skip_btn);
void initGame();

// Game
void initRobot();
void controlRobot();
void isLevelDone();
void resetGame();
void gameInGame();
void gameCredits();
void gameSplash();
void gameIntro();
void gameOutro();

// Interpreter
void robotDelay(float t);
void initInterpreter();
void stopProgram();
void runProgram();

#endif
