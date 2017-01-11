#include "elf_reader.h"

void AjoutNomStrtab(char * nom, Strtab * strtab, Elf32_Sym * symb) {
	int i = 0;
	while ((strtab->names[strtab->nbNames][i] = nom[i]) != '\0') {
		i++;
	}
	symb->st_name = strtab->offsetCourant;
	//printf("\t\tAjoutStrtab position %d de %s avec offset de %x\n",strtab->nbNames,strtab->names[strtab->nbNames],strtab->offsetCourant);
	strtab->offsetCourant += strlen(strtab->names[strtab->nbNames]) + 1;
	strtab->nbNames++;
}

int fusionTableSymbole(FichierElf structFichier1, FichierElf structFichier2, int sizeTab1, int sizeTab2, Symbole *newTabSymbole, Strtab * strtab){
	int i;
	int j;
	char *c1;
	char *c2;
	c1 = malloc(50);
	c2 = malloc(50);
	FILE* f1 = structFichier1.fichierElf;
	FILE* f2 = structFichier2.fichierElf;
	
	int nbEntree = 0;
	int flag = 0;
	int offsetStringTable1 = rechercheOffsetSection(structFichier1.header_elf, structFichier1.sectionsTable, f1, ".strtab");
	int offsetStringTable2 = rechercheOffsetSection(structFichier2.header_elf, structFichier2.sectionsTable, f2, ".strtab");
	for(i=0; i<sizeTab1; i++){
		recupNomSymbole(structFichier1.tabSymbole[i].symbole.st_name, f1, offsetStringTable1, c1);
		if(ELF32_ST_BIND(structFichier1.tabSymbole[i].symbole.st_info) == 0){ //Symboles locaux de la première table
			newTabSymbole[nbEntree] = structFichier1.tabSymbole[i];
			AjoutNomStrtab(c1, strtab, &newTabSymbole[nbEntree].symbole);
			newTabSymbole[nbEntree].fichier = 1;
			nbEntree++;
		}else if(ELF32_ST_BIND(structFichier1.tabSymbole[i].symbole.st_info) == 1){ //symboles globaux de la première table
			for(j=0; j<sizeTab2; j++){
				recupNomSymbole(structFichier2.tabSymbole[j].symbole.st_name, f2, offsetStringTable2, c2);
				if(ELF32_ST_BIND(structFichier2.tabSymbole[j].symbole.st_info) == 1){ //symboles globaux de la seconde table
					if(strcmp(c1,c2) == 0){ // Les symboles ont le même nom
						flag = 1;
						if(structFichier1.tabSymbole[i].symbole.st_shndx != SHN_UNDEF && structFichier2.tabSymbole[j].symbole.st_shndx != SHN_UNDEF){ //Les symboles sont tous les deux définis
							printf("Echec de l'édition de lien, deux symboles ont un nom identique\n");
							exit(0);
						}else{
							if(structFichier1.tabSymbole[i].symbole.st_shndx != SHN_UNDEF && structFichier2.tabSymbole[j].symbole.st_shndx == SHN_UNDEF){ //Le symbole est défini dans le premier fichier pas dans le deuxième
								newTabSymbole[nbEntree] = structFichier1.tabSymbole[i];
								AjoutNomStrtab(c1, strtab, &newTabSymbole[nbEntree].symbole);
								newTabSymbole[nbEntree].fichier = 1;
								nbEntree++;
							}
							else if(structFichier1.tabSymbole[i].symbole.st_shndx == SHN_UNDEF && structFichier2.tabSymbole[j].symbole.st_shndx != SHN_UNDEF){ //Le symbole est défini dans le deuxième fichier pas dans le premier
								newTabSymbole[nbEntree] = structFichier2.tabSymbole[j];
								AjoutNomStrtab(c2, strtab, &newTabSymbole[nbEntree].symbole);	
								newTabSymbole[nbEntree].fichier = 2;			
								nbEntree++;
							}
							else if(structFichier1.tabSymbole[i].symbole.st_shndx == SHN_UNDEF && structFichier2.tabSymbole[j].symbole.st_shndx == SHN_UNDEF){ //Le symbole n'est défini dans aucune des deux tables
								newTabSymbole[nbEntree] = structFichier1.tabSymbole[i];
								AjoutNomStrtab(c1, strtab, &newTabSymbole[nbEntree].symbole);
								newTabSymbole[nbEntree].fichier = 1;
								nbEntree++;
							}
						}
					}
				}
			}
			if(!flag){ // Si le symbole n'apparait que dans la première table
				newTabSymbole[nbEntree]  = structFichier1.tabSymbole[i];
				AjoutNomStrtab(c1, strtab, &newTabSymbole[nbEntree].symbole);
				newTabSymbole[nbEntree].fichier = 1;
				nbEntree++;
			}			
		}
		flag = 0;
	}
	
	for(i=0; i<sizeTab2; i++){// boucle pour vérifier si un symbole n'apparait que dans la deuxième table
		recupNomSymbole(structFichier2.tabSymbole[i].symbole.st_name, f2, offsetStringTable2, c2);
		if(ELF32_ST_BIND(structFichier2.tabSymbole[i].symbole.st_info) == 0){ //Symboles locaux de la seconde table
			newTabSymbole[nbEntree]  = structFichier2.tabSymbole[i];
			AjoutNomStrtab(c2, strtab, &newTabSymbole[nbEntree].symbole);
			newTabSymbole[nbEntree].fichier = 2;
			nbEntree++;
		}
		else if(ELF32_ST_BIND(structFichier1.tabSymbole[i].symbole.st_info) == 1){ //symboles globaux de la seconde table
			for(j=0; j<sizeTab1; j++){
				recupNomSymbole(structFichier1.tabSymbole[j].symbole.st_name, f1, offsetStringTable1, c1);
				if(ELF32_ST_BIND(structFichier1.tabSymbole[j].symbole.st_info) == 1){ //symbole globaux de la première table
					
					if(strcmp(c1,c2) == 0){
						flag =1;
					}
				}
			}
			if(!flag){
				newTabSymbole[nbEntree]  = structFichier2.tabSymbole[i];
				AjoutNomStrtab(c2, strtab, &newTabSymbole[nbEntree].symbole);
				newTabSymbole[nbEntree].fichier = 2;
				nbEntree++;
			}
		}

		flag =0;
	}
	free(c1);
	free(c2);
	//	printf("\t\tfusionTableSymbole nbEntree %d\n",nbEntree);
	
	return nbEntree;
}

Section creerSectionTableSymbole(Symbole *tableSymbole, int sizeTableSymbole, Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2){
	Section newSection;
	int i;
	int temp_info = 0;

	//Section header
	newSection.header.sh_type = SHT_SYMTAB;
	newSection.header.sh_info = getShInfo(tableSymbole, sizeTableSymbole);
	newSection.header.sh_entsize = sizeof(Elf32_Sym);


	if (sectionHeader1.sh_addralign != sectionHeader2.sh_addralign) {
	  printf("\t\tpas le meme sh_addralign : %d et %d\n",sectionHeader1.sh_addralign,sectionHeader2.sh_addralign);
	} else {
	   newSection.header.sh_addralign = sectionHeader1.sh_addralign;
	}

	
	//Contenu section
	newSection.nbOctets = sizeTableSymbole * newSection.header.sh_entsize;
	EcrireContenu(tableSymbole, sizeTableSymbole, &newSection);
	
	//printf("\t\tcreerSectionTableSymbole sh_entsize %d\n",newSection.header.sh_entsize);
	return newSection;
}

int getSizeOfSectionTable(Symbole *tabSymbole, int sizeTableSymbole){
	int i;
	int size =0;
	for(i=0; i<sizeTableSymbole; i++){
		size+=tabSymbole[i].symbole.st_size;
	}
	return size;
}

int getShInfo(Symbole* tableSymbole, int sizeTableSymbole){
	int i;
	int temp_info = 0;
	//ajout du sh_info lorsque qu'on aura parcouru toutes les sections
	for(i=0; i<sizeTableSymbole; i++){
		if(ELF32_ST_BIND(tableSymbole[i].symbole.st_info) == STB_LOCAL){
			temp_info = i;
		}
	}
	temp_info +=1;
	return temp_info;
}

void EcrireContenu(Symbole *tableSymbole, int sizeTab, Section *section){
	int sh_entsize = section->header.sh_entsize;
	int i,j;
	char* c;
	int nbOctets = 0;
	

	section->contenu = malloc(sizeTab * sh_entsize);
	char * buffer = malloc(1);
	for(i=0; i<sizeTab;i++){
		buffer = (char *) &tableSymbole[i].symbole;
		for (j=0; j < sh_entsize; j++) {
			section->contenu[nbOctets] = *buffer;
			buffer++;
			nbOctets++;
		}
	}
	//printf("\t\tEcrire Contenu nbOctets %d entsize %d  sizeTab %d \n",nbOctets, sh_entsize, sizeTab);
}

int getSt_shndx(Symbole symbol, FichierElf * fichierElf, int nbSections, Shstrtab *shstrtab) {
	Elf32_Shdr section = fichierElf->sectionsTable[symbol.symbole.st_shndx]; //Section pointé par le st_shndx dans le fichier à fusionner
	char * name = getSectionName(section, fichierElf);

	for (int i = 0; i < nbSections; i++) { //On parcours toutes les section
		if (strcmp(name,shstrtab->names[i]) == 0) { //Pour les comparer avec le nom recherché
			return i; //Si trouvé on revoie l'index
		}

	}
	return -1; //Si pas trouvé
}



