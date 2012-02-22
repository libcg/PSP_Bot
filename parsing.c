// 
// PSP_Bot by Geecko
//
// This work is licensed under the Creative Commons BY-NC-SA 3.0 Unported License.
// See LICENSE for more details.

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "parsing.h"

#define MAX_CHAR 512
#define NAME_FORMAT ":%s="
#define VALUE_FORMAT "%[^;];"

char *data = NULL, *fpath = NULL;
FILE* file = NULL;

void _initData()
{
  data = realloc(data,sizeof(char));
  *data = '\0';
}


short loadFile(char* path)
{
  _initData();

  file = fopen(path,"r");
  if (file == NULL) return -1;
  fpath = malloc(strlen(path)+1);
  strcpy(fpath,path);
  
  do
  {
    data = realloc(data,strlen(data)+MAX_CHAR);
    if (data == NULL) break;
  }
  while (fgets(data+strlen(data),MAX_CHAR,file) != NULL);
  data = realloc(data,strlen(data)+1);

  fclose(file);
  return 0;
}


void closeFile()
{
  _initData();
  free(fpath);
  fpath = NULL;
}


void readFromFile(char* name, char* dest)
{
  if (fpath == NULL) return;
  if (name == NULL || dest == NULL) return;
  
  char buffer[MAX_CHAR];
  sprintf(buffer,NAME_FORMAT,name);
  char* p = strstr(data,buffer);
  *dest = '\0';
  if (p == NULL) return;
  p += strlen(buffer);
  sscanf(p,VALUE_FORMAT,dest);
}
