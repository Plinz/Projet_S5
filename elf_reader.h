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

void afficheSectionByNum(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, int secNum);

void afficheContenue(Elf32_Shdr currentSection, FILE* elf);


//Etape 4
void affichageTableSymbole( Elf32_Sym* tabSymboleDynamique, int sizeTabSymboleDynamique, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

void affichageTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

int lectureTableSymbole(Elf32_Sym* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

int lectureTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

Elf32_Word rechercheSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);


//Relocation Etape 5

int taillerela(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rela* lesrela, FILE* elf) ;

int taillerel(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rel* lesrel, FILE* elf);
  
int affichage_relocation(Elf32_Sym* tabSymbole, Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers, int sizeTabSymbole, FILE* elf);
