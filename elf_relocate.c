#include "elf_reader.h"
// YA PLUS QU'A TESTER

const char* relType[] = 
	{ 
		"R_ARM_NONE", "R_ARM_PC24", "R_ARM_ABS32", "R_ARM_REL32", "R_ARM_LDR_PC_G0", 
		"R_ARM_ABS16", "R_ARM_ABS12", "R_ARM_THM_ABS5", "R_ARM_ABS8", "R_ARM_SBREL32", 
		"R_ARM_THM_CALL", "R_ARM_THM_PC8", "R_ARM_BREL_ADJ", "R_ARM_TLS_DESC", "R_ARM_THM_SWI8", 
		"R_ARM_XPC25", "R_ARM_THM_XPC22", "R_ARM_TLS_DTPMOD32", "R_ARM_TLS_DTPOFF32", "R_ARM_TLS_TPOFF32",
		"R_ARM_COPY", "R_ARM_GLOB_DAT", "R_ARM_JUMP_SLOT", "R_ARM_RELATIVE", "R_ARM_GOTOFF32", 
		"R_ARM_BASE_PREL", "R_ARM_GOT_BREL", "R_ARM_PLT32", "R_ARM_CALL", "R_ARM_JUMP24",
		"R_ARM_THM_JUMP24", "R_ARM_BASE_ABS", "R_ARM_ALU_PCREL_7_0", "R_ARM_ALU_PCREL_15_8", "R_ARM_ALU_PCREL_23_15", 
		"R_ARM_LDR_SBREL_11_0_NC", "R_ARM_ALU_SBREL_19_12_NC", "R_ARM_ALU_SBREL_27_20_CK", "R_ARM_TARGET1", "R_ARM_SBREL31", 
		"R_ARM_V4BX", "R_ARM_TARGET2", "R_ARM_PREL31", "R_ARM_MOVW_ABS_NC", "R_ARM_MOVT_ABS", 
		"R_ARM_MOVW_PREL_NC", "R_ARM_MOVT_PREL", "R_ARM_THM_MOVW_ABS_NC", "R_ARM_THM_MOVT_ABS", "R_ARM_THM_MOVW_PREL_NC",
		"R_ARM_THM_MOVT_PREL", "", "", "", "", "" 
	};


int taillerela(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rela *lesrela, FILE* elf) {	
	// Calcul de la taille de Rela et construction de sa table. 
	int nb_sec = file_header->e_shnum;
	int taille = 0; 	
	int nombre = 0;
	int k = 0; 
	int trela = sizeof(Elf32_Rela);
	for (int i = 0; i < nb_sec; i++) {
		// Pour chacun des sections du programme, si on a un type Rela :
		if (section_headers[i].sh_type == SHT_RELA) {
			// On divise la taille de la section par la taille de chaque entrée pour connaitre leur nombre
			nombre = section_headers[i].sh_size / section_headers[i].sh_entsize;	
			taille += nombre;
			lesrela = realloc(lesrela, taille * sizeof(Elf32_Rela));
			for (int j=0; j<nombre; j++) {
				// on recupere chaque element de type Rela
				fseek(elf, section_headers[i].sh_offset, SEEK_SET);
				fread(&(lesrela[k]),trela, 1, elf);
				k++;
			}
		}
	} 
	return taille;
}

int taillerel(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rel *lesrel, FILE* elf) {
	// Calcul de la taille de Rela et construction de sa table. 
	int nb_sec = file_header->e_shnum;
	int taille = 0; 	
	int nombre = 0;
	int k = 0; 
	int trel = sizeof(Elf32_Rel);
	for (int i = 0; i < nb_sec; i++) {
		// Pour chacun des sections du programme, si on a un type Rela :
		if (section_headers[i].sh_type == SHT_REL) {
			// On divise la taille de la section par la taille de chaque entrée pour connaitre leur nombre
			fseek(elf, section_headers[i].sh_offset, SEEK_SET);
			nombre = section_headers[i].sh_size / section_headers[i].sh_entsize;	
			taille += nombre;
			lesrel = realloc(lesrel, taille * sizeof(Elf32_Rel));
			for (int j=0; j<nombre; j++) {
				// on recupere chaque element de type Rel
				fread(&(lesrel[k]),trel, 1, elf);
				printf("offset : %x \n",lesrel[k].r_offset);
				printf("info : %x \n",lesrel[k].r_info);
				k++;
			}
		}
	} 
	return taille;
}
/*
	size_rel = taillerel(elf_struct);	
	size_rela = taillerela(elf_struct);

	elf_struct->a_rel = malloc( sizeof(Elf32_Rel) * size_rel );
	elf_struct->a_rela = malloc( sizeof(Elf32_Rela) * size_rela );
	Malloc sans size_rel + realloc à chaque fois
	if ( get_rela_table(elf_struct) == -1) {
		printf("Error reading file to be relocated.\n");
		return -1;
	}

	if ( get_rel_table(elf_struct) == -1) {
		printf("Error reading file to be relocated.\n");
		return -1;
	}*/

int affichage_relocation(Elf32_Sym* tabSymbole, Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers, FILE* elf) {
	
	Elf32_Rela* rela = malloc(sizeof(Elf32_Rela)) ;
	Elf32_Rel* rel = malloc(sizeof(Elf32_Rel));
	
	
	int trela = taillerela(fileHeader,sections_headers,rela,elf);
	int trel = taillerel(fileHeader,sections_headers,rel,elf);
	int nb_sec = fileHeader->e_shnum;
	int j = 0 ;	
	int noms,nombres;
	char c;	
	const char* type;
	unsigned int strtab = 1;	
	unsigned int shstrtab = 0;
	char* nomsec;		

	//Initialisation des adresses de STRTAB et SHSTRTAB
	shstrtab = sections_headers[fileHeader->e_shstrndx].sh_offset;	
	 
	if ( (trela + trel) == 0 ) {printf("Pas de Relocation a faire \n"); return -1;}
	
	for (int i = 0; i < nb_sec; i++) {
		if ( (sections_headers[i].sh_type == SHT_STRTAB) && (sections_headers[i].sh_offset != shstrtab) ) {
			strtab = sections_headers[i].sh_offset;
			// Ajout de l'offset à strtab
		}
	}

	if (strtab == 1) {printf("Pas de table des chaines \n"); return -1;}
		
	// Affichage de Rela
	j = 0;
	for (int k = 0; k < nb_sec; k++) {
		
		if (sections_headers[k].sh_type == SHT_RELA) {	
			
			fseek(elf, shstrtab + sections_headers[k].sh_name, SEEK_SET);
			noms = 0;
			c = fgetc(elf);
			while (c != '\0') {
				c = fgetc(elf);
				noms++;
			}
			nomsec = malloc( (noms + 1) * sizeof(char) );
			fseek(elf, sections_headers[fileHeader->e_shstrndx].sh_offset + sections_headers[k].sh_name, SEEK_SET);
			noms = 0;
			
			c = fgetc(elf);
			while (c != '\0') {
				nomsec[noms] = c;
				noms++;
				c = fgetc(elf);
			}
			nomsec[noms] = c;
			
			// Affichage de Rela 
			nombres = sections_headers[k].sh_size / sections_headers[k].sh_entsize;	

			printf("Relocation :  Nom Section : %s  à l'adresse 0x%x avec %d éléments \n", nomsec, sections_headers[k].sh_offset, nombres);
			printf("RELA - Symboles \n");
			printf("offset:       info :           Type:        Valeur Symbole : \n");

	
			for (int l = 0; l < nombres; l++) {
				// get type
				type = relType[ELF32_R_TYPE(rel[j].r_info)]; 
				printf("%x     %x     %s       %x    \n",rel[j].r_offset, rel[j].r_info, type, tabSymbole[ELF32_R_SYM(rela[j].r_info)].st_value);
				j++;				
			}		
			free(nomsec);	
		}		
	}	

	// Displaying Rel content
	j = 0;
	for (int k = 0; k < nb_sec; k++) {
		if (sections_headers[k].sh_type == SHT_REL) {	
			
			fseek(elf, shstrtab + sections_headers[k].sh_name, SEEK_SET);
			noms = 0;
			c = fgetc(elf);

			while (c != '\0') {
				c = fgetc(elf);
				noms++;
			}

			nomsec = malloc( (noms + 1) * sizeof(char) );
			fseek(elf, sections_headers[fileHeader->e_shstrndx].sh_offset + sections_headers[k].sh_name, SEEK_SET);
			noms = 0;
			
			c = fgetc(elf);
			while (c != '\0') {
				nomsec[noms] = c;
				noms++;
				c = fgetc(elf);
			}
			nomsec[noms] = c;

			//printRel
			nombres = sections_headers[k].sh_size / sections_headers[k].sh_entsize;	

			printf("Relocation :  Nom Section : %s  à l'adresse 0x%x avec %d éléments \n", nomsec, sections_headers[k].sh_offset, nombres);
			printf("REL - Symboles \n");
			printf("offset:       info :           Type:        Valeur Symbole : \n");

	
			for (int l = 0; l < nombres; l++) {
				// get type
				type = relType[ELF32_R_TYPE(rel[j].r_info)]; 
				printf("%d     %x     %s       %x    \n",rel[j].r_offset, rel[j].r_info, type, tabSymbole[ELF32_R_SYM(rel[j].r_info)].st_value);
				printf(" num %d \n",j);
				j++;				
			}			
			free(nomsec);	
		}		
	}	

	printf("\n");
	for (int i = 0; i < 10; i++) { 
		printf("offset : %x  ----- ", rel[i].r_offset);
		printf("info : %x \n", rel[i].r_info);
		printf("offset : %x ", rela[i].r_offset);
		printf("---- info : %x \n", rela[i].r_info);
		printf("\n");
	}
	return 1;
	
}
