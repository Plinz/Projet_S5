#include "elf_reader.h"
// * A MODIFIER ET A PERSONNALISER POUR QUE CA MARCHE AVEC NOUS :3

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

int get_rela_size(ELF_STRUCT *elf_struct) {

	Elf32_Ehdr* elf_header = elf_struct->elf_header;
	Elf32_Shdr* a_shdr = elf_struct->a_shdr;	
int nb_shdr = elf_header->e_shnum;
	int i = 0;	// i counter
	int nb_rela = 0; 	// nb number of rela
	int nb_entries = 0;
	
	for (i = 0; i < nb_shdr; i++) {
		if (a_shdr[i].sh_type == SHT_RELA) {
			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	// calcul the number of entries
			nb_rela += nb_entries;
		}
	} 

	return nb_rela;

}

int get_rel_size(ELF_STRUCT *elf_struct) {

	Elf32_Ehdr* elf_header = elf_struct->elf_header;
	Elf32_Shdr* a_shdr = elf_struct->a_shdr;

	int nb_shdr = elf_header->e_shnum;
	int i = 0;	// i counter
	int nb_rel = 0; 	// nb number of rel
	int nb_entries = 0;
	
	for (i = 0; i < nb_shdr; i++) {
		if (a_shdr[i].sh_type == SHT_REL) {
			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	// calcul the number of entries
			nb_rel += nb_entries;
		}
	}
	return nb_rel;

}

int get_rela_table(ELF_STRUCT *elf_struct) {
	
	FILE* f = elf_struct->elf_file;
	Elf32_Ehdr* elf_header = elf_struct->elf_header;
	Elf32_Shdr* a_shdr = elf_struct->a_shdr;
	Elf32_Rela* a_rela = elf_struct->a_rela;

	int nb_shdr = elf_header->e_shnum;
	int i = 0, j = 0, k = 0;	// i counter ,j table counter, k entry counter
	int nb_entries;	
	int rela_size = sizeof(Elf32_Rela);

	for (i = 0; i < nb_shdr; i++) {
		
		if (a_shdr[i].sh_type == SHT_RELA) {
			
			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	// calcul the number of entries

			for(k = 0; k < nb_entries; k++) {
				
				if ( fseek(f, a_shdr[i].sh_offset, SEEK_SET) == -1 ) {
					fprintf(stderr, "Error while fseek to read rela\n");
					return -1;
				}
				
				if ( fread(&(a_rela[j]), rela_size, 1, f) != 1 ) {
					fprintf(stderr, "Error while reading rela\n");
					return -1;
				}

				if ( this_needs_reverse(elf_header->e_ident[EI_DATA]) ) {

					a_rela[j].r_offset	= reverse_4(a_rela[j].r_offset);
					a_rela[j].r_info	= reverse_4(a_rela[j].r_info);
					a_rela[j].r_addend	= reverse_4(a_rela[j].r_addend);		

				}

				j++;

			}
		}		
	}

	return 1;	
}

int get_rel_table(ELF_STRUCT *elf_struct) {
	
	FILE* f = elf_struct->elf_file;
	Elf32_Ehdr* elf_header = elf_struct->elf_header;
	Elf32_Shdr* a_shdr = elf_struct->a_shdr;
	Elf32_Rel* a_rel = elf_struct->a_rel;

	int nb_shdr = elf_header->e_shnum;
	int i = 0, j = 0, k = 0;	// i counter ,j table counter, k entry counter
	int nb_entries;	//number of entry
	int rel_size = sizeof(Elf32_Rel);

	for (i = 0; i <nb_shdr; i++) {
		
		if (a_shdr[i].sh_type == SHT_REL) {


			if ( fseek(f, a_shdr[i].sh_offset, SEEK_SET) == -1 ) {
				fprintf(stderr, "Error while fseek to read rel\n");
				return -1;
			}

			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	// calcul the number of entries	
			
			for (k = 0; k < nb_entries; k++) {
				
				if ( fread(&(a_rel[j]), rel_size, 1, f) != 1 ) {
					fprintf(stderr, "Error while reading rel\n");
					return -1;
				}
				
				if ( this_needs_reverse(elf_header->e_ident[EI_DATA]) ) {

					a_rel[j].r_offset	= reverse_4(a_rel[j].r_offset);
					a_rel[j].r_info	= reverse_4(a_rel[j].r_info);

				}

				j++;

			}
		}		
	}

	return 1;

}

int display_relocate_section(ELF_STRUCT *elf_struct) {
	
	FILE* f = elf_struct->elf_file;
	Elf32_Ehdr* elf_header = elf_struct->elf_header;
	Elf32_Shdr* a_shdr = elf_struct->a_shdr;
	Elf32_Rela* rela = elf_struct->a_rela;
	Elf32_Rel* rel = elf_struct->a_rel;
	Elf32_Sym* a_symb = elf_struct->a_sym; 
	
	int rela_size = get_rela_size(elf_struct);
	int rel_size = get_rel_size(elf_struct);
	int nb_shdr = elf_header->e_shnum;
	int i = 0, j = 0, k = 0, l = 0;		// i counter ,j table counter, k entry counter, l symb counter
	int iCnt;		// iCnt Name counter
	char c;			// c counter for reading FILE
	const char* type;	// Type name
	char* symName = NULL;	// Symbol name
	int nb_entries;		// Number of entry	
	int strndx = -1;	// STRTAB address
	int shstrndx = 0;	// SHSTRTAB address
	int symb_size = 0;	// Symbol size
	char* name;		// Section name

	//Initialise strndx and shstrndx
	shstrndx = a_shdr[elf_header->e_shstrndx].sh_offset;	
	 
	if ( (rela_size + rel_size) == 0 ) {
		printf("No relocation in this file.\n\n");
		return 1;
	}
	
	for (l = 0; l < elf_header->e_shnum; l++) {
		if ( (a_shdr[l].sh_type == SHT_STRTAB) && (a_shdr[l].sh_offset != shstrndx) ) {
			strndx = a_shdr[l].sh_offset;
		}
	}

	if (strndx == -1) return -1;
		
	// Displaying Rela content
	j = 0;
	for (i = 0; i < nb_shdr; i++) {
		
		if (a_shdr[i].sh_type == SHT_RELA) {	
			
			//getName
			if ( fseek(f, shstrndx + a_shdr[i].sh_name, SEEK_SET) == -1 ) {
				fprintf(stderr, "error while fseek\n");
				return -1;
			}

			iCnt = 0;
			
			c = fgetc(f);
			if ( ferror(f) ) return -1;

			while (c != '\0') {
				iCnt++;
				c = fgetc(f);
				if ( ferror(f) ) return -1;
			}

			name = malloc( (iCnt + 1) * sizeof(char) );
			if (name == NULL) {
				fprintf(stderr, "Error malloc name \n");
				return -1;	
			}

			if ( fseek(f, a_shdr[elf_header->e_shstrndx].sh_offset + a_shdr[i].sh_name, SEEK_SET) == - 1 ) {
				fprintf(stderr, "error while fseek\n");
				free(name);
				return -1;
			}

			iCnt = 0;
			
			c = fgetc(f);
			if ( ferror(f) ) {
				free(name);
				return -1;
			}

			while (c != '\0') {
				name[iCnt] = c;			
				iCnt++;
				c = fgetc(f);
				if ( ferror(f) ) {
					free(name);
					return -1;
				}
			}

			name[iCnt] = c;
			
			//printRela
			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	//calcul number of entries

			printf("\nSection relocation \e[1;31m%s\e[0m   at offset address 0x%x contains %d entries:\n", name, a_shdr[i].sh_offset, nb_entries);
			printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
   			printf("║                                     SymbTable:                                     ║\n");
   			printf("╠═══════════╤════════════╤══════════════╤═════════════════════╤══════════════════════╣\n");
			printf("║ offset:   │    Info    │      Type    │     Symbol-value    │      Symbol-name     ║\n");
			printf("╟───────────┼────────────┼──────────────┼─────────────────────┼──────────────────────╢\n");
	
			for (k = 0; k < nb_entries; k++) {
				// get type
				type = relType[ELF32_R_TYPE(rela[j].r_info)]; 
				if (type == NULL) { 
					fprintf(stderr, "error : unknown type. Break.\n");
					free(name);
					return -1;
				}
				
				/////
				if (a_symb[ELF32_R_SYM(rela[j].r_info)].st_value != 0 ) {
					// AAA
					symb_size = get_symb_name_size(f, a_symb[ELF32_R_SYM(rela[j].r_info)].st_name, strndx);
					if (symb_size == -1) {
						symName = NULL;
					} else {
						symName = malloc( sizeof(char) * symb_size);
						if (symName == NULL) {
							fprintf(stderr, "Error : name malloc failed. Break.\n");
							free(name);
							return -1;
						}
						get_symb_name(f, a_symb[ELF32_R_SYM(rela[j].r_info)].st_name, strndx, symName, symb_size);
					}	
				} else {
					symb_size = get_symb_name_size(f, a_shdr[a_symb[ELF32_R_SYM(rel[j].r_info)].st_shndx].sh_name, shstrndx);
					if (symb_size == -1) {
						symName = NULL;
					} else {
						symName = malloc( sizeof(char) * symb_size);
						if (symName == NULL) {
							fprintf(stderr, "Error : name malloc failed. Break.\n");
							free(name);
							return -1;
						}
						get_symb_name(f, a_shdr[a_symb[ELF32_R_SYM(rela[j].r_info)].st_shndx].sh_name, shstrndx, symName, symb_size);
					}
				}	
				/////
				printf("║ %08x  │  %08x  │ %-10s  │      %08x       │     %-16s ║\n",rela[j].r_offset, rela[j].r_info, type, a_symb[ELF32_R_SYM(rela[j].r_info)].st_value, symName==NULL?"":symName);
				j++;				
			}
			printf("╚═══════════╧════════════╧══════════════╧═════════════════════╧══════════════════════╝\n");		
			free(symName);		
			free(name);	
		}		
	}	

	// Displaying Rel content
	j = 0;
	for (i = 0; i < nb_shdr; i++) {
		if (a_shdr[i].sh_type == SHT_REL) {	
			
			//getName
			if ( fseek(f, shstrndx + a_shdr[i].sh_name, SEEK_SET) == -1 ) {
				fprintf(stderr, "error while fseek\n");
				return -1;
			}

			iCnt = 0;

			c = fgetc(f);
			if ( ferror(f) ) return -1;

			while (c != '\0') {
				iCnt++;
				c = fgetc(f);		
				if ( ferror(f) ) return -1;
			}

			name = malloc( (iCnt + 1) * sizeof(char) );
			if (name == NULL) {
				return -1;	
			}

			if ( fseek(f, shstrndx + a_shdr[i].sh_name, SEEK_SET) == -1 ) {
				fprintf(stderr, "error while fseek\n");
				free(name);
				return -1;
			}

			iCnt = 0;

			c = fgetc(f);
			if ( ferror(f) ) {
				free(name);
				return -1;
			}

			while (c != '\0') {
				name[iCnt] = c;			
				iCnt++;
				c = fgetc(f);
				if ( ferror(f) ) {
					free(name);
					return -1;
				}
			}
			name[iCnt] = c;

			//printRel
			nb_entries = a_shdr[i].sh_size / a_shdr[i].sh_entsize;	//calcul de nombre d'entrée
			printf("\nSection relocation \e[1;31m%s\e[0m  at offset address 0x%x contains %d entries:\n", name, a_shdr[i].sh_offset, nb_entries);
			printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
   			printf("║                                      SymbTable:                                    ║\n");
   			printf("╠═══════════╤════════════╤══════════════╤═════════════════════╤══════════════════════╣\n");
			printf("║ offset:   │    Info    │      Type    │     Symbol-value    │      Symbol-name     ║\n");
			printf("╟───────────┼────────────┼──────────────┼─────────────────────┼──────────────────────╢\n");
	
			for(k = 0; k < nb_entries; k++) {
				// get type
				type = relType[ELF32_R_TYPE(rel[j].r_info)]; 
				if (type == NULL) { 
					fprintf(stderr, "error : unknown type. Break.\n");
					free(name);
					return -1;
				}
				/////
				if ( a_symb[ELF32_R_SYM(rel[j].r_info)].st_value != 0 ) {
					symb_size = get_symb_name_size(f, a_symb[ELF32_R_SYM(rel[j].r_info)].st_name, strndx);
					if (symb_size == -1) {
						symName = NULL;
					} else {
						symName = malloc( sizeof(char) * symb_size );
						if (symName == NULL) {
							fprintf(stderr, "Error : name malloc failed. Break.\n");
							free(name);
							return -1;
						}
						get_symb_name(f, a_symb[ELF32_R_SYM(rel[j].r_info)].st_name, strndx, symName, symb_size);
					}	
				} else {
					symb_size = get_symb_name_size(f, a_shdr[a_symb[ELF32_R_SYM(rel[j].r_info)].st_shndx].sh_name, shstrndx);
					if (symb_size == -1) {
						symName = NULL;
					} else {
						symName = malloc( sizeof(char) * symb_size );
						if (symName == NULL) {
							fprintf(stderr, "Error : name malloc failed. Break.\n");
							free(name);
							return -1;
						}
						get_symb_name(f, a_shdr[a_symb[ELF32_R_SYM(rel[j].r_info)].st_shndx].sh_name, shstrndx, symName, symb_size);
					}
				}			
				/////
				printf("║ %08x  │  %08x  │ %-10s  │      %08x       │     %-16s ║\n",rel[j].r_offset, rel[j].r_info, type, a_symb[ELF32_R_SYM(rel[j].r_info)].st_value, symName==NULL?"":symName);
				j++;				
			}
			printf("╚═══════════╧════════════╧══════════════╧═════════════════════╧══════════════════════╝\n");		
			free(symName);		
			free(name);	
		}		
	}	

	printf("\n");
	
	return 1;
	
}
