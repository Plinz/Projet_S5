#include "elf_reader.h"


/*
/	Fonction qui parcour la table des sections d'un fichier elf et l'ecrit dans un argument passe par pointeur
/	Arguments:
/		-elf 			:	Fichier elf
/		-header_elf	:	Header du fichier elf
/		-t 			:	Tableau qui va contenir la table des sections
*/
void lectureTableSection(FILE* elf, Elf32_Ehdr header_elf, Elf32_Shdr *sections_table) 
{
	Elf32_Shdr section_header;
	int i;
	
	//decomposition de la table des section pour remplir le tableau des sections
	for(i=0; i<header_elf.e_shnum; i++)
	{	

			//remplissage des différent champ de chanque en tête de section
		 	fread(&section_header.sh_name, sizeof(uint32_t), 1, elf);
			section_header.sh_name = Swap32(section_header.sh_name, header_elf.e_ident[5]);
			fread(&section_header.sh_type, sizeof(uint32_t), 1, elf);
			section_header.sh_type = Swap32(section_header.sh_type, header_elf.e_ident[5]);
			fread(&section_header.sh_flags, sizeof(uint32_t), 1, elf);
			section_header.sh_flags = Swap32(section_header.sh_flags, header_elf.e_ident[5]);
			fread(&section_header.sh_addr, sizeof(Elf32_Addr), 1, elf);
			section_header.sh_addr = Swap32(section_header.sh_addr, header_elf.e_ident[5]);
			fread(&section_header.sh_offset, sizeof(Elf32_Off), 1, elf);
			section_header.sh_offset = Swap32(section_header.sh_offset, header_elf.e_ident[5]);
			fread(&section_header.sh_size, sizeof(uint32_t), 1, elf);
			section_header.sh_size = Swap32(section_header.sh_size, header_elf.e_ident[5]);
			fread(&section_header.sh_link, sizeof(uint32_t), 1, elf);
			section_header.sh_link = Swap32(section_header.sh_link, header_elf.e_ident[5]);
			fread(&section_header.sh_info, sizeof(uint32_t), 1, elf);
			section_header.sh_info = Swap32(section_header.sh_info, header_elf.e_ident[5]);
			fread(&section_header.sh_addralign, sizeof(uint32_t), 1, elf);
			section_header.sh_addralign = Swap32(section_header.sh_addralign, header_elf.e_ident[5]);
			fread(&section_header.sh_entsize, sizeof(uint32_t), 1, elf);
			section_header.sh_entsize = Swap32(section_header.sh_entsize, header_elf.e_ident[5]);
			
			sections_table[i] = section_header;
	}

}

/*
/	Fonction d'affichage du nom d'une section
/	Arguments:
/		-elf 			:	Fichier elf
/		-header_elf			:	Header du fichier elf
/		-sections_table	:	La table des sections
/		-headerCourant	:	La section dont le nom va etre affiche
*/
int afficherNomSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, Elf32_Shdr headerCourant){
	char courant;
	int offset;
	int i = 0;
	
	offset = sections_table[header_elf.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections
	fseek(elf, offset + headerCourant.sh_name, SEEK_SET); //offset nous ammène au début du stockage des noms, et sh_name contient l'index du nom de la section

	//On ecrit alors le nom de la section char par char jusqu'à tomber sur le char de fin
	courant = fgetc(elf);
	while(courant != '\0'){
		i++;
		printf("%c", courant);
		courant = fgetc(elf);
	}
	return i; // en retournant la taille du nom, on pourra mieux gérer l'affichage
}



/*
/	Fonction d'affichage de la table des sections
/	Arguments:
/		-elf 			:	Fichier elf
/		-header_elf		:	Header du fichier elf
/		-sections_table	:	La table des sections
*/
void affichageTabsection(Elf32_Shdr *sections_table, Elf32_Ehdr header_elf, FILE* elf){

	int i, string_size = 0;

	printf("Section Headers:\n");
	printf("[Nr]	Name			Type			Addr	Off	Size	ES	Flg	Lk	Inf	Al\n");


	for(i=0; i<header_elf.e_shnum; i++)
	{	
		
		printf("%d	", i);
		
		string_size = afficherNomSection(header_elf, sections_table, elf, sections_table[i]);

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
		switch(sections_table[i].sh_type){
			case SHT_NULL :
				printf("NULL			");
				break;
			case SHT_PROGBITS :
				printf("PROGBITS		");
				break;
			case SHT_SYMTAB :
				printf("SYMTAB			");
				break;
			case SHT_STRTAB :
				printf("STRTAB			");
				break;
			case SHT_RELA :
				printf("RELA			");
				break;
			case SHT_HASH :
				printf("HASH			");
				break;
			case SHT_DYNAMIC :
				printf("DYNAMIC			");
				break;
			case SHT_NOTE :
				printf("NOTE			");
				break;
			case SHT_NOBITS :
				printf("NOBITS			");
				break;
			case SHT_REL :
				printf("REL			");
				break;
			case SHT_SHLIB :
				printf("SHLIB			");
				break;
			case SHT_DYNSYM :
				printf("DYNSYM			");
				break;
			case SHT_INIT_ARRAY :
				printf("INIT_ARRAY		");
				break;
			case SHT_FINI_ARRAY :
				printf("FINI_ARRAY		");
				break;
			case SHT_PREINIT_ARRAY :
				printf("PREINIT_ARRAY 	");
				break;
			case SHT_GROUP :
				printf("GROUP			");
				break;
			case SHT_SYMTAB_SHNDX :
				printf("SYMTAB_SHNDX	");
				break;
			case SHT_NUM :
				printf("NUM				");
				break;
			case SHT_ARM_ATTRIBUTES :
				printf("ARM_ATTRIBUTES		");
				break;
			case SHT_ARM_EXIDX:
				printf("ARM_EXIDX		");
				break;
			case SHT_ARM_PREEMPTMAP  :
				printf("ARM_PREEMPTMAP	");
				break;
			case SHT_GNU_verdef :
				printf("VERDEF			");
				break;
			case SHT_GNU_verneed :
				printf("VERNEED 		");
				break;
			case SHT_GNU_versym :
				printf("VERSYM			");
				break;
			case SHT_LOOS :
				printf("LOOS			");
				break;
			case SHT_LOPROC :
				printf("LOPROC			");
				break;
			case SHT_HIPROC :
				printf("HIPROC			");
				break;
			case SHT_LOUSER :
				printf("LOUSER			");
				break;
			case SHT_HIUSER :
				printf("HIUSER			");
				break;
			default :
				printf("Inconnu			");
				break;

		};
		
		//affichage addresse de début, l'endroit du fichier où sont les section, leurs taile en octet
		printf("%x	", sections_table[i].sh_addr);
		printf("%x	", sections_table[i].sh_offset);
		printf("%x	", sections_table[i].sh_size);
		printf("%x	", sections_table[i].sh_entsize);




		//affichage des drapeaux
		int inconnu = 1;
		// WRITE FLAG
		if(sections_table[i].sh_flags & 0x1)
		{
			printf("W");
			inconnu =0;
		}
		// ALLOC FLAG
		if(sections_table[i].sh_flags & 0x2) 
		{
			printf("A");
			inconnu =0;
		}
		// EXECINSTR
		if(sections_table[i].sh_flags & 0x4)
		{
			printf("X");
			inconnu =0;
		}
		// MERGE
		if(sections_table[i].sh_flags & 0x10)
		{
			printf("M");
			inconnu =0;
		}
		// STRINGS
		if(sections_table[i].sh_flags & 0x20)
		{
			printf("S");
			inconnu =0;
		}
		// INFO_LINK 
		if(sections_table[i].sh_flags & 0x40)
		{
			printf("I");
			inconnu =0;
		}
		// LINK_ORDER 
		if(sections_table[i].sh_flags & 0x80)
		{
			printf("L");
			inconnu =0;
		}
		// OS_NONCONFORMING 
		if(sections_table[i].sh_flags & 0x100)
		{
			printf("O");
			inconnu =0;
			// MASKPROC
			if(sections_table[i].sh_flags & 0xf0000000)
			{
				printf("p");
			}
			// MASKOS
			if(sections_table[i].sh_flags & 0x0ff00000)
			{
				printf("o");
			}			
		}
		// GROUP 
		if(sections_table[i].sh_flags & 0x200)
		{
			printf("G");
			inconnu =0;
		}
		// TLS  
		if(sections_table[i].sh_flags & 0x400)
		{
			printf("T");
			inconnu =0;
		}
		// ORDERED 
		if(sections_table[i].sh_flags & 0x40000000)
		{
			printf("O");
			inconnu =0;
		}
		// EXCLUDE  
		if(sections_table[i].sh_flags & 0x80000000)
		{
			printf("E");
			inconnu =0;
		}
		if (sections_table[i].sh_flags == 0)
		{
			inconnu =0;
		}
		if(inconnu) {
			printf("x");
		}

		printf("	");
		

		//affichage des derniers champs
		printf("%d	", sections_table[i].sh_link);
		printf("%d	", sections_table[i].sh_info);
		printf("%d	", sections_table[i].sh_addralign);


		printf("\n");

	
	}

	//lègende des drapeaux
	printf("Key to Flags:\n");
 	printf("W (write), A (alloc), X (execute), M (merge), S (string), I (info link), L (link order), G (group), O (ordered), T (TLS), E (exclude), O (os non conforming) p (proc), o (os)x (unknown)");



}