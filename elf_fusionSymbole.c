#include "elf_reader.h"



Elf32_Shdr getSectionByName(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName){
	int i = 0;
	char * currentName = malloc(40);
	char courant;
	int trouve = 0;
	Elf32_Shdr currentSection;
	
	int offsetNameTable = sections_table[header_elf.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections

	//On parcour chaque section jusqu'à trouver la bonne ou, toute les parcourirs
	while (!trouve && i < header_elf.e_shnum) {
		//On se place dans le fichier afin de retouver le nom de la section courante
		currentSection = sections_table[i];
		fseek(elf, offsetNameTable + currentSection.sh_name, SEEK_SET); //offset nous ammène au début du stockage des noms, et sh_name contient l'index du nom de la section courante

		//On recupere le nom de la section courrante
		int j =0;
		while((courant = fgetc(elf))!= '\0'){
			currentName[j] = courant;	
			j++;		
		}
		currentName[j] = '\0';

		//Si le nom corespond à la recherche on arrête
		if (strcmp(secName, currentName) == 0) {
			trouve = 1;
			return sections_table[i];
		}

		i++;
		return NULL;
	}

	//Une fois la bonne section trouvee on affiche son contenue
	if (trouve) {
		afficheContenue(currentSection, elf);
	} else {
		printf("Aucune section ne correspond à ce nom\n");
	}
}


int fusionTableSymbole(fichierElf structFichier1, fichierElf structFichier2, FILE* f1, FILE* f2, int sizeTab1, int sizeTab2, Elf32_Sym *newTabSymbole){
	int i;
	int j;
	char *c1;
	char *c2;
	c1 = malloc(50);
	c2 = malloc(50);
	
	Elf32_Sym* newTabSymbole;
	int nbEntree = 0;
	int flag = 0;
	
	int offsetStringTable1 = rechercheOffsetSection(structFichier1.header, structFichier1.sectionsTable, f1, ".strtab");
	int offsetStringTable2 = rechercheOffsetSection(structFichier2.header, structFichier2.sectionsTable, f2, ".strtab");
	for(i=0; i<sizeTab1; i++){
		if(ELF32_ST_BIND(structFichier1.tabSymbole[i].st_info) == 1){ //Symboles locaux
			newTabSymbole[i].structFichier1.tabSymbole[i];
			nbEntree++;
		}else if(ELF32_ST_BIND(structFichier1.tabSymbole[i].st_info) == 0){ //symboles globaux
			c1 = recupNomSymbole(structFichier1.tabSymbole[i].st_name, f1, offsetStringTable1);
			for(j=0; j<sizeTab2; j++){
				c2 = recupNomSymbole(structFichier2.tabSymbole[j].st_name, f2, offsetStringTable2);
				if(strcmp(c1,c2) == 0){ // Les symboles ont le même nom
					flag = 1;
					if(structFichier1.tabSymbole[i].st_shndx != Sh_UNDEF && structFichier2.tabSymbole[j].st_shndx != Sh_UNDEF){ //Les symboles sont tous les deux définis
					printf("Echec de l'édition de lien, deux symboles ont un nom identique\n");
					exit(0);
					}else{
						if(structFichier1.tabSymbole[i].st_shndx != Sh_UNDEF && structFichier2.tabSymbole[j].st_shndx == Sh_UNDEF){ //Le symbole est défini dans le premier fichier pas dans le deuxième
							newTabSymbole.tabSymbole[nbEntree] = structFichier1.tabSymbole[i];
							nbEntree++;
						}
						if(structFichier1.tabSymbole[i].st_shndx == Sh_UNDEF && structFichier2.tabSymbole[j].st_shndx != Sh_UNDEF){ //Le symbole est défini dans le deuxième fichier pas dans le premier
							newTabSymbole.tabSymbole[nbEntree] = structFichier2.tabSymbole[j];
							nbEntree++;
						}
						if(structFichier1.tabSymbole[i].st_shndx == Sh_UNDEF && structFichier2.tabSymbole[j].st_shndx == Sh_UNDEF){ //Le symbole n'est défini dans aucune des deux tables
							newTabSymbole.tabSymbole[nbEntree] = structFichier1.tabSymbole[i];
							nbEntree++;
						}
					}
				}
			}
			if(!flag){ // Si le symbole n'apparait que dans la première table
				newTabSymbole[nbEntree] = structFichier1.tabSymbole[i];
				nbEntree++;
			}
			flag=0;
		}
	}
	for(i=0; i<sizeTab2; i++){// boucle pour vérifier si un symbole n'apparait que dans la deuxième table
		c2 = recupNomSymbole(structFichier2.tabSymbole[i].st_name, f2, offsetStringTable2);
		for(j=0; j<sizeTab1; j++){
			c1 = recupNomSymbole(structFichier1.tabSymbole[j].st_name, f1, offsetStringTable1);
			if(strcmp(c1,c2) == 0){
				flag =1;
			}
		}
		if(!flag){
			newTabSymbole[nbEntree] = structFichier2.tabSymbole[i];
			nbEntree++;
		}
		flag =0;
	}
	return nbEntree
}

Section creerSectionTableSymbole(Elf32_Sym *tableSymbole, int sizeTableSymbole, FichierElf structFichier1, FichierElf structFichier2){
	Section newSection;
	int i;
	int temp_info = 0;
	newSection.header.sh_type = SHT_SYMTAB;
	newSection.header.sh_info = getShInfo(tableSymbole, sizeTableSymbole);
	newSection.header.sh_entsize = getShEntSize(structFichier1, structFichier2);
	
	
	
	newSection.nbOctets = getSizeOfSectionTable(tabSymbole, sizeTableSymbole);
}

int getSizeOfSectionTable(Elf32_Sym *tabSymbole, int sizeTableSymbole){
	int i;
	int size =0;
	for(i=0; i<sizeTableSymbole; i++){
		size+=tabSymbole[i].st_size;
	}
	return size;
}

int getShEntSize(FichierElf structFichier1, FichierElf structFichier2){
	int sh_entsize;
	if(structFichier1.header.sh_entsize >= structFichier2.header.sh_entsize=){
		return structFichier1.header.sh_entsize;
	}else{
			return structFichier2.header.sh_entsize;
	}
}

int getShInfo(Elf32_Sym* tableSymbole, int sizeTableSymbole){
	int i;
	int temp_info = 0;
	//ajout du sh_info lorsque qu'on aura parcouru toutes les sections
	for(i=0; i<sizeTableSymbole; i++){
		if(ELF32_ST_BIND(tabSymbole[i].st_info) == STB_LOCAL){
			temp_info = i;
		}
	}
	temp_info +=1;
	return temp_info;
}
