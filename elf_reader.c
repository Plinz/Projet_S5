#include "elf_reader.h"

int main (int argc, char *argv[]) 
{
	FILE *fich; /* Fichier ELF qu'on va lire */
	Elf32_Ehdr head; /* Header ELF de taille Fixe */
	  
	/* Verification du nombre d'arguments */
	if (argc<2){
	  printf("Erreur : ./prog [options] [fichier .o]");
	  return 0;
	}

	static struct option long_options[] =
	{
		{"all", no_argument, 0, 'a'},
		{"file-header", no_argument, 0, 'h'},
		{"sections", no_argument, 0, 'S'},
		{"section-name", required_argument, 0, 'n'},
		{"section-number", required_argument, 0, 'N'},
		{"symbols", no_argument, 0, 's'},
		{"relocs", no_argument, 0, 'r'},
		{0, 0, 0, 0}
        };
	int c = 0;
	int option_index = 0;
	int fileHeader = 0, sections = 0, sectionNumber = -1, symbols = 0, relocs =  0;
	char* sectionName = NULL;
	while ((c = getopt_long (argc, argv, "ahSn:N:sr", long_options, &option_index)) != -1) {
		switch (c){
			case 'a' : fileHeader = 1; sections = 1; symbols = 1; relocs = 1;
				break;
			case 'h' : fileHeader = 1;
				break;
			case 'S' : sections = 1;
				break;
			case 's' : symbols = 1;
				break;
			case 'r' : relocs = 1;
				break;
			case 'n' : sectionName = optarg;
				break;
			case 'N' : sectionNumber = atoi(optarg);
				break;
			default :
				printf("\nRelancez le programme en veillant à choisir une option existante: \n");
				printf("a : affichage de toute les options\n");				
				printf("h : affichage du ELF header.\n");
				printf("S: affichage de la table des sections.\n");
				printf("n: affichage du contenu d'une section via son nom.\n");
				printf("N: affichage du contenu d'une section via son numéro.\n");
				printf("s: affichage de la table des symboles.\n");
				printf("r : affichage des tables de réimplantation.\n");
				exit(EXIT_FAILURE);
		}
	}

	/* Ouverture du fichier */
	if ((fich = fopen(argv[argc-1],"rb")) == NULL ){
	 printf("Erreur ouverture fichier"); 
	  return 0;
	}

	/* Lecture de l'header ELF au début du fichier */
	head = lectureheader(fich);

	//Déclaration pour l'affichage d'une section par son nom
	Elf32_Shdr * TableSec = malloc(sizeof(Elf32_Shdr)*head.e_shnum);
	fseek(fich, head.e_shoff, SEEK_SET);
	lectureTableSection(fich, head,TableSec);
	//char * secName = malloc(50);

	//Déclaration pour l'affichage d'une section par son numero
	//int secNum;

	//Déclaration de la table des symboles
	Elf32_Sym *tabSymb = malloc(head.e_shentsize);	
	int nbSym;
	
	//Choix de l'affichage
	if (fileHeader){
		//Etape 1
		/* Affichage de l'header de l'ELF */
		affichageheader(head);
		printf("\n");
	}
	if (sections){
		//Etape 2
		//Affichage de la table des sections
		affichageTabsection(TableSec, head, fich);
		printf("\n");
	}
	if (sectionName != NULL){
		//Etape 3
		//Affichage du contenu d'une section dont le nom est donné en entrée
		afficheSectionByName(head, TableSec, fich, sectionName);
		printf("\n");
	}
	if (sectionNumber > -1){
		//Etape 3
		//Affichage du contenu d'une section dont le numéro est donné en entrée
		afficheSectionByNum(head, TableSec,  fich, sectionNumber);
		printf("\n");
	}
	if (symbols){
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
		printf("\n");
	}
	if (relocs){
		//Etape 5
		//Affichage des tables de réimplantation
		nbSym = lectureTableSymbole(tabSymb, head, TableSec, fich);
		affichage_relocation(tabSymb, &head, TableSec, nbSym, fich);
		printf("\n");
	}
	printf("\n");
}
