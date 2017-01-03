#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>

Elf32_Ehdr lectureheader(FILE* f);
void affichageheader(Elf32_Ehdr header_elf);

uint32_t Swap32(uint32_t valeur, char endianess);

uint16_t Swap16(uint16_t valeur, char endianess);


//Section header
void lectureTableSection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t);

int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr *section_header, FILE* elf, Elf32_Shdr headerCourant);

void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf);