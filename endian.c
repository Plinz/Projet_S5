#include "elf_reader.h"

uint32_t Swap32(uint32_t valeur, char endianess){
	if(endianess == ELFDATA2LSB){
		return valeur;
	}else{
		return __bswap_32(valeur);
	}
}

uint16_t Swap16(uint16_t valeur, char endianess){
	if(endianess == ELFDATA2LSB){
		return valeur;
	}else{
		return __bswap_16(valeur);
	}
}
