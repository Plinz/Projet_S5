#include "elf_reader.h"

Elf32_Ehdr recupEnTete(FILE* f){
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
