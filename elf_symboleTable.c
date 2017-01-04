#include "elf_reader.h"

int lectureTableSymbole(Elf32_Sym* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f){	
	int nbSection = header.e_shnum;
	int i, j;
	int nbEntree = 0;
	int nbSymbole;
	for(i=0; i<nbSection; i++){
		if(tabSection[i].sh_type == SHT_SYMTAB || tabSection[i].sh_type == SHT_DYNSYM){ //TODO: vérifier le test de comparaison
			fseek(f, tabSection[i].sh_offset, SEEK_SET);
			nbSymbole = tabSection[i].sh_size / tabSection[i].sh_entsize;
			for(j = 0; j<nbSymbole; j++){			
				fread(&tabSymbole[nbEntree].st_name, sizeof(Elf32_Word), 1, f);
				tabSymbole[nbEntree].st_name = Swap16(tabSymbole[nbEntree].st_name, header.e_ident[5]);
				fread(&tabSymbole[nbEntree].st_value, sizeof(Elf32_Addr), 1, f);
				tabSymbole[nbEntree].st_value = Swap16(tabSymbole[nbEntree].st_value, header.e_ident[5]);	
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

Elf32_Off getIndexStringTable(Elf32_Shdr* tabSection){
	int i=0;
	while(tabSection[i].sh_name != SHT_STRTAB){
		i++;
	}
	return tabSection[i].sh_offset;
}

void recupNomSymbole(Elf32_Off offsetStringTable, Elf32_Word index, FILE* f){
	fseek(f, offsetStringTable+index, SEEK_SET);
	char c;
	c = fgetc(f);
	while(c!= '\0'){
		printf("%c", c);
		c = fgetc(f);
	}
	
}


void affichageTableSymbole(Elf32_Sym* tabSymbole, int size, FILE* f, Elf32_Shdr* tabSection){
	int i;
	for(i=0; i<size; i++){
		recupNomSymbole(getIndexStringTable(tabSection, tabSymbole[i].st_name;
		printf("value: %x \n", tabSymbole[i].st_value);
		printf("size: %d \n", tabSymbole[i].st_size);
		printf("bind: ");
		switch(ELF_32_ST_BIND(tabSymbole[i].st_info)){
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





