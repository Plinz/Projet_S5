#include "elf_reader.h"

int lectureTableSymbole(Elf32_Sym* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f){	
	int nbSection = header.e_shnum;
	int i, j;
	int nbEntree = 0;
	int nbSymbole;
	for(i=0; i<nbSection; i++){
		
		
		if(tabSection[i].sh_type == SHT_SYMTAB){ //|| tabSection[i].sh_type == SHT_DYNSYM){ //TODO: vérifier le test de comparaison
			fseek(f, tabSection[i].sh_offset, SEEK_SET);
			nbSymbole = tabSection[i].sh_size / tabSection[i].sh_entsize;
			printf("Taille section : %d    Taille fixée : %d     NbSymbole : %d\n",tabSection[i].sh_size,tabSection[i].sh_entsize,nbSymbole);
			tabSymbole = realloc(tabSymbole, sizeof(Elf32_Sym)*nbSymbole);
			if(tabSymbole == NULL){
				printf("erreur alloc");
				exit(1);
			}
			printf("\n test1 \n");
			for(j = 0; j<nbSymbole; j++){		
				printf("Fread - 1\n");
				fread(&tabSymbole[nbEntree].st_name, sizeof(Elf32_Word), 1, f);
				printf("tabSymbole - 1\n");
				tabSymbole[nbEntree].st_name = Swap32(tabSymbole[nbEntree].st_name, header.e_ident[5]);
				printf("Fread - 2\n");
				fread(&tabSymbole[nbEntree].st_value, sizeof(Elf32_Addr), 1, f);
				printf("tabSymbole - 2\n");
				tabSymbole[nbEntree].st_value = Swap32(tabSymbole[nbEntree].st_value, header.e_ident[5]);	
				printf("Fread - 3\n");
				fread(&tabSymbole[nbEntree].st_size, sizeof(Elf32_Word), 1, f);
				printf("tabSymbole - 3\n");
				tabSymbole[nbEntree].st_size = Swap16(tabSymbole[nbEntree].st_size, header.e_ident[5]);	
				printf("Fread - 4\n");	
				fread(&tabSymbole[nbEntree].st_info, sizeof(unsigned char), 1, f);
				printf("tabSymbole - 4\n");
				tabSymbole[nbEntree].st_info = Swap16(tabSymbole[nbEntree].st_info, header.e_ident[5]);
				printf("Fread - 5\n");
				fread(&tabSymbole[nbEntree].st_other, sizeof(unsigned char), 1, f);
				printf("tabSymbole - 5\n");
				tabSymbole[nbEntree].st_other = Swap16(tabSymbole[nbEntree].st_other, header.e_ident[5]);
				printf("Fread - 6\n");
				fread(&tabSymbole[nbEntree].st_shndx, sizeof(Elf32_Half), 1, f);
				printf("tabSymbole - 6\n");
				tabSymbole[nbEntree].st_shndx = Swap16(tabSymbole[nbEntree].st_shndx, header.e_ident[5]);			
				nbEntree++;
				printf("Fin Boucle - %d       j:%d\n\n\n",nbEntree, j);	
			}
			printf("test 2\n");
		}
	}
	return nbEntree;
}


void recupNomSymbole(Elf32_Word index, Elf32_Shdr *tabSection, FILE* f){

	//int i=0;
	int offset;/*
	while(strcmp(tabSection[i].sh_name, ".strtab"){
		i++;
		printf(" tab section index i: %d \n", i);
	}*/
	offset = tabSection[30].sh_offset;
	
	fseek(f, offset+index, SEEK_SET);
	char c;
	c = fgetc(f);
	while(c!= '\0'){
		printf("%c", c);
		c = fgetc(f);
	}
	printf("\n");


}


void affichageTableSymbole(Elf32_Sym* tabSymbole, int size, FILE* f, Elf32_Shdr* tabSection){
	int i;
	for(i=0; i<size; i++){
		recupNomSymbole(tabSymbole[i].st_name, tabSection, f);
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





