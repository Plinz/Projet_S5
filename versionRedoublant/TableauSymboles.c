#include "projet.h"


int getTableauSymboles(FILE* file, Elf32_Ehdr header_elf, Elf32_Shdr *section_elf, Elf32_Sym **t) 
{
	Elf32_Sym symbole_elf;
	Elf32_Sym *table;
	int i, j=0;	

	// parcours sections
	for(i=0; i<(header_elf.e_shnum); i++)
	{

		// si section est de type table de symboles
		if(section_elf[i].sh_type == SHT_SYMTAB)
		{
			// curseur debut de la section
			fseek(file, section_elf[i].sh_offset, SEEK_SET);
			
			// allocation du pointeur structure de donnée utilisée pour la table des symboles
			*t = malloc(section_elf[i].sh_size);
			
			// cas où pointeur est nul
			if (*t==NULL){
				printf("problème d'allocation mémoire de la table des symbole");
				exit (1);
			}
			
			// allocation de la structure de donnée utilisée pour la table des symboles
			table = malloc(section_elf[i].sh_size);
			

			// cas où pointeur est nul
			if (table==NULL){
				printf("problème d'allocation mémoire de la table des symbole");
				exit (1);
			}
			
			//
           		 table=*t;

			// parcours chaque entrees de symbole
			for(j=0; j<((section_elf[i].sh_size) / (section_elf[i].sh_entsize)); j++)
			{
					//remplissage des champs de la table des symboles
					fread(&symbole_elf.st_name, sizeof(uint32_t), 1, file);
					symbole_elf.st_name = recupererValeurSwap32(symbole_elf.st_name, header_elf.e_ident[5]);
					fread(&symbole_elf.st_value, sizeof(uint32_t), 1, file);
					symbole_elf.st_value = recupererValeurSwap32(symbole_elf.st_value, header_elf.e_ident[5]);
					fread(&symbole_elf.st_size, sizeof(uint32_t), 1, file);
					symbole_elf.st_size = recupererValeurSwap32(symbole_elf.st_size, header_elf.e_ident[5]);
					fread(&symbole_elf.st_info, sizeof(unsigned char), 1, file);
					symbole_elf.st_info = recupererValeurSwap16(symbole_elf.st_info, header_elf.e_ident[5]);
					fread(&symbole_elf.st_other, sizeof(unsigned char), 1, file);
					symbole_elf.st_other = recupererValeurSwap16(symbole_elf.st_other, header_elf.e_ident[5]);
					fread(&symbole_elf.st_shndx, sizeof(uint16_t), 1, file);
					symbole_elf.st_shndx = recupererValeurSwap16(symbole_elf.st_shndx, header_elf.e_ident[5]);

					table[j] = symbole_elf;
			}
		}

	}
	
	return j;
}


void affichageSymbole(FILE* elf, Elf32_Ehdr header_elf, Elf32_Sym *symboltable, Elf32_Shdr *section_elf, int size){

	int i;


	printf("Symbol Table Entries:\n");
	printf("[Nr]	Value		Size		Type		Bind		Vis		Ndx	Name\n");

	for(i=0; i<size; i++)
	{

		printf("[%d]	", i);

		//affichage de la valeur et la taille
		printf("%x		", symboltable[i].st_value);
		
		printf("%d		", symboltable[i].st_size);

		//traduction des info sur chaque symbole
		switch(ELF32_ST_TYPE(symboltable[i].st_info)){
			case STT_NOTYPE :
				printf("NOTYPE");
				break;
			case STT_OBJECT :
				printf("OBJECT");
				break;
			case STT_FUNC:
				printf("FUNC");
				break;
			case STT_SECTION :
				printf("SECTION");
				break;
			case STT_FILE :
				printf("FILE");
				break;
			case STT_LOPROC :
				printf("LOPROC");
				break;
			case STT_HIPROC :
				printf("STT_HIPROC");
				break;
			default :
				printf("?");
				break;
		}

		printf("		");

		switch(ELF32_ST_BIND(symboltable[i].st_info)){
			case STB_LOCAL :
				printf("LOCAL");
				break;
			case STB_GLOBAL:
				printf("GLOBAL");
				break;
			case STB_WEAK:
				printf("WEAK");
				break;
			case STB_LOPROC :
				printf("LOPROC");
				break;
			case STB_HIPROC:
				printf("HIPROC");
				break;
			default :
				printf("?");
				break;
		}	

		printf("		");

		//traduction de la fin du reste des info
		switch(symboltable[i].st_other){
			case STV_DEFAULT :
				printf("DEFAULT");
				break;
			case STV_INTERNAL :
				printf("INTERNAL");
				break;
			case STV_HIDDEN :
				printf("HIDDEN");
				break;
			case STV_PROTECTED:
				printf("PROTECTED");
				break;
			default :
				printf("?");
				break;

		}

		printf("		");

		printf("%d	", symboltable[i].st_shndx);

		// il représente l'indice de la table des chaînes qui donne son nom au symbole.
		afficherNomSection(header_elf, section_elf, elf, section_elf[i]);

		printf("\n");
	}

}






