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
	
	int i;
	printf("ELF Header:\n");
	printf("Magic: ");
	for(i=0;i<16;i++){
		if(header_elf.e_ident[i]/16 == 0){
			printf(" 0%x", header_elf.e_ident[i]);
		}else{
			printf(" %x",header_elf.e_ident[i]);
		}
	}
	printf(" \n");
	// Utilisation du Magic pour determiner classe, donnees et OS-ABI
	printf("Classe : ");
	switch(header_elf.e_ident[4]){
		case 0 :
			printf ("Aucun");
			break;
		case 1 :
			printf ("ELF32");
			break;
		case 2 : 
			printf("ELF64");
			break;
		default :
			printf("Erreur");
			break;
	}
	printf("\n");


	printf("Endianness : ");
	switch(header_elf.e_ident[5]){
		case 0 :
			printf ("Aucun");
			break;
		case 1 :
			printf ("LSB");
			break;
		case 2 : 
			printf("MSB");
			break;
		default :
			printf("Erreur");
			break;
	}
	printf("\n");

	printf("ABI : ");
	switch(header_elf.e_ident[7]){
		case 0 :
			printf ("UNIX System V");
			break;
		case 1 :
			printf ("HP-UX");
			break;
		case 2 : 
			printf("NetBSD");
			break;
		case 3 : 
			printf("Linux");
			break;
		case 6 : 
			printf("Sun Solaris");
			break;
		case 7 : 
			printf("IBM AIX");
			break;
		case 8 : 
			printf("SGI IRIX");
			break;
		case 9 : 
			printf("FreeBSD");
			break;
		case 10 : 
			printf("Compaq TRU64");
			break;
		case 11 : 
			printf("Novell Modesto");
			break;
		case 12 : 
			printf("OpenBSD");
			break;
		case 13 : 
			printf("OpenVMS");
			break;
		case 14 : 
			printf("NonStop Kernel");
			break;
		case 15 : 
			printf("AROS");
			break;
		case 16 : 
			printf("Fenix OS");
			break;
		case 17 : 
			printf("CloudABI");
			break;
		case 64 : 
			printf("ARM Eabi");
			break;
		case 83 : 
			printf("Sortix");
			break;
		case 97 : 
			printf("ARM");
			break;
		case 255 : 
			printf("Standalone");
			break;
		default :
			printf("Erreur");
			break;
	}
	printf("\n");
	//différent cas de type de fichier pour affiché la traduction des code hexa
	printf("Type: ");
	switch(header_elf.e_type){
		case ET_NONE :
			printf("Type inconnu");
			break;
		case ET_REL :
			printf("Fichier repositionnable");
			break;
		case ET_EXEC :
			printf("Fichier exécutable");
			break;
		case ET_DYN :
			printf("Objet partagé");
			break;
		case 4 :
			printf("Fichier core");
			break;
		default :
			printf("Erreur");
			break;

	}
	printf("\n");

	//différent cas de type de machine où le code est écris pour affiché la traduction des code hexa
	printf("Machine: ");
	switch(header_elf.e_machine){
		case EM_NONE :
			printf("Machine inconnue");
			break;
		case EM_M32 :
			printf("AT&T WE 32100");
			break;
		case EM_SPARC :
			printf("Sun Microsystems SPARC");
			break;
		case EM_386 :
			printf("Intel 80386");
			break;
		case EM_68K :
			printf("Motorola 68000");
			break;
		case EM_88K :
			printf("Motorola 88000");
			break;
		case EM_860 :
			printf("Intel 80860");
			break;
		case EM_MIPS :
			printf("MIPS RS3000");
			break;
		case EM_PARISC :
			printf("HP/PA");
			break;
		case EM_SPARC32PLUS :
			printf("SPARC avec jeu d'instruction étendu");
			break;
		case EM_PPC :
			printf("PowerPC");
			break;
		case EM_PPC64 :
			printf("PowerPC 64 bits");
			break;
		case EM_S390 :
			printf("BM S/390");
			break;
		case EM_ARM :
			printf("Advanced RISC Machine (ARM)");
			break;
		case EM_SH :
			printf("Renesas SuperH");
			break;
		case EM_SPARCV9 :
			printf("Machine : SPARCC v9 64 bits");
			break;
		case EM_IA_64 :
			printf("Intel Itanium");
			break;
		case EM_X86_64 :
			printf("AMD x86-64");
			break;
		case EM_VAX :
			printf("DEC Vax");
			break;
		default :
			printf("Erreur");
			break;
	}
	printf("\n");

	//version du programe
	printf("Version: ");
	switch(header_elf.e_version){
		case EV_NONE :
			printf("Version invalide");
			break;
		case EV_CURRENT :
			printf("Version actuelle");
			break;
	}
	printf("\n");	

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
