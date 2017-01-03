#include "projet.h"

int main(int argc, char *argv[]){
	
	Elf32_Ehdr enTete;
	Elf32_Shdr *SectionTable;
	//Elf32_Shdr *NouvelleSectionTable;
	Elf32_Sym *SymbolTable = NULL;
	TableReimplantation* pointeur_reimplantation=NULL;
	int SectionNumber;
	FILE* elf;
	int SymbolTable_size = 0;

	//test pour avoir le bon nombre d'argument
	if(argc == 1){		
		printf("un argument est demandé \n");
		exit(0);
	}
	
	
	elf = fopen(argv[1], "r");
	
	//recupération en tête
	enTete = recupEnTete(elf);		
	affichageEnTete(enTete);

	printf("\n ---------------------------------------------------- \n");
	
	SectionNumber =  enTete.e_shnum;

	//on se place au bonne endrois du fichier	
	fseek(elf, enTete.e_shoff, SEEK_SET);		

	//allocation du tableau des sections
	SectionTable = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*(SectionNumber));	
	
	if (SectionTable==NULL){
		printf("problème d'allocation mémoire tableau des sections");
		exit (1);
	}

	recupTabsection(elf, enTete, SectionTable);
	affichageTabsection(SectionTable, enTete, elf);
	
	printf("\n ---------------------------------------------------- \n");
	
	affichageContenuSection(SectionTable, SectionNumber, elf);

	printf("\n ---------------------------------------------------- \n");

	SymbolTable_size = getTableauSymboles(elf, enTete, SectionTable, &SymbolTable);
	affichageSymbole(elf, enTete, SymbolTable, SectionTable, SymbolTable_size);

	printf("\n ---------------------------------------------------- \n");

	pointeur_reimplantation = recupTabReimplantation(elf, enTete, SectionTable);
	affichageTabReimplantation(SymbolTable, elf, enTete, SectionTable, pointeur_reimplantation, nb_tab_rel(enTete, SectionTable));
	
	printf("\n ---------------------------------------------------- \n");
	
	
	
	//printf("\n ====================================================== \n");
	//printf("\n 			PARTIE 2        		  \n");
	//printf("\n ====================================================== \n");
	
	////allocation nouvelle table de section
	//NouvelleSectionTable = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*(SectionNumber));		
	
	//if (NouvelleSectionTable==NULL){
		//printf("problème d'allocation mémoire nouvelle tableau des setions");
		//exit (1);
	//}
	
	//renumerotationSection(SectionTable, NouvelleSectionTable, enTete);
	//affichageTabsection(NouvelleSectionTable, enTete, elf);
	
	
	fclose(elf);
	//free(NouvelleSectionTable);
	free(SectionTable);
	free(SymbolTable);
	//free(pointeur_reimplantation);
	return 0;
}

