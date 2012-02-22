// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <pspctrl.h>

SceCtrlData pad, oldPad;


short testButton(unsigned int btn, short test)
{
  short tmp=0;
  if (test==0) tmp =  (pad.Buttons & btn);
  if (test==1) tmp =  (pad.Buttons & btn) && !(oldPad.Buttons & btn);
  if (test==2) tmp = !(pad.Buttons & btn) &&  (oldPad.Buttons & btn);
  if (test==3) tmp =  (oldPad.Buttons & btn); 
  return tmp;
}


void updateControls()
{
  oldPad = pad;
  sceCtrlPeekBufferPositive(&pad,1);
}
