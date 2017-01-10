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
			tabSymbole = realloc(tabSymbole, sizeof(Elf32_Sym)*nbSymbole);
			if(tabSymbole == NULL){
				printf("erreur alloc \n");
				exit(1);
			}
			for(j = 0; j<nbSymbole; j++){		

				fread(&tabSymbole[nbEntree].st_name, sizeof(Elf32_Word), 1, f);
				tabSymbole[nbEntree].st_name = Swap32(tabSymbole[nbEntree].st_name, header.e_ident[5]);

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
			tabSymboleDynamique = realloc(tabSymboleDynamique, sizeof(Elf32_Sym)*nbSymbole);
			if(tabSymboleDynamique == NULL){
				printf("erreur alloc \n");
				exit(1);
			}

			for(j = 0; j<nbSymbole; j++){		
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
		return currentSection.sh_offset;
	} else {
		printf("erreur recherche section \n");
		return NULL;
	}
}

void recupNomSymbole(Elf32_Word index, FILE* f, Elf32_Word offset, char *c){
	fseek(f, offset+index, SEEK_SET);
	int i = 0 ;
	while((c[i] = fgetc(f))!= '\0'){
		i++;
	}
}


void affichageTableSymbole( Elf32_Sym* tabSymbole, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header){
		printf("\n-------------------Table des symboles // Nb Symbole: %d-----------------------------\n\n", sizeTabSymbole);

	int i;
	Elf32_Word offset;
	char * stringTabSymb;
	char *nomSymbole;
	nomSymbole = malloc(50);
	stringTabSymb = malloc(50);
	stringTabSymb = ".strtab";
	offset = rechercheOffsetSection(header, tabSection, f, stringTabSymb);
	printf("num		value		size		bind		type			other			shndx 				nom\n");
	for(i=0; i<sizeTabSymbole; i++){
		printf("[%3d]", i);
		recupNomSymbole(tabSymbole[i].st_name, f, offset, nomSymbole);
		printf("%15x", tabSymbole[i].st_value);
		printf("%15d", tabSymbole[i].st_size);
		printf("		");
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
		printf("		");
		switch(ELF32_ST_TYPE(tabSymbole[i].st_info)){
			case 0:
				printf("NOTYPE ");
				break;
			case 1:
				printf("OBJECT ");
				break;
			case 2:
				printf("FUNC   ");
				break;
			case 3: 
				printf("SECTION");
				break;
			case 4:
				printf("FILE   ");
				break;
			case 13:
				printf("LOPROC ");
				break;
			case 15:
				printf("HIPROC ");
				break;
			default:
				printf("??     ");
				break;
		}
		printf("%20d", tabSymbole[i].st_other);
		printf("			");
		switch(tabSymbole[i].st_shndx){
			case SHN_UNDEF:
				printf("UNDEF");
				break;
			case SHN_ABS:
				printf("ABS");
				break;
			default:
				printf("%2d", tabSymbole[i].st_shndx);
				break;
		}
		printf("%40s", nomSymbole);
		printf("\n");
	}
}

void affichageTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, int sizeTabSymboleDynamique, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header){
	printf("\n-------------------Table des symboles dynamique // Nb Symbole: %d-----------------------------\n\n", sizeTabSymboleDynamique);
	int i;
	Elf32_Word offsetTableStringDynamique;
	char * tabSymbdynamique;
	char *nomSymboleDynamique;
	tabSymbdynamique = malloc(50);
	nomSymboleDynamique = malloc(50);
	tabSymbdynamique = ".dynstr";
	offsetTableStringDynamique = rechercheOffsetSection(header, tabSection, f, tabSymbdynamique);
	printf("num		value		size		bind		type			other			shndx 				nom\n");
	for(i=0; i<sizeTabSymboleDynamique; i++){
		printf("[%d]", i);
		recupNomSymbole(tabSymboleDynamique[i].st_name, f, offsetTableStringDynamique, nomSymboleDynamique);
		printf("%15x", tabSymboleDynamique[i].st_value);
		printf("%15d", tabSymboleDynamique[i].st_size);
		printf("		");
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
		printf("		");
		switch(ELF32_ST_TYPE(tabSymboleDynamique[i].st_info)){
			case 0:
				printf("NOTYPE ");
				break;
			case 1:
				printf("OBJECT ");
				break;
			case 2:
				printf("FUNC   ");
				break;
			case 3: 
				printf("SECTION");
				break;
			case 4:
				printf("FILE   ");
				break;
			case 13:
				printf("LOPROC ");
				break;
			case 15:
				printf("HIPROC ");
				break;
			default:
				printf("??     ");
				break;
		}
		printf("%20d", tabSymboleDynamique[i].st_other);
		printf("			");
		switch(tabSymboleDynamique[i].st_shndx){
			case SHN_UNDEF:
				printf("UNDEF");
				break;
			case SHN_ABS:
				printf("ABS");
				break;
			default:
				printf("%2d", tabSymboleDynamique[i].st_shndx);
				break;
		}
		printf("%40s", nomSymboleDynamique);
		printf("\n");
	}
}

