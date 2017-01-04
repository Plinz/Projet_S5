#include "elf_reader.h"

int main (int argc, char *argv[]) 
{
FILE *fich; /* Fichier ELF qu'on va lire */
Elf32_Ehdr head; /* Header ELF de taille Fixe */
  
  /*Elf Section Header*/
  
  /* Verification du nombre d'arguments */
if (argc<2){
  printf("Erreur Arguments");
  return 0;
}

  /* ouverture du fichier */
if ((fich = fopen(argv[1],"rb")) == NULL ){
 printf("Erreur ouverture fichier"); 
  return 0;
}

if(argc!=3){
		  printf("\nVeuillez choisir une option parmi les suivantes: \n");
  		  printf("h : affichage du ELF header.\n");
		  printf("t: affichage de la table des sections.\n");
		  printf("n: affichage du contenu d'une section via son nom.\n");
		  printf("N: affichage du contenu d'une section via son numéro.\n");
		  printf("s: affichage de la table des symboles.\n");
		  printf("r : affichage des tables de réimplantation.\n");
} 

else{
char o = *argv[2];

/* Lecture de l'header ELF au début du fichier */
head = lectureheader(fich);

//Déclaration pour l'affichage d'une section par son nom
Elf32_Shdr * TableSec = malloc(sizeof(Elf32_Shdr)*head.e_shnum);
fseek(fich, head.e_shoff, SEEK_SET);
lectureTableSection(fich, head,TableSec);
char * secName = malloc(50);

//Déclaration pour l'affichage d'une section par son numero
int secNum;

//Choix de l'affichage
switch(o){
	case 'h' :
		//Test Etape 1

		/* Affichage de l'header de l'ELF */
		affichageheader(head);
		//test test
		break;
	
	case 't' :
		//Test Etape 2

		affichageTabsection(TableSec, head, fich);
		break;
	
	case 'n' :
		//Test Etape 3
		printf("\nNom de la section à afficher\n");
		scanf("%s",secName);
		printf("\n");
		afficheSectionByName(head, TableSec, fich, secName);
		break;
	case 'N' :
		printf("\nNumero de la section à afficher\n");
		scanf("%d",&secNum);
		printf("\n");
		afficheSectionByNum(head, TableSec,  fich, secNum);
		break;

	case 's' :
		//Test Etape 4
		printf("====================================================");
		Elf32_Sym *tabSymb = malloc(head.e_shentsize);
		int nbSymbole = lectureTableSymbole(tabSymb, head, TableSec, fich);
		affichageTableSymbole(tabSymb, nbSymbole, fich, TableSec);
		
		break;
	
	case 'r' :
		printf("test");
		break;
	
	default :
		  printf("\nVeuillez choisir une option parmi les suivantes: \n");
  		  printf("h : affichage du ELF header.\n");
		  printf("t: affichage de la table des sections.\n");
		  printf("n: affichage du contenu d'une section via son nom.\n");
		  printf("N: affichage du contenu d'une section via son numéro.\n");
		  printf("s: affichage de la table des symboles.\n");
		  printf("r : affichage des tables de réimplantation.\n");
		  break;
};
}
}
