#include "projet.h"

/*
TODO :
DECOUPAGE DE TYPE UN PAR UN 
*/



void recupTabsection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t) 
{
	Elf32_Shdr section_elf;
	int i;
	
	//decomposition de la table des section pour remplir le tableau des sections
	for(i=0; i<header_elf.e_shnum; i++)
	{	

			//remplissage des différent champ de chanque en tête de section
		 	fread(&section_elf.sh_name, sizeof(uint32_t), 1, f);
			section_elf.sh_name = recupererValeurSwap32(section_elf.sh_name, header_elf.e_ident[5]);
			fread(&section_elf.sh_type, sizeof(uint32_t), 1, f);
			section_elf.sh_type = recupererValeurSwap32(section_elf.sh_type, header_elf.e_ident[5]);
			fread(&section_elf.sh_flags, sizeof(uint32_t), 1, f);
			section_elf.sh_flags = recupererValeurSwap32(section_elf.sh_flags, header_elf.e_ident[5]);
			fread(&section_elf.sh_addr, sizeof(Elf32_Addr), 1, f);
			section_elf.sh_addr = recupererValeurSwap32(section_elf.sh_addr, header_elf.e_ident[5]);
			fread(&section_elf.sh_offset, sizeof(Elf32_Off), 1, f);
			section_elf.sh_offset = recupererValeurSwap32(section_elf.sh_offset, header_elf.e_ident[5]);
			fread(&section_elf.sh_size, sizeof(uint32_t), 1, f);
			section_elf.sh_size = recupererValeurSwap32(section_elf.sh_size, header_elf.e_ident[5]);
			fread(&section_elf.sh_link, sizeof(uint32_t), 1, f);
			section_elf.sh_link = recupererValeurSwap32(section_elf.sh_link, header_elf.e_ident[5]);
			fread(&section_elf.sh_info, sizeof(uint32_t), 1, f);
			section_elf.sh_info = recupererValeurSwap32(section_elf.sh_info, header_elf.e_ident[5]);
			fread(&section_elf.sh_addralign, sizeof(uint32_t), 1, f);
			section_elf.sh_addralign = recupererValeurSwap32(section_elf.sh_addralign, header_elf.e_ident[5]);
			fread(&section_elf.sh_entsize, sizeof(uint32_t), 1, f);
			section_elf.sh_entsize = recupererValeurSwap32(section_elf.sh_entsize, header_elf.e_ident[5]);
			
			t[i] = section_elf;
	}

}


void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf){

	int i, string_size = 0;

	printf("Section Headers:\n");
	printf("[Nr]	Name			Type		Addr	Off	Size	ES	Flg	Lk	Inf	Al\n");


	for(i=0; i<header_elf.e_shnum; i++)
	{	
	
	printf("%d	", i);
	
	string_size = afficherNomSection(header_elf, section_elf, elf, section_elf[i]);

	if(string_size <= 7)
	{	
		printf("			"); 
	}
	else if(string_size <= 15)
	{ 
		printf("		"); 
	}
	else 
	{ 
		printf("	"); 
	}

	//affichage de la traducion des type de section
	switch(section_elf[i].sh_type){
		case SHT_NULL :
			printf("NULL		");
			break;
		case SHT_PROGBITS :
			printf("PROGBITS	");
			break;
		case SHT_SYMTAB :
			printf("SYMTAB		");
			break;
		case SHT_STRTAB :
			printf("STRTAB		");
			break;
		case SHT_RELA :
			printf("RELA		");
			break;
		case SHT_HASH :
			printf("HASH		");
			break;
		case SHT_DYNAMIC :
			printf("DYNAMIC		");
			break;
		case SHT_NOTE :
			printf("NOTE		");
			break;
		case SHT_NOBITS :
			printf("NOBITS		");
			break;
		case SHT_REL :
			printf("REL		");
			break;
		case SHT_SHLIB :
			printf("SHLIB		");
			break;
		case SHT_DYNSYM :
			printf("DYNSYM		");
			break;
		case SHT_LOPROC :
			printf("LOPROC		");
			break;
		case SHT_HIPROC :
			printf("HIPROC		");
			break;
		case SHT_LOUSER :
			printf("LOUSER		");
			break;
		case SHT_HIUSER :
			printf("HIUSER		");
			break;
		default :
			printf("Erreur		");
			break;

	};
	
	//affichage addresse de début, l'endroit du fichier où sont les section, leurs taile en octet
	printf("%d	", section_elf[i].sh_addr);
	printf("%x	", section_elf[i].sh_offset);
	printf("%x	", section_elf[i].sh_size);
	printf("%x	", section_elf[i].sh_entsize);


	//affichage des drapeaux
	// WRITE FLAG
	if(section_elf[i].sh_flags & 0x1)
	{
		printf("W");
	}
	// ALLOC FLAG
	if(section_elf[i].sh_flags & 0x2) 
	{
		printf("A");
	}
	// EXECINSTR
	if(section_elf[i].sh_flags & 0x4)
	{
		printf("X");
	}
	// MASKPROC
	if(section_elf[i].sh_flags & 0xf0000000)
	{
		printf("M");
	}

	printf("	");
	

	//affichage des derniers champs
	printf("%d	", section_elf[i].sh_link);
	printf("%d	", section_elf[i].sh_info);
	printf("%d	", section_elf[i].sh_addralign);


	printf("\n");

	
	}

	//lègende des drapeaux
	printf("Key to Flags:\n");
 	printf("W (write), A (alloc), X (execute), M (merge), ? (unknown)");



}


