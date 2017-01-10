#include "elf_reader.h"

Elf32_Ehdr lectureheader(FILE* f){
	Elf32_Ehdr header_elf;
	int i;

	//lecture du numéro magique	
	for(i = 0; i<16;i++){
		fread(&header_elf.e_ident[i], 1, 1, f); 
	}

		//remplissage des champs de la structure de l'en tête  
		fread(&header_elf.e_type, sizeof(uint16_t), 1, f);
		header_elf.e_type = Swap16(header_elf.e_type, header_elf.e_ident[5]);
		fread(&header_elf.e_machine, sizeof(uint16_t), 1, f);
		header_elf.e_machine = Swap16(header_elf.e_machine, header_elf.e_ident[5]);
		fread(&header_elf.e_version, sizeof(uint32_t), 1, f);
		header_elf.e_version = Swap32(header_elf.e_version, header_elf.e_ident[5]);
		fread(&header_elf.e_entry, sizeof(Elf32_Addr), 1, f);
		header_elf.e_entry = Swap32(header_elf.e_entry, header_elf.e_ident[5]);
		fread(&header_elf.e_phoff, sizeof(Elf32_Off), 1, f);
		header_elf.e_phoff = Swap32(header_elf.e_phoff, header_elf.e_ident[5]);
		fread(&header_elf.e_shoff, sizeof(Elf32_Off), 1, f);
		header_elf.e_shoff = Swap32(header_elf.e_shoff, header_elf.e_ident[5]);
		fread(&header_elf.e_flags, sizeof(uint32_t), 1, f);
		header_elf.e_flags = Swap32(header_elf.e_flags, header_elf.e_ident[5]);
		fread(&header_elf.e_ehsize, sizeof(uint16_t), 1, f);
		header_elf.e_ehsize = Swap16(header_elf.e_ehsize, header_elf.e_ident[5]);
		fread(&header_elf.e_phentsize, sizeof(uint16_t), 1, f);
		header_elf.e_phentsize = Swap16(header_elf.e_phentsize, header_elf.e_ident[5]);
		fread(&header_elf.e_phnum, sizeof(uint16_t), 1, f);
		header_elf.e_phnum = Swap16(header_elf.e_phnum, header_elf.e_ident[5]);
		fread(&header_elf.e_shentsize, sizeof(uint16_t), 1, f);
		header_elf.e_shentsize = Swap16(header_elf.e_shentsize, header_elf.e_ident[5]);
		fread(&header_elf.e_shnum, sizeof(uint16_t), 1, f);
		header_elf.e_shnum = Swap16(header_elf.e_shnum, header_elf.e_ident[5]);
		fread(&header_elf.e_shstrndx, sizeof(uint16_t), 1, f);
		header_elf.e_shstrndx = Swap16(header_elf.e_shstrndx, header_elf.e_ident[5]);
	
	
	return header_elf;
}

void affichageheader(Elf32_Ehdr header_elf){
	printf("ELF Header:\n");
	affichageMagicClasseEndianABIType(header_elf);
	affichageMachine(header_elf);
	affichageOthers(header_elf);
}

void affichageNumMagic(Elf32_Ehdr header_elf){
	char classe[3][6] = {"Aucun", "ELF32", "ELF64"};
	char endian[3][6] = {"Aucun", "LSB", "MSB"};
	char abi[20][15] = {"UNIX System V", "HP-UX", "NetBSD", "Linux", "Sun Solaris", "IBM AIX", "SGI IRIX", "FreeBSD", "Compaq TRU64", "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS", "Fenix OS", "CloudABI", "ARM Eabi", "Sortix", "ARM", "Standalone"};
	int c = header_elf.e_ident[4];
	int e = header_elf.e_ident[5];
	int a = header_elf.e_ident[7];
	printf("Magic: ");
	for(int i=0; i<16; i++){
		if(header_elf.e_ident[i]/16 == 0)
			printf(" 0%x\n", header_elf.e_ident[i]);
		else
			printf(" %x\n",header_elf.e_ident[i]);
	}
	printf("Classe: ");
	if (c < 3 && c > -1)
		printf("%s\n", classe[c]);
	else	
		printf("Erreur\n");

	printf("Endianness: ");
	if (e < 3 && c > -1)
		printf("%s\n", endian[e]);
	else
		printf("Erreur\n");

	printf("ABI: ");
	if (a < 20 && c > -1)
		printf("%s\n", abi[a]);
	else
		printf("Erreur\n");

}

void affichageMagicClasseEndianABIType(Elf32_Ehdr header_elf){
	char classe[3][6] = {"Aucun", "ELF32", "ELF64"};
	char endian[3][6] = {"Aucun", "LSB", "MSB"};
	char abi[20][15] = {"UNIX System V", "HP-UX", "NetBSD", "Linux", "Sun Solaris", "IBM AIX", "SGI IRIX", "FreeBSD", "Compaq TRU64", "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS", "Fenix OS", "CloudABI", "ARM Eabi", "Sortix", "ARM", "Standalone"};
	char type[5][23] = {"Type Inconnu", "Fichier Repositionable", "Fichier Executable", "Objet partagé", "Fichier Core"};
	int c = header_elf.e_ident[4];
	int e = header_elf.e_ident[5];
	int a = header_elf.e_ident[7];
	int t = header_elf.e_type;
	printf("Magic: ");
	for(int i=0; i<16; i++){
		if(header_elf.e_ident[i]/16 == 0)
			printf(" 0%x", header_elf.e_ident[i]);
		else
			printf(" %x",header_elf.e_ident[i]);
	}
	printf("\nClasse : ");
	if (c < 3 && c > -1)
		printf("%s\n", classe[c]);
	else	
		printf("Erreur\n");

	printf("Endianness : ");
	if (e < 3 && e > -1)
		printf("%s\n", endian[e]);
	else
		printf("Erreur\n");

	printf("ABI : ");
	if (a < 20 && a > -1)
		printf("%s\n", abi[a]);
	else
		printf("Erreur\n");
	
	printf("Type: ");
	if (t < 5 && t > -1)
		printf("%s\n", type[t]);
	else
		printf("Erreur\n");
}

void affichageMachine(Elf32_Ehdr header_elf){
	printf("Machine: ");
	switch(header_elf.e_machine){
		case EM_NONE : printf("Machine inconnue");
			break;
		case EM_M32 : printf("AT&T WE 32100");
			break;
		case EM_SPARC : printf("Sun Microsystems SPARC");
			break;
		case EM_386 : printf("Intel 80386");
			break;
		case EM_68K : printf("Motorola 68000");
			break;
		case EM_88K : printf("Motorola 88000");
			break;
		case EM_860 : printf("Intel 80860");
			break;
		case EM_MIPS : printf("MIPS RS3000");
			break;
		case EM_PARISC : printf("HP/PA");
			break;
		case EM_SPARC32PLUS : printf("SPARC avec jeu d'instruction étendu");
			break;
		case EM_PPC : printf("PowerPC");
			break;
		case EM_PPC64 : printf("PowerPC 64 bits");
			break;
		case EM_S390 : printf("BM S/390");
			break;
		case EM_ARM : printf("Advanced RISC Machine (ARM)");
			break;
		case EM_SH : printf("Renesas SuperH");
			break;
		case EM_SPARCV9 : printf("Machine : SPARCC v9 64 bits");
			break;
		case EM_IA_64 : printf("Intel Itanium");
			break;
		case EM_X86_64 : printf("AMD x86-64");
			break;
		case EM_VAX : printf("DEC Vax");
			break;
		default : printf("Erreur");
			break;
	}
	printf("\n");
}

void affichageOthers(Elf32_Ehdr header_elf){
	printf("Version: ");
	int version = header_elf.e_version;
	if (version == 0)
		printf("Version invalide\n");
	else if (version == 1)
		printf("Version actuelle\n");
	else
		printf("Erreur\n");

	//affichage du reste des champs de l'en tête
	printf("Adresse de point d'entrée : 0x%x\n",header_elf.e_entry);
	printf("Debut de l'en-tête du programme : %d (octets)\n", header_elf.e_phoff);
	printf("Debut de la table des sections : %d (octets)\n", header_elf.e_shoff);
	printf("Flags : 0x%x\n",header_elf.e_flags);
	printf("Taille de l'en-tête : %d (octets)\n", header_elf.e_ehsize);
	printf("Taille de l'en-tête des sections : %d (octets)\n",header_elf.e_shentsize);
	printf("Nombre d'en-tête de sections : %d\n",header_elf.e_shnum);
	printf("Table d'indexes des chaînes d'entêtes de section : %d\n",header_elf.e_shstrndx);
	printf("Taille de l'en-tête du programme (octets) : %d\n", header_elf.e_phentsize);
	printf("Nombre d'en-tête du programme : %d\n", header_elf.e_phnum);
}
