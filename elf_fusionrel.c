#include "elf_reader.h"

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


int relsize(FichierElf * elfile) {
	// Calcul de la taille de Rel - sert pour le malloc ??
	
	Elf32_Ehdr header = elfile->header_elf;
	Elf32_Shdr* section_headers = elfile->sectionsTable;
	

	int nb_sec = header.e_shnum;
	int taille = 0; 	
	int nombre = 0;
	for (int i = 0; i < nb_sec; i++) {
		// Pour chacun des sections du programme, si on a un type Rela :
		if (section_headers[i].sh_type == SHT_REL) {
			// On divise la taille de la section par la taille de chaque entrée pour connaitre leur nombre
			nombre = section_headers[i].sh_size / section_headers[i].sh_entsize;	
			taille += nombre;
		}
	} 
	return taille;
}

Reloctable* crea_rel_table (FichierElf * elfile) {
	// Creation de la table de REL
	// Avant : malloc le elf32_Rel de elfile en utilisant relsize.
	Elf32_Ehdr header = elfile->header_elf;
	Elf32_Shdr* section_headers = elfile->sectionsTable;
	Elf32_Rel* lesrel = elfile->tabRel;
	FILE *elf = elfile->fichierElf;
	Reloctable* reltab;  
	int nb_sec = header.e_shnum;	
	int nombre = 0;
	int trel = sizeof(Elf32_Rel);
	int k=0;
	int diffsec = 0;
	for (int i=0; i < nb_sec; i++){
		if (section_headers[i].sh_type == SHT_REL) {
			fseek(elf, section_headers[i].sh_offset, SEEK_SET);
			nombre = section_headers[i].sh_size / section_headers[i].sh_entsize;
			reltab[diffsec].tablerel = lesrel[k];
			reltab[diffsec].indice_section = i;
			for (int j=0; j < nombre; j++) {
				fread(&(lesrel[k]),trel, 1,elf);
				k++;
			}
			diffsec ++;
		}

	}
	return reltab;
}




/*
Section RelFusion(fichierElf* elf1, fichierElf* elf2, Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2) {
// Si deux sections ont le même nom : On concat

}

*/
Section RelUpdate (FichierElf* oldelf, FichierElf* newelf, Reloctable reltab, int nbnewsymbole) {
// Si une section a besoin d'être mise a jour suite à la fusion. 
// Appel de type reloctable[i] ?? On va tester ça.


Elf32_Ehdr oldheader = oldelf->header_elf;
Elf32_Shdr* oldsection_headers = oldelf->sectionsTable;
Elf32_Rel* oldlesrel = oldelf->tabRel;
Elf32_Sym *oldsym = oldelf->tabSymbole;
Elf32_Ehdr newheader = newelf->header_elf;
Elf32_Shdr* newsection_headers = newelf->sectionsTable;
Elf32_Rel* newlesrel = newelf->tabRel;
Elf32_Sym *newsym = newelf->tabSymbole;

int nombre = 0;
int numsec = reltab.indice_section;
int type = 0;
int idsym = 0;
int j=0;
int found = 0;
nombre = oldsection_headers[numsec].sh_size / oldsection_headers[numsec].sh_entsize;

	for (int i = 0; i<nombre ; i++) {
	// Calcul de l'offset de reimplementation
		// tabRel.tablerel[i]. 

	// Calcul du numero de symbole a partir de la nouvelle table des symboles.
	// Calcul du truc pour les sections. 
		type  = ELF32_R_TYPE(oldlesrel[i].r_info);
		idsym = ELF32_R_SYM(oldlesrel[i].r_info);
		switch (type) {
			case (2) : 
				// R_ARM_ABS32
				break;
			case (5) :
				// R_ARM_ABS16
				break;
			case (6) :
				// R_ARM_ABS12
				break;
			case (8) :
				// R_ARM_ABS8
				break;
			
			case (29) :
				// R_ARM_JUMP24
				break;

			case (28) :
				// R_ARM_CALL
				break;

			default :
				// On fait symbole :D 
				j = 0;
				found = 0;
				
				while (j < nbnewsymbole && found == 0 ) {
					if ( ( oldsym[idsym].st_value == newsym[j].st_value ) &&( oldsym[idsym].st_size == newsym[j].st_size ) &&( oldsym[idsym].st_info == newsym[j].st_info ) ) {
						// &&( oldsym[idsym].st_shdnx == newsym[j].st_shdnx )  aussi mais ça marche po :(
						newlesrel[i].r_info = ELF32_R_INFO(j,type);
						found = 1;
						
						}

					j++;
				} 
				break ;

		
		}
	}
}
