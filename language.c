// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <string.h>

#include "language.h"

Text text;


// Say hello to hex-editers
void decrypt(char* src, char* dst, int len)
{
  int i;
  for (i=0; i!=len; i++) dst[i] = src[i] ^ 42;
  dst[i] = '\0';
}


void initLanguage()
{
  // * Français *
  strcpy(text.game.intro_title ,"Niveau %d");

  strcpy(text.game.setup_robot ,"[] : Paramétrer le robot");
  strcpy(text.game.stop_robot  ,"X : Arrêter");
  strcpy(text.game.instructions,"%d commande(s)");

  strcpy(text.game.func.list[0],"Avancer");
  strcpy(text.game.func.list[1],"Tourner à gauche");
  strcpy(text.game.func.list[2],"Tourner à droite");
  strcpy(text.game.func.list[3],"Sauter");
  strcpy(text.game.func.list[4],"Allumer la case");
  strcpy(text.game.func.list[5],"Fonction 1");
  strcpy(text.game.func.list[6],"Fonction 2");

  strcpy(text.game.func.f[0]   ,"Fonction Principale");
  strcpy(text.game.func.f[1]   ,"Fonction 1");
  strcpy(text.game.func.f[2]   ,"Fonction 2");

  strcpy(text.game.func.ctrl   ,"Start : Démarrer      Select : Réinitialiser");

  strcpy(text.outro            ,"Vous avez fini le jeu !\n\nNous espérons que vous avez aimé utiliser\nvotre logique pour résoudre ces problèmes.\n\nSi oui, vous pouvez envisager d'apprendre\nun véritable langage de programmation,\ncomme le C, Lua, Ruby ou Python.\n\n\nScore : %d commandes.");

  /* English *
  strcpy(text.game.intro_title ,"Level %d");

  strcpy(text.game.setup_robot ,"Press square to setup the bot");
  strcpy(text.game.stop_robot  ,"Press X to stop");
  strcpy(text.game.instructions,"%d command(s)");

  strcpy(text.game.func.list[0],"Forward");
  strcpy(text.game.func.list[1],"Turn left");
  strcpy(text.game.func.list[2],"Turn right");
  strcpy(text.game.func.list[3],"Jump");
  strcpy(text.game.func.list[4],"Switch light");
  strcpy(text.game.func.list[5],"Function 1");
  strcpy(text.game.func.list[6],"Function 2");

  strcpy(text.game.func.f[0]   ,"Main method");
  strcpy(text.game.func.f[1]   ,"Function 1");
  strcpy(text.game.func.f[2]   ,"Function 2");

  strcpy(text.game.func.ctrl   ,"Start to launch      Select to reinit");

  strcpy(text.outro            ,"You finished the game !\n\nWe hope you enjoyed using your logic to solve these problems.\n\nIf so, you might consider learning\na real programming language,\nlike C, Lua, Ruby or Python.\n\n\nScore : %d commands.");
  */

  char clear[1024];
  decrypt(ENCRYPTED_INGAME_TITLE,clear,ENCRYPTED_INGAME_TITLE_LEN);
  strcpy(text.game.title,clear);
  decrypt(ENCRYPTED_CREDITS,clear,ENCRYPTED_CREDITS_LEN);
  strcpy(text.credits,clear);
}
  

// EOF
