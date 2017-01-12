#include "elf_reader.h"
#include "elf_fusion.h"

int main (int argc, char *argv[]) 
{
	FichierElf fichierElf;
	  
	/* Verification du nombre d'arguments */
	if (argc<3){
		printf("Erreur : ./reader [options] [fichier .o]\n");
		printf("Relancez le programme en veillant à choisir une option existante: \n");
		printf("a : affichage de toute les options\n");				
		printf("h : affichage du ELF header.\n");
		printf("S: affichage de la table des sections.\n");
		printf("n: affichage du contenu d'une section via son nom.\n");
		printf("N: affichage du contenu d'une section via son numéro.\n");
		printf("s: affichage de la table des symboles.\n");
		printf("r : affichage des tables de réimplantation.\n");
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
	if ((fichierElf.fichierElf = fopen(argv[argc-1],"rb")) == NULL ){
	 printf("Erreur ouverture fichier\n"); 
	  return 0;
	}

	/* Lecture de l'header ELF au début du fichier */
	fichierElf.header_elf = lectureheader(fichierElf.fichierElf);

	//Déclaration pour l'affichage d'une section par son nom
	fseek(fichierElf.fichierElf, fichierElf.header_elf.e_shoff, SEEK_SET);
	fichierElf.sectionsTable = malloc(fichierElf.header_elf.e_shnum * sizeof(Elf32_Shdr));
	lectureTableSection(fichierElf.fichierElf, fichierElf.header_elf,fichierElf.sectionsTable);

	Elf32_Shdr recherche;
	
	//Choix de l'affichage
	if (fileHeader){
		//Etape 1
		/* Affichage de l'header de l'ELF */
		affichageheader(fichierElf.header_elf);
		printf("\n");
	}
	if (sections){
		//Etape 2
		//Affichage de la table des sections
		affichageTabsection(fichierElf.sectionsTable, fichierElf.header_elf, fichierElf.fichierElf);
		printf("\n");
	}
	if (sectionName != NULL){
		//Etape 3
		//Affichage du contenu d'une section dont le nom est donné en entrée
		recherche = getSectionByName(fichierElf.header_elf, fichierElf.sectionsTable, fichierElf.fichierElf, sectionName);
		afficheContenue(recherche, fichierElf.fichierElf);
		printf("\n");
	}
	if (sectionNumber > -1){
		//Etape 3
		//Affichage du contenu d'une section dont le numéro est donné en entrée
		recherche = getSectionByIndex(fichierElf.header_elf, fichierElf.sectionsTable, sectionNumber);
		afficheContenue(recherche, fichierElf.fichierElf);
		printf("\n");
	}
	if (symbols){
		//Etape 4
		//Affichage de la table des symboles
		printf("====================================================================================\n");
		
		Elf32_Shdr header; int present;
		RechercheSectionByName(&fichierElf, ".symtab", &header, &present);
		int sizeTableSymbole = header.sh_size/header.sh_entsize;
		fichierElf.tabSymbole = malloc(sizeTableSymbole*sizeof(Symbole));
		int nbSymbole = lectureTableSymbole(fichierElf.tabSymbole, fichierElf.header_elf, fichierElf.sectionsTable, fichierElf.fichierElf);
		affichageTableSymbole(fichierElf.tabSymbole, nbSymbole, fichierElf.fichierElf, fichierElf.sectionsTable, fichierElf.header_elf);
		/*Elf32_Sym *tabSymbDynamique = malloc(head.e_shentsize);
		int nbSymboleDynamique = lectureTableSymboleDynamique(tabSymbDynamique, head, TableSec, fich);
		if(nbSymboleDynamique > 0){
			affichageTableSymboleDynamique(tabSymbDynamique, nbSymboleDynamique, fich, TableSec, head);
		}*/

		free(fichierElf.tabSymbole);

		printf("====================================================================================\n");
		printf("\n");
	}
	if (relocs){
		//Etape 5
		//Affichage des tables de réimplantation
		Elf32_Shdr header; int present;
		RechercheSectionByName(&fichierElf, ".symtab", &header, &present);
		int sizeTableSymbole = header.sh_size/header.sh_entsize;
		fichierElf.tabSymbole = malloc(sizeTableSymbole*sizeof(Symbole));
		affichage_relocation(&fichierElf.header_elf, fichierElf.sectionsTable, fichierElf.fichierElf);
		printf("\n");

		free(fichierElf.tabSymbole);
	}

	free(fichierElf.sectionsTable);
	printf("\n");
}
