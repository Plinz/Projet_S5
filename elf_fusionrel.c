#include "elf_reader.h"
#include "elf_fusion.h"

/*const char* relType[] =
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
	};*/


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

int init_new_rel (FichierElf new_elf, FichierElf oldelf1, FichierElf oldelf2) {

int nb_sec1 = oldelf1.header_elf.e_shnum;
int nb_sec2 = oldelf1.header_elf.e_shnum ;
int nb_rel = 0 ;
Reloctable * reltab;
for (int i=0; i<nb_sec1; i++) {
	if ( oldelf1.sectionsTable[i].sh_type == SHT_REL ) { nb_rel ++ ; } 
}
for (int i=0; i<nb_sec2; i++) {
	if ( oldelf2.sectionsTable[i].sh_type == SHT_REL ) { nb_rel ++ ; } 
}

reltab = malloc(sizeof(Reloctable) * nb_rel);
if ( reltab == NULL ) { return -1 ;} 
else { new_elf.tabRel = reltab; return 1 ; } 


}

Reloctable* crea_rel_table (FichierElf * elfile) {
	// Creation de la table de REL
	// Avant : malloc le elf32_Rel de elfile en utilisant relsize.
	Elf32_Ehdr header = elfile->header_elf;
	Elf32_Shdr* section_headers = elfile->sectionsTable;
	// Faut-il le malloc ???
	Reloctable* tabrel = elfile->tabRel;
	FILE *elf = elfile->fichierElf;

	int nb_sec = header.e_shnum;	
	int nombre = 0;
	int trel = sizeof(Elf32_Rel);
	int k=0;
	int diffsec = 0;
	for (int i=0; i < nb_sec; i++){
		if (section_headers[i].sh_type == SHT_REL) {
			fseek(elf, section_headers[i].sh_offset, SEEK_SET);
			nombre = section_headers[i].sh_size / section_headers[i].sh_entsize;
			
			tabrel[diffsec].indice_section = i;
			tabrel[diffsec].nombre_relocation = nombre;
			tabrel[diffsec].nom_section = getSectionName(section_headers[i],elfile);
			for (int j=0; j < nombre; j++) {
				fread(&(tabrel[diffsec].tablerel[k]),trel, 1,elf);
				k++;
			}
			diffsec ++;
		}


	}	
	return tabrel;
}





Section RelFusion(FichierElf* oldelf1, Elf32_Shdr OldSec1, Elf32_Shdr OldSec2, FichierElf* oldelf2, FichierElf * newelf, int nbnewsymbole) {
// Si deux sections ont le même nom : On concatene tout ca.
Elf32_Ehdr oldheader1 = oldelf1->header_elf;
Elf32_Shdr* oldsection_headers1 = oldelf1->sectionsTable;
Reloctable * oldlesrel1 = oldelf1->tabRel;
Elf32_Sym *oldsym1 = oldelf1->tabSymbole;
Elf32_Ehdr oldheader2 = oldelf2->header_elf;
Elf32_Shdr* oldsection_headers2 = oldelf2->sectionsTable;
Reloctable* oldlesrel2 = oldelf2->tabRel;
Elf32_Sym *oldsym2 = oldelf2->tabSymbole;
Elf32_Ehdr newheader = newelf->header_elf;
Elf32_Shdr* newsection_headers = newelf->sectionsTable;
Reloctable * newlesrel = newelf->tabRel;
Elf32_Sym *newsym = newelf->tabSymbole;

int parcoureltab1 = 0 ;
// A VERIFIER LA BOUCLE
while (strcmp(oldlesrel1[parcoureltab1].nom_section,getSectionName(OldSec1,oldelf1))==0){parcoureltab1++;}

int parcoureltab2 = 0 ;
while (strcmp(oldlesrel2[parcoureltab2].nom_section,getSectionName(OldSec2,oldelf2))==0){parcoureltab2++;}
// Verifier la valeur de parcoureltab si on a i=0... Pour pas que ça prenne un de plus

int indice_new_rel = 0;
while ( newlesrel[indice_new_rel].indice_section != -1 ) { indice_new_rel ++ ; }









int nombre1 = 0;

int numsec1 = oldlesrel1[parcoureltab1].indice_section;
int nombre2 = 0;
int numsec2 = oldlesrel2[parcoureltab2].indice_section;
int indicenew = 0;
nombre1 = oldsection_headers1[numsec1].sh_size / oldsection_headers1[numsec1].sh_entsize;
nombre2 = oldsection_headers2[numsec2].sh_size / oldsection_headers2[numsec2].sh_entsize;
int type = 0;
int idsym = 0;
int j = 0;
int found = 0 ;
int lastoff = 0;
int taille = 0;
Section secrel;

// Somme du nombre de relocation
newlesrel[indice_new_rel].nombre_relocation = oldlesrel1[parcoureltab1].nombre_relocation + oldlesrel2[parcoureltab2].nombre_relocation;


//Nom de section inchangé 
newlesrel[indice_new_rel].nom_section = oldlesrel1[parcoureltab1].nom_section;

//Calcul du nouvel indice de section
int nouvel_ind_sec = 0;
while (strcmp(oldlesrel1[parcoureltab1].nom_section,getSectionName(newsection_headers[nouvel_ind_sec],newelf))==0) {
	nouvel_ind_sec ++;
	if (nouvel_ind_sec > 100 ) { printf ("Boucle dans le vide");  } } 
	newlesrel[indice_new_rel].indice_section = nouvel_ind_sec ;
// VERIFIER la valeur de nouvel_ind_sec 

for (int i = 0; i<nombre1 ; i++ ) {
// On s'occupe de la premiere table des relocations

	// On calcule le nouvel offset
	newlesrel[indice_new_rel].tablerel[indicenew].r_offset = oldlesrel1[parcoureltab1].tablerel[i].r_offset;

	// On enregistre le type et l'ID précédent.
	type  = ELF32_R_TYPE(oldlesrel1[parcoureltab1].tablerel[i].r_info);
	idsym = ELF32_R_SYM(oldlesrel1[parcoureltab1].tablerel[i].r_info);


	// On mets le type & le nouvel id
	j = 0;
	found = 0;

	while (j < nbnewsymbole && found == 0 ) {
		if ( ( oldsym1[idsym].st_value == newsym[j].st_value ) &&( oldsym1[idsym].st_size == newsym[j].st_size ) &&( oldsym1[idsym].st_info == newsym[j].st_info ) ) {
			newlesrel[indice_new_rel].tablerel[i].r_info = ELF32_R_INFO(j,type);
			found = 1;

			}

		j++;}

	// On stocke le dernier offset
	lastoff = newlesrel[indice_new_rel].tablerel[i].r_offset;

	switch (type) {
				// Que faire si on entre dans ces types ?
				// Prendre la section en question et prendre les 4 premiers octets
				// Les modifier dans le fichier ??
				// Il faut ajouter pour chacun l'offset de la table des symboles
				// plus le ndx du symbole en
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
				// R_ARM_CALL -- PAS SUR DU TOUT CELUI LA
				break;

			default :
				break ;


		}


	// On incremente le nouvel indice que prendront la prochaine table
	indicenew ++;
}

for (int i = 0; i<nombre2 ; i ++ ) {
// On concatene la seconde table des relocations

	// On calcule le nouvel offset
	newlesrel[indice_new_rel].tablerel[indicenew].r_offset = lastoff + oldsection_headers2[numsec2].sh_entsize;



	// On enregistre le type et l'ID précédent.
	type  = ELF32_R_TYPE(oldlesrel2[parcoureltab2].tablerel[i].r_info);
	idsym = ELF32_R_SYM(oldlesrel2[parcoureltab2].tablerel[i].r_info);

	// On mets le type & le nouvel id
	j = 0;
	found = 0;

	while (j < nbnewsymbole && found == 0 ) {
		if ( ( oldsym2[idsym].st_value == newsym[j].st_value ) &&( oldsym2[idsym].st_size == newsym[j].st_size ) &&( oldsym2[idsym].st_info == newsym[j].st_info ) ) {
			newlesrel[indice_new_rel].tablerel[indicenew].r_info = ELF32_R_INFO(j,type);
			found = 1;

			}

		j++;}


	// On stocke le dernier offset
	lastoff = newlesrel[indice_new_rel].tablerel[indicenew].r_offset;

		switch (type) {
				// Que faire si on entre dans ces types ?
				// Prendre la section en question et prendre les 4 premiers octets
				// Les modifier dans le fichier ??
				// Il faut ajouter pour chacun l'offset de la table des symboles
				// plus le ndx du symbole en
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
				// R_ARM_CALL -- PAS SUR DU TOUT CELUI LA
				break;

			default :
				break ;


		}

	// On incremente le nouvel indice que prendront la prochaine table
	indicenew ++;


}
	taille = relsize(newelf);
	// EcritureStruct(newreltab[indicenew].tablerel,taille,&secrel,newsection_headers);
	return secrel;
}




Section RelUpdate (FichierElf* oldelf, FichierElf* newelf, Elf32_Shdr OldSec, int nbnewsymbole) {
// Si une section a besoin d'être mise a jour suite à la fusion. 
// Appel de type reloctable[i] ?? On va tester ça.


Elf32_Ehdr oldheader = oldelf->header_elf;
Elf32_Shdr* oldsection_headers = oldelf->sectionsTable;
Reloctable* oldlesrel = oldelf->tabRel;
Elf32_Sym *oldsym = oldelf->tabSymbole;
Elf32_Ehdr newheader = newelf->header_elf;
Elf32_Shdr* newsection_headers = newelf->sectionsTable;
Reloctable* newlesrel = newelf->tabRel;
Elf32_Sym *newsym = newelf->tabSymbole;

int parcoureltab = 0 ;
// A VERIFIER LA BOUCLE
while (strcmp(oldlesrel[parcoureltab].nom_section,getSectionName(OldSec,oldelf))==0){parcoureltab++;}

int indice_new_rel = 0;
while ( newlesrel[indice_new_rel].indice_section != -1 ) { indice_new_rel ++ ; }

int nombre = 0;
// Verifier la valeur de parcoureltab si on a i=0... Pour pas que ça prenne un de plus
int numsec = oldlesrel[parcoureltab].indice_section;
int type = 0;
int idsym = 0;
int j=0;
int found = 0;
// Inutile ??
nombre = oldsection_headers[numsec].sh_size / oldsection_headers[numsec].sh_entsize;

// Nombre de relocation inchangé
newlesrel[indice_new_rel].nombre_relocation = oldlesrel[parcoureltab].nombre_relocation;

//Nom de section inchangé 
newlesrel[indice_new_rel].nom_section = oldlesrel[parcoureltab].nom_section;

//Calcul du nouvel indice de section
int nouvel_ind_sec = 0;
while (strcmp(oldlesrel[parcoureltab].nom_section,getSectionName(newsection_headers[nouvel_ind_sec],newelf))==0) {
	nouvel_ind_sec ++;
	if (nouvel_ind_sec > 100 ) { printf ("Boucle dans le vide");  } } 
	newlesrel[indice_new_rel].indice_section = nouvel_ind_sec ;
// VERIFIER la valeur de nouvel_ind_sec 

	for (int i = 0; i<nombre ; i++) {
	// Calcul de l'offset de reimplementation
	// On va dire que l'offset ne change pas quand on ne fusionne pas 2 tables de relocation
		newlesrel[indice_new_rel].tablerel[i].r_offset = oldlesrel[parcoureltab].tablerel[i].r_offset;

	// Calcul du numero de symbole a partir de la nouvelle table des symboles.
	// Calcul du truc pour les sections. 
		type  = ELF32_R_TYPE(oldlesrel[parcoureltab].tablerel[i].r_info);
		idsym = ELF32_R_SYM(oldlesrel[parcoureltab].tablerel[i].r_info);
		
				// On fait symbole :D 
				j = 0;
				found = 0;
				
				while (j < nbnewsymbole && found == 0 ) {
					if ( ( oldsym[idsym].st_value == newsym[j].st_value ) &&( oldsym[idsym].st_size == newsym[j].st_size ) &&( oldsym[idsym].st_info == newsym[j].st_info ) ) {
						newlesrel[indice_new_rel].tablerel[i].r_info = ELF32_R_INFO(j,type);
						found = 1;
						
						}

					j++;
				} 
			switch (type) {
				// Que faire si on entre dans ces types ?
				// Prendre la section en question et prendre les 4 premiers octets
				// Les modifier dans le fichier ??
				// Il faut ajouter pour chacun l'offset de la table des symboles
				// plus le ndx du symbole en
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
				// R_ARM_CALL -- PAS SUR DU TOUT CELUI LA
				break;

			default :
				break ;

		
		}
	}
}

void EcritureStruct (Elf32_Rel *tabrel, int taillerel, Section *section, Elf32_Shdr* headsec) {
	int sh_entsize = headsec->sh_entsize;
	int i,j;
	int nbOctets = 0;


	section->contenu = malloc(taillerel * sh_entsize);
	char * buffer = malloc(1);
	for(i=0; i<taillerel;i++){
		buffer = (char *) &tabrel[i];
		for (j=0; j < sh_entsize; j++) {
			section->contenu[nbOctets] = *buffer;
			buffer++;
			nbOctets++;
		}
	}
	section->nbOctets = nbOctets;
	section->header = *headsec;

}
/*
  Structure: Cette structure est crée pour chaque nouvelle section après la fusion
    contenu: contenu brut de la section après fusion
    nbOctets: taille de la section créée
  Utile pour construire de manière générique le nouveau fichier objet

typedef struct section {
 char * contenu;
 int nbOctets;
 Elf32_Shdr header;
} Section;
*/
