#include "projet.h"

uint32_t recupererValeurSwap32(uint32_t valeur, char endianess){
	if(endianess == ELFDATA2LSB){
		return valeur;
	}else{
		return __bswap_32(valeur);
	}
}

uint16_t recupererValeurSwap16(uint16_t valeur, char endianess){
	if(endianess == ELFDATA2LSB){
		return valeur;
	}else{
		return __bswap_16(valeur);
	}
}


int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr *section_header, FILE* elf, Elf32_Shdr headerCourant){
	char courant;
	int offset;
	int i = 0;
	
	offset = section_header[header.e_shstrndx].sh_offset;
	fseek(elf, offset + headerCourant.sh_name, SEEK_SET);

	courant = fgetc(elf);
	while(courant != '\0'){
		i++;
		printf("%c", courant);
		courant = fgetc(elf);
	}
	return i;
}

