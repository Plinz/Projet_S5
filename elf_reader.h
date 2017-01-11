#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

/*
Structure permettant de sauvegarder un symbole ainsi que son fichier d'origine
  Utilse pour la fusion et connaitre l'origine d'un symbole
*/
typedef struct symbole {
  Elf32_Sym symbole;
  int fichier;
} Symbole;

/*
  Structure: Super Structure regroupant toutes les composantes d'un fichier Elf
  Utile afin d'avoir accès à tous les éléments d'un fichier Elf
*/
typedef struct fichierElf {
	Elf32_Ehdr	header_elf;
	Elf32_Shdr	*sectionsTable;
	Symbole	*tabSymbole;
	Elf32_Sym	*tabSymboleDynamique;
	FILE 	 	*fichierElf;
	Elf32_Rel	*tabRel;
} FichierElf;

/*
  Structure: Cette structure est crée pour chaque nouvelle section après la fusion
    contenu: contenu brut de la section après fusion
    nbOctets: taille de la section créée
  Utile pour construire de manière générique le nouveau fichier objet
*/
typedef struct section {
 char * contenu;
 int nbOctets;
 int indexHeader;
 Elf32_Shdr header;
} Section;

/*
Structure: comme la structure section mais spécifique pour la section des chaines
  names: tableau à deux entrées stockant les noms
  offsetCourant: offset jusqu'où l'on écrit dans le fichier
  nbNames: nombre d'entrées

*/
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

void affichageNumMagic(Elf32_Ehdr header_elf);

void affichageMagicClasseEndianABIType(Elf32_Ehdr header_elf);

void affichageMachine(Elf32_Ehdr header_elf);

void affichageOthers(Elf32_Ehdr header_elf);

uint32_t Swap32(uint32_t valeur, char endianess);

uint16_t Swap16(uint16_t valeur, char endianess);


//Section header Etape 2
void lectureTableSection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t);

int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr *section_header, FILE* elf, Elf32_Shdr headerCourant);

void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf);


//Recherche Section Etape 3
Elf32_Shdr getSectionByName(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, char * secName);

Elf32_Shdr getSectionByIndex(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, int secNum);

void afficheContenue(Elf32_Shdr currentSection, FILE* elf);


//Etape 4
void affichageTableSymbole( Symbole* tabSymboleDynamique, int sizeTabSymboleDynamique, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

void affichageTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

int lectureTableSymbole(Symbole* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

int lectureTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

Elf32_Word rechercheOffsetSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);

void recupNomSymbole(Elf32_Word index, FILE* f, Elf32_Word offset,char *c);


//Relocation Etape 5

int taillerela(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rela* lesrela, FILE* elf) ;

int taillerel(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rel* lesrel, FILE* elf);

int affichage_relocation(Symbole* tabSymbole, Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers, int sizeTabSymbole, FILE* elf);

//Fusion Simple Etape 6

void initFichierELF(FichierElf fichierElf);

void freeMemory(FichierElf fichierElf);

Elf32_Ehdr header(FichierElf *fichierElf1, FichierElf *fichierElf2);

void sectionShstrtab(Section *section, Strtab * shstrtab);

void sectionStrtab(Section *section, Strtab * strtab);

void sectionFusionSimple(Section *fusion, Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2, Strtab * shstrtab);

Section sectionfusion(Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2,FichierElf * fichierElfRes, Strtab * shstrtab, Strtab * strtab);

Section SectionAjout(Elf32_Shdr sectionHeader, FichierElf * fichierElf, Strtab * shstrtab);

void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res, int * present);

char * getSectionName(Elf32_Shdr section, FichierElf * fichier);

int getNbSymbols(FichierElf *f);

void ecritureFichierFusionnee(FichierElf *fichierElfRes, Section * sections_elfRes, Strtab * shstrtab);

void fusion(FichierElf *fichierElf1, FichierElf *fichierElf2, FichierElf *fichierElfRes);

//Fusion Symbole Etape 7

//Elf32_Shdr getSectionByName(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);

void AjoutNomStrtab(char * nom, Strtab * strtab, Elf32_Sym * symb);

int fusionTableSymbole(FichierElf structFichier1, FichierElf structFichier2, int sizeTab1, int sizeTab2, Symbole *newTabSymbole, Strtab * strtab);

Section creerSectionTableSymbole(Symbole *tableSymbole, int sizeTableSymbole, Elf32_Shdr structFichier1, Elf32_Shdr structFichier2);

int getSizeOfSectionTable(Symbole *tabSymbole, int sizeTableSymbole);

int getShInfo(Symbole* tableSymbole, int sizeTableSymbole);

void EcrireContenu(Symbole *tableSymbole, int sizeTab, Section *section);

int getSt_shndx(Symbole symbol, FichierElf * fichierElf, int nbSections, Strtab *shstrtab);
