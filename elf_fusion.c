#include "elf_reader.h"

int main (int argc, char *argv[])
{
	/* Verification du nombre d'arguments */
	if (argc != 4){
	  printf("Erreur : ./prog [fichier1 .o] [fichier2 .o] [fichier destination]\n");
	  return 0;
	}

	//Declaration pour la fusion de fichier
	FichierElf fichierElf1;
	FichierElf fichierElf2;
	FichierElf fichierElfRes;

	fichierElf1.fichierElf = fopen(argv[1],"r");
	if (fichierElf1.fichierElf == NULL) {
		printf("Erreur lors de l'ouverture du fichier1\n");
		exit(1);
	}
	fichierElf2.fichierElf = fopen(argv[2],"r");
	if (fichierElf2.fichierElf == NULL) {
		printf("Erreur lors de l'ouverture du fichier2\n");
		exit(1);
	}
	fichierElfRes.fichierElf = fopen(argv[3],"w+");
	if (fichierElfRes.fichierElf == NULL) {
		printf("Erreur lors de l'ouverture du fichierDest\n");
		exit(1);
	}

	initFichierELF(fichierElf1);
	initFichierELF(fichierElf2);

	//Initialisation de la structure resultat
	fichierElfRes.sectionsTable = malloc((fichierElf1.header_elf.e_shnum + fichierElf2.header_elf.e_shnum)*sizeof(Elf32_Shdr));
	fichierElfRes.tabSymbole = malloc(fichierElf1.header_elf.e_shentsize + fichierElf2.header_elf.e_shentsize);

	//Fusion
	fusion(&fichierElf1,&fichierElf2,&fichierElfRes);

	//Liberation de la memoire
	freeMemory(fichierElf1);
	freeMemory(fichierElf2);
	freeMemory(fichierElfRes);

	return 0;
}

//Fonction d'initialisation des fichiers sources ELF
void initFichierELF(FichierElf fichierElf){
	//header
	fichierElf.header_elf = lectureheader(fichierElf.fichierElf);
	//Section headers
	fichierElf.sectionsTable = malloc(sizeof(Elf32_Shdr)*fichierElf.header_elf.e_shnum);
	fseek(fichierElf.fichierElf, fichierElf.header_elf.e_shoff, SEEK_SET);
	lectureTableSection(fichierElf.fichierElf, fichierElf.header_elf,fichierElf.sectionsTable);
	//symboles
	Elf32_Shdr symboleHeader = getSectionByName(fichierElf.header_elf, fichierElf.sectionsTable, fichierElf.fichierElf, ".symtab");
	fichierElf.tabSymbole = malloc(sizeof(Symbole));
	lectureTableSymbole(fichierElf.tabSymbole, fichierElf.header_elf, fichierElf.sectionsTable, fichierElf.fichierElf);
}

//Fonction de liberation de la memoire
void freeMemory(FichierElf fichierElf){
	free(fichierElf.sectionsTable);
	free(fichierElf.tabSymbole);
	//free(fichierElf.tabSymboleDynamique);
	fclose(fichierElf.fichierElf);
}
