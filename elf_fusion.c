#include "elf_reader.h"

int main (int argc, char *argv[]) 
{
	/* Verification du nombre d'arguments */
	if (argc != 4){
	  printf("Erreur : ./prog [fichier1 .o] [fichier2 .o] [fichier destination]");
	  return 0;
	}

	//Declaration pour la fusion de fichier
	FichierElf fichierElf1;
	FichierElf fichierElf2;

	//Init fichier1
	printf("0\n");
	fichierElf1.fichierElf = fopen(argv[1],"r");
	if (fichierElf1.fichierElf == NULL) {
		printf("Erreur lors de l'ouverture du fichier1\n");
		exit(1);
	}
	fichierElf1.header_elf = lectureheader(fichierElf1.fichierElf);
	fichierElf1.sectionsTable = malloc(sizeof(Elf32_Shdr)*fichierElf1.header_elf.e_shnum);
	fseek(fichierElf1.fichierElf, fichierElf1.header_elf.e_shoff, SEEK_SET);
	lectureTableSection(fichierElf1.fichierElf, fichierElf1.header_elf,fichierElf1.sectionsTable);

	printf("Décalage de %d pour la  table\n",fichierElf1.sectionsTable[fichierElf1.header_elf.e_shstrndx].sh_offset);


	printf("1\n");
	//Init fichier1
	fichierElf2.fichierElf = fopen(argv[2],"r");
	if (fichierElf2.fichierElf == NULL) {
		printf("Erreur lors de l'ouverture du fichier2\n");
		exit(1);
	}
	fichierElf2.header_elf = lectureheader(fichierElf2.fichierElf);
	fichierElf2.sectionsTable = malloc(sizeof(Elf32_Shdr)*fichierElf2.header_elf.e_shnum);
	fseek(fichierElf2.fichierElf, fichierElf2.header_elf.e_shoff, SEEK_SET);
	lectureTableSection(fichierElf2.fichierElf, fichierElf2.header_elf,fichierElf2.sectionsTable);

	printf("2\n");
	FILE *fichierDest = fopen(argv[3],"w+");
	if (fichierDest == NULL) {
		printf("Erreur lors de l'ouverture du fichierDest\n");
		exit(1);
	}

	printf("3\n");
	fusion(&fichierElf1,&fichierElf2,fichierDest);


	fclose(fichierElf1.fichierElf);
	fclose(fichierElf2.fichierElf);
	fclose(fichierDest);
	//Les free ???
}
