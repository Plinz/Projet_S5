#include "elf_reader.h"
#include "elf_fusion.h"

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

	//Init fichier1
		fichierElf1.fichierElf = fopen(argv[1],"r");
		if (fichierElf1.fichierElf == NULL) {
			printf("Erreur lors de l'ouverture du fichier1\n");
			exit(1);
		}
		//header
		fichierElf1.header_elf = lectureheader(fichierElf1.fichierElf);
		//Section headers
		fichierElf1.sectionsTable = malloc(sizeof(Elf32_Shdr)*fichierElf1.header_elf.e_shnum);
		fseek(fichierElf1.fichierElf, fichierElf1.header_elf.e_shoff, SEEK_SET);
		lectureTableSection(fichierElf1.fichierElf, fichierElf1.header_elf,fichierElf1.sectionsTable);
		//symboles
		Elf32_Shdr header; int present;
		RechercheSectionByName(&fichierElf1, ".symtab", &header, &present);
		int sizeTableSymbole = header.sh_size/header.sh_entsize;
		fichierElf1.tabSymbole = malloc(sizeTableSymbole*sizeof(Symbole));
		lectureTableSymbole(fichierElf1.tabSymbole, fichierElf1.header_elf, fichierElf1.sectionsTable, fichierElf1.fichierElf);

	//Init fichier2
		fichierElf2.fichierElf = fopen(argv[2],"r");
		if (fichierElf2.fichierElf == NULL) {
			printf("Erreur lors de l'ouverture du fichier2\n");
			exit(1);
		}
		//header
		fichierElf2.header_elf = lectureheader(fichierElf2.fichierElf);
		//Section headers
		fichierElf2.sectionsTable = malloc(sizeof(Elf32_Shdr)*fichierElf2.header_elf.e_shnum);
		fseek(fichierElf2.fichierElf, fichierElf2.header_elf.e_shoff, SEEK_SET);
		lectureTableSection(fichierElf2.fichierElf, fichierElf2.header_elf,fichierElf2.sectionsTable);
		//symboles
		RechercheSectionByName(&fichierElf2, ".symtab", &header, &present);
		sizeTableSymbole = header.sh_size/header.sh_entsize;
		fichierElf2.tabSymbole = malloc(sizeTableSymbole*sizeof(Symbole));
		lectureTableSymbole(fichierElf2.tabSymbole, fichierElf2.header_elf, fichierElf2.sectionsTable, fichierElf2.fichierElf);

		//Init fichierRes
		fichierElfRes.fichierElf = fopen(argv[3],"w+");
		if (fichierElfRes.fichierElf == NULL) {
			printf("Erreur lors de l'ouverture du fichierDest\n");
			exit(1);
		}
		//Section headers
		fichierElfRes.sectionsTable = malloc((fichierElf1.header_elf.e_shnum + fichierElf2.header_elf.e_shnum)*sizeof(Elf32_Shdr));
		//symboles
		fichierElfRes.tabSymbole = malloc(fichierElf1.header_elf.e_shentsize + fichierElf2.header_elf.e_shentsize);


		fusion(&fichierElf1,&fichierElf2,&fichierElfRes);


		printf("Phase de liberation de la memoire :\n");
		printf("FichierElf1 : ");
		free(fichierElf1.sectionsTable);
		free(fichierElf1.tabSymbole); //Pas encore alloué
		//free(fichierElf1.tabSymboleDynamique); //Pas encore alloué
		fclose(fichierElf1.fichierElf);
		printf("OK\n");
		printf("FichierElf2 : ");
		free(fichierElf2.sectionsTable);
		free(fichierElf2.tabSymbole);
		//free(fichierElf2.tabSymboleDynamique); //Pas encore alloué
		fclose(fichierElf2.fichierElf);
		printf("OK\n");
		printf("FichierElfRes : ");
		free(fichierElfRes.sectionsTable);
		free(fichierElfRes.tabSymbole);
		//free(fichierElfRes.tabSymboleDynamique); //Pas encore alloué
		fclose(fichierElfRes.fichierElf);
		printf("OK\n");
		return 0;
}
