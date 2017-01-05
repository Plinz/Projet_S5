#include "elf_reader.h"

int lectureTableSymbole(Elf32_Sym* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f){	
	int nbSection = header.e_shnum;
	int i, j;
	int nbEntree = 0;
	int nbSymbole;
	for(i=0; i<nbSection; i++){
		
		
		if(tabSection[i].sh_type == SHT_SYMTAB){
			fseek(f, tabSection[i].sh_offset, SEEK_SET);
			nbSymbole = tabSection[i].sh_size / tabSection[i].sh_entsize;
			printf("Taille section : %d    Taille fixée : %d     NbSymbole : %d\n",tabSection[i].sh_size,tabSection[i].sh_entsize,nbSymbole);
			tabSymbole = realloc(tabSymbole, sizeof(Elf32_Sym)*nbSymbole);
			if(tabSymbole == NULL){
				printf("erreur alloc \n");
				exit(1);
			}
			printf("\n test1 \n");
			for(j = 0; j<nbSymbole; j++){		

				fread(&tabSymbole[nbEntree].st_name, sizeof(Elf32_Word), 1, f);
				tabSymbole[nbEntree].st_name = Swap32(tabSymbole[nbEntree].st_name, header.e_ident[5]);
				printf("st name: %d \n", tabSymbole[nbEntree].st_name);

				fread(&tabSymbole[nbEntree].st_value, sizeof(Elf32_Addr), 1, f);
				tabSymbole[nbEntree].st_value = Swap32(tabSymbole[nbEntree].st_value, header.e_ident[5]);	

				fread(&tabSymbole[nbEntree].st_size, sizeof(Elf32_Word), 1, f);
				tabSymbole[nbEntree].st_size = Swap16(tabSymbole[nbEntree].st_size, header.e_ident[5]);	
	
				fread(&tabSymbole[nbEntree].st_info, sizeof(unsigned char), 1, f);
				tabSymbole[nbEntree].st_info = Swap16(tabSymbole[nbEntree].st_info, header.e_ident[5]);

				fread(&tabSymbole[nbEntree].st_other, sizeof(unsigned char), 1, f);
				tabSymbole[nbEntree].st_other = Swap16(tabSymbole[nbEntree].st_other, header.e_ident[5]);

				fread(&tabSymbole[nbEntree].st_shndx, sizeof(Elf32_Half), 1, f);
				tabSymbole[nbEntree].st_shndx = Swap16(tabSymbole[nbEntree].st_shndx, header.e_ident[5]);			
				nbEntree++;

			}
		}
	}
	return nbEntree;
}


int lectureTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f){	
	int nbSection = header.e_shnum;
	int i, j;
	int nbEntree = 0;
	int nbSymbole;
	for(i=0; i<nbSection; i++){
		
		
		if(tabSection[i].sh_type == SHT_DYNSYM){
			fseek(f, tabSection[i].sh_offset, SEEK_SET);
			nbSymbole = tabSection[i].sh_size / tabSection[i].sh_entsize;
			printf("Taille section : %d    Taille fixée : %d     NbSymbole : %d\n",tabSection[i].sh_size,tabSection[i].sh_entsize,nbSymbole);
			tabSymboleDynamique = realloc(tabSymboleDynamique, sizeof(Elf32_Sym)*nbSymbole);
			if(tabSymboleDynamique == NULL){
				printf("erreur alloc \n");
				exit(1);
			}

			for(j = 0; j<nbSymbole; j++){	
				printf("je passe \n");	
				fread(&tabSymboleDynamique[nbEntree].st_name, sizeof(Elf32_Word), 1, f);
				tabSymboleDynamique[nbEntree].st_name = Swap32(tabSymboleDynamique[nbEntree].st_name, header.e_ident[5]);

				fread(&tabSymboleDynamique[nbEntree].st_value, sizeof(Elf32_Addr), 1, f);
				tabSymboleDynamique[nbEntree].st_value = Swap32(tabSymboleDynamique[nbEntree].st_value, header.e_ident[5]);	

				fread(&tabSymboleDynamique[nbEntree].st_size, sizeof(Elf32_Word), 1, f);
				tabSymboleDynamique[nbEntree].st_size = Swap16(tabSymboleDynamique[nbEntree].st_size, header.e_ident[5]);	

				fread(&tabSymboleDynamique[nbEntree].st_info, sizeof(unsigned char), 1, f);
				tabSymboleDynamique[nbEntree].st_info = Swap16(tabSymboleDynamique[nbEntree].st_info, header.e_ident[5]);

				fread(&tabSymboleDynamique[nbEntree].st_other, sizeof(unsigned char), 1, f);
				tabSymboleDynamique[nbEntree].st_other = Swap16(tabSymboleDynamique[nbEntree].st_other, header.e_ident[5]);

				fread(&tabSymboleDynamique[nbEntree].st_shndx, sizeof(Elf32_Half), 1, f);
				tabSymboleDynamique[nbEntree].st_shndx = Swap16(tabSymboleDynamique[nbEntree].st_shndx, header.e_ident[5]);			
				nbEntree++;	
			}
			printf("test 2\n");
		}
	}
	return nbEntree;
}


Elf32_Word rechercheOffsetSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName){
	int i = 0;
	char * currentName = malloc(50);
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
		}

		i++;
	}

	//Une fois la bonne section trouvee on affiche son contenue
	if (trouve) {
		printf("offset: %d \n", currentSection.sh_offset);
		return currentSection.sh_offset;
	} else {
		printf("erreur recherche section \n");
		return NULL;
	}
}

void recupNomSymbole(Elf32_Word index, FILE* f, Elf32_Word offset){
	fseek(f, offset+index, SEEK_SET);
	char c;
	c = fgetc(f);
	while(c!= '\0'){
		printf("%c", c);
		c = fgetc(f);
	}
	printf("\n");
}


void affichageTableSymbole( Elf32_Sym* tabSymbole, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header){
	int i;
	
	Elf32_Word offset;
	char * stringTabSymb;
	stringTabSymb = malloc(50);
	stringTabSymb = ".strtab";
	offset = rechercheOffsetSection(header, tabSection, f, stringTabSymb);
	for(i=0; i<sizeTabSymbole; i++){
		printf("nom: ");
		recupNomSymbole(tabSymbole[i].st_name, f, offset);
		printf("value: %x \n", tabSymbole[i].st_value);
		printf("size: %d \n", tabSymbole[i].st_size);
		printf("bind: ");
		switch(ELF32_ST_BIND(tabSymbole[i].st_info)){
			case 0:
				printf("LOCAL");
				break;
			case 1:
				printf("GLOBAL");
				break;
			case 2:
				printf("WEAK");
				break;
			case 13:
				printf("LOPROC");
				break;
			case 15:
				printf("HIPROC");
				break;
			default:
				printf("??");
				break;
		}
		printf("\n");
	}
}

void affichageTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, int sizeTabSymboleDynamique, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header){
	printf("-------------------Symbole dynamique-----------------------------");
	int i;
	Elf32_Word offsetTableStringDynamique;
	char * tabSymbdynamique;
	tabSymbdynamique = malloc(50);
	tabSymbdynamique = ".dynstr";
	offsetTableStringDynamique = rechercheOffsetSection(header, tabSection, f, tabSymbdynamique);
	for(i=0; i<sizeTabSymboleDynamique; i++){
		recupNomSymbole(tabSymboleDynamique[i].st_name, f, offsetTableStringDynamique);
		printf("value: %x \n", tabSymboleDynamique[i].st_value);
		printf("size: %d \n", tabSymboleDynamique[i].st_size);
		printf("bind: ");
		switch(ELF32_ST_BIND(tabSymboleDynamique[i].st_info)){
			case 0:
				printf("LOCAL");
				break;
			case 1:
				printf("GLOBAL");
				break;
			case 2:
				printf("WEAK");
				break;
			case 13:
				printf("LOPROC");
				break;
			case 15:
				printf("HIPROC");
				break;
			default:
				printf("??");
				break;
		}
		printf("\n");
	}
}

