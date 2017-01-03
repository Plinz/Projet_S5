#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
File *fich; /* Fichier ELF qu'on va lire */
Elf32_Ehdr head; /* Header ELF de taille Fixe */
  
  /*Elf Section Header*/
  
  /* Verification du nombre d'arguments */
if (argc!=2){
  printf("Erreur Arguments");
  return 0;
}

  /* ouverture du fichier */
if ((fich = fopen("argv[1]","rb")) == NULL ){
 printf("Erreur ouverture fichier"); 
  return 0;
}

/* Lecture de l'header ELF au début du fichier */


//test test

}
