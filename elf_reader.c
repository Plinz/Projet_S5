#include "elf_reader.h"

int main (int argc, char *argv[]) 
{
FILE *fich; /* Fichier ELF qu'on va lire */
Elf32_Ehdr head; /* Header ELF de taille Fixe */
  
  /*Elf Section Header*/
  
  /* Verification du nombre d'arguments */
if (argc!=2){
  printf("Erreur Arguments");
  return 0;
}

  /* ouverture du fichier */
if ((fich = fopen(argv[1],"rb")) == NULL ){
 printf("Erreur ouverture fichier"); 
  return 0;
}

/* Lecture de l'header ELF au début du fichier */
head = lectureheader(fich);

/* Affichage de l'header de l'ELF */
affichageheader(head);
//test test

//Test Etape 2
Elf32_Shdr * t = malloc(sizeof(Elf32_Shdr)*head.e_shnum);
fseek(fich, head.e_shoff, SEEK_SET);

lectureTableSection(fich, head,t);

affichageTabsection(t, head, fich);

//Test Etape 3
char * secName = malloc(50);
printf("\nNom de la section à afficher\n");
scanf("%s",secName);
printf("\n");
afficheSectionByName(head, t, fich, secName);


}
