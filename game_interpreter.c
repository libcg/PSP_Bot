// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspkernel.h>

#include "game.h"
#include "level.h"
#include "common.h"

Interpreter intrp = {{{0}},0,{0},0,0};

// Robot Functions

void forward()
{
  robotDelay(0.4f);

  int tx = robot.mx + (robot.dir==E) - (robot.dir == W),
      ty = robot.my + (robot.dir==S) - (robot.dir == N),
      th = SQU_ROBOT.h - lvl.map[tx][ty].h;
  if (th == 0) robot.mx = tx, robot.my = ty;

  // Collide ?
  if (robot.mx < 0) robot.mx = 0;
  else if (robot.mx >= LEVEL_SIDE) robot.mx = LEVEL_SIDE - 1;
  if (robot.my < 0) robot.my = 0;
  else if (robot.my >= LEVEL_SIDE) robot.my = LEVEL_SIDE - 1;
}


void turn_left()
{
  robotDelay(0.1f);

  if ((--robot.dir) < N) robot.dir = W;
}


void turn_right()
{
  robotDelay(0.1f);

  if ((++robot.dir) > W) robot.dir = N;
}


void jump()
{
  robotDelay(0.5f);

  int tx = robot.mx + (robot.dir==E) - (robot.dir == W),
      ty = robot.my + (robot.dir==S) - (robot.dir == N),
      th = lvl.map[tx][ty].h - SQU_ROBOT.h;
  if (th == 1 || th < 0) robot.mx = tx, robot.my = ty;

  // Collide ?
  if (robot.mx < 0) robot.mx = 0;
  else if (robot.mx >= LEVEL_SIDE) robot.mx = LEVEL_SIDE - 1;
  if (robot.my < 0) robot.my = 0;
  else if (robot.my >= LEVEL_SIDE) robot.my = LEVEL_SIDE - 1;
}


void switch_light()
{
  robotDelay(0.1f);
  robot.flying = 0;
  SQU_ROBOT.state += (SQU_ROBOT.state == 1) - (SQU_ROBOT.state == 2);
  robotDelay(0.2f);
  robot.flying = 1;
}


void f1()
{
  STACK_H.instruction++;
  intrp.stack_h++;
  STACK_H.f_id = 1;
  STACK_H.instruction = -1; // Will be incremented in the loop
}


void f2()
{
  STACK_H.instruction++;
  intrp.stack_h++;
  STACK_H.f_id = 2;
  STACK_H.instruction = -1;
}


// Regular functions


void robotDelay(float t)
{
  sceKernelDelayThread(t*1000000);
}


//* Convert the instruction matrix to an array (in terms of the active function)
//* And return current instruction.
int getInstruction()
{
  int f_id = STACK_H.f_id, // 0 = main, 1 = f1, 2 = f2
      f_w = (f_id > 0) ? F_FUNC_W : F_MAIN_W,
      f_x = F_MAIN_W * (f_id >= 1) + F_FUNC_W * (f_id == 2),
      n = STACK_H.instruction;

  // Error : out of function (abort)
  if (n < 0 || n >= f_w * MATRIX_H) return -1;
  // Return the instruction id
  return game.func_matrix[f_x + n % f_w][n / f_w];
}


int intrpThread(SceSize args, void *argp)
{
  game.state = GAME;
  initRobot();
  robot.flying = 1;
  robotDelay(0.5f);

  intrp.stack_h = 0;
  STACK_H.instruction = 0;

  int i;

  while (intrp.stack_h >= 0)
  {
    i = getInstruction();
    if (i < 0) // Abort current function
    {
      intrp.stack_h--;
      continue;
    }
    if (i==0) // Blank instruction
    {
      STACK_H.instruction++;
      continue;
    }

    intrp.f[i-1]();
    if (intrp.stack_h >= STACK_SIZE) intrp.stack_h = -1; // Stack overflow

    STACK_H.instruction++;
  }

  robotDelay(0.5f);
  if (getFadeState(main_fade) != FADING) stopProgram();

  intrp.running = 0;
  return 0;
}


void stopProgram()
{
  if (!intrp.running) return;
  sceKernelTerminateThread(intrp.thid);
  intrp.running = 0;
  resetGame();
}


void runProgram()
{
  stopProgram();
  sceKernelStartThread(intrp.thid,0,0);
  intrp.running = 1;
}


void initInterpreter()
{
  intrp.thid = sceKernelCreateThread("intrp_thread",intrpThread,0x11,0xFA0,0,0);

  intrp.f[0] = forward;
  intrp.f[1] = turn_left;
  intrp.f[2] = turn_right;
  intrp.f[3] = jump;
  intrp.f[4] = switch_light;
  intrp.f[5] = f1;
  intrp.f[6] = f2;
}
