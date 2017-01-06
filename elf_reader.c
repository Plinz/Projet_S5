#include "elf_reader.h"

int main (int argc, char *argv[]) 
{
FILE *fich; /* Fichier ELF qu'on va lire */
Elf32_Ehdr head; /* Header ELF de taille Fixe */
char o;
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
		  printf("\n");
		  scanf("%s", &o);
		  printf("\n");
} 

else
	o = *argv[2];

/* Lecture de l'header ELF au début du fichier */
head = lectureheader(fich);

//Déclaration pour l'affichage d'une section par son nom
Elf32_Shdr * TableSec = malloc(sizeof(Elf32_Shdr)*head.e_shnum);
fseek(fich, head.e_shoff, SEEK_SET);
lectureTableSection(fich, head,TableSec);
char * secName = malloc(50);

//Déclaration pour l'affichage d'une section par son numero
int secNum;

//Déclaration de la table des symboles
Elf32_Sym *tabSymb = malloc(head.e_shentsize);	
	int nbSym;
	
//Choix de l'affichage
switch(o){
	case 'h' :
		//Etape 1

		/* Affichage de l'header de l'ELF */
		affichageheader(head);
		//test test
		break;
	
	case 't' :
		//Etape 2
		//Affichage de la table des sections
		affichageTabsection(TableSec, head, fich);
		break;
	
	case 'n' :
		//Etape 3
		//Affichage du contenu d'une section dont le nom est donné en entrée
		printf("\nNom de la section à afficher\n");
		scanf("%s",secName);
		printf("\n");
		afficheSectionByName(head, TableSec, fich, secName);
		break;
	case 'N' :
		//Affichage du contenu d'une section dont le numéro est donné en entrée
		printf("\nNumero de la section à afficher\n");
		scanf("%d",&secNum);
		printf("\n");
		afficheSectionByNum(head, TableSec,  fich, secNum);
		break;

	case 's' :
		//Etape 4
		//Affichage de la table des symboles
		printf("===============================================================================================================\n");
		Elf32_Sym *tabSymbole = malloc(head.e_shentsize);
		int nbSymbole = lectureTableSymbole(tabSymbole, head, TableSec, fich);
		affichageTableSymbole(tabSymbole, nbSymbole, fich, TableSec, head);
		Elf32_Sym *tabSymbDynamique = malloc(head.e_shentsize);
		int nbSymboleDynamique = lectureTableSymboleDynamique(tabSymbDynamique, head, TableSec, fich);
		if(nbSymboleDynamique > 0){
			affichageTableSymboleDynamique(tabSymbDynamique, nbSymboleDynamique, fich, TableSec, head);
		}
		
		printf("===============================================================================================================\n");
		
		break;
	
	case 'r' :
		//Etape 5
		
		nbSym = lectureTableSymbole(tabSymb, head, TableSec, fich);
		affichage_relocation(tabSymb, &head, TableSec, nbSym, fich);
		break;
	
	default :
		  printf("\nRelancez le programme en veillant à choisir une option existante: \n");
  		  printf("h : affichage du ELF header.\n");
		  printf("t: affichage de la table des sections.\n");
		  printf("n: affichage du contenu d'une section via son nom.\n");
		  printf("N: affichage du contenu d'une section via son numéro.\n");
		  printf("s: affichage de la table des symboles.\n");
		  printf("r : affichage des tables de réimplantation.\n");
		  break;
};
printf("\n");
}