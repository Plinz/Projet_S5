#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>

typedef struct fichierElf {
	Elf32_Ehdr	header_elf;
	Elf32_Shdr	*sectionsTable;
	Elf32_Sym	*tabSymbole;
	Elf32_Sym	*tabSymboleDynamique;
	FILE 	 	*fichierElf;
	Elf32_Rel	*tabRel;
} FichierElf;

typedef struct section {
 char * contenu;
 int nbOctets;
 Elf32_Shdr header;
} Section;

typedef struct shstrtab {
  char ** names;
  int offsetCourant;
  int nbNames;
} Shstrtab;

typedef struct strtab {
  char ** names;
  int offsetCourant;
  int nbNames;
} Strtab;

typedef struct reloctable {
  Elf32_Rel tablerel;
  int indice_section;
} Reloctable ;


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

Elf32_Word rechercheOffsetSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);

void recupNomSymbole(Elf32_Word index, FILE* f, Elf32_Word offset,char *c);


//Relocation Etape 5

int taillerela(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rela* lesrela, FILE* elf) ;

int taillerel(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rel* lesrel, FILE* elf);

int affichage_relocation(Elf32_Sym* tabSymbole, Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers, int sizeTabSymbole, FILE* elf);

//Fusion Simple Etape 6
Elf32_Ehdr header(FichierElf *fichierElf1, FichierElf *fichierElf2);

Section sectionShstrtab(Section section, Shstrtab * shstrtab);

Section sectionfusion(Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2, Shstrtab * shstrtab, Strtab * strtab);

Section SectionAjout(Elf32_Shdr sectionHeader, FichierElf * fichierElf, Shstrtab * shstrtab);

void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res, int * present);

char * getSectionName(Elf32_Shdr section, FichierElf * fichier);

int recupIndexByName(char * sht_name, int e_shstrndx, int nbSections, Elf32_Shdr * sections ,FILE * elf);

void fusion(FichierElf *fichierElf1, FichierElf *fichierElf2, FILE *elfRes);
