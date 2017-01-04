#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>

Elf32_Ehdr lectureheader(FILE* f);
void affichageheader(Elf32_Ehdr header_elf);

uint32_t Swap32(uint32_t valeur, char endianess);

uint16_t Swap16(uint16_t valeur, char endianess);


//Section header Etape 2
void lectureTableSection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t);

int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr *section_header, FILE* elf, Elf32_Shdr headerCourant);

void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf);

//Recherche Section Etape 3
void afficheSectionByName(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, char * secName);


//Etape 5
int lectureTableSymbole(Elf32_Sym* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

Elf32_Off getIndexStringTable(Elf32_Shdr* tabSection);

void recupNomSymbole(Elf32_Off offsetStringTable, Elf32_Word index, FILE* f);

void affichageTableSymbole(Elf32_Sym* tabSymbole, int size, FILE* f);
