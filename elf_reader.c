#include <elf.h>
#include <stdio.h>

int main (int argc, char *argv[]) 
{
File *fich; 
  /*Elf Header
  #Elf Section Header*/
if (argc!=2){
  printf("Erreur Arguments");
  return 0;
}

if ((fich = fopen("argv[1]","r")) == NULL ){
 printf("Erreur ouverture fichier"); 
  return 0;
}






}
