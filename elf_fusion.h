#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

/*=====================================================================================================
                                           STRUCTURE
=====================================================================================================*/


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

/*=====================================================================================================================
                          GESTION FUSION SIMPLE ET SECTION // elf_fusionSimple.c
=======================================================================================================================*/


/*
  fonction: Permet d'initialiser le contenu de la structure de fichier Elf
  @Param: FichierElf fichierELf
*/
void initFichierELF(FichierElf fichierElf);

/*
  fonction: permet de libérer la mémoire allouer à la structure fichierElf passé en paramètre
  @Param: FichierElf fichierELf
*/
void freeMemory(FichierElf fichierElf);

/*
  fonction: permet de fusionner deux en-têtes de fichier
  @Param: FichierElf *fichierElf1: la structure du premier fichier à fusionner
  @Param: FichierElf *fichierElf2: la structure du deuxième fichier à fusionner
  @Return: Elf32_Ehdr: une structure contenant le nouvelle en-tête du fichier Elf
*/
Elf32_Ehdr header(FichierElf *fichierElf1, FichierElf *fichierElf2);


/*
  fonction: permet de recréer la section Shstrab (nom des sections)
  @Param: Section *section: pointeur vers la structure de section qui va contenir les éléments de Shstrab
  @Param: Strtab* shstratb: la table des chaines qui contient les éléménts nécessaires pour écrire la section
  @SideEffect: la section est rempli
*/
void sectionShstrtab(Section *section, Strtab * shstrtab);

/*
  fonction: permet de recréer la section strab (table des chaines)
  @Param: Section *section: pointeur vers la structure de section qui va contenir les éléments de strab
  @Param: Strtab* stratb: la table des chaines qui contient les éléménts nécessaires pour écrire la section
  @SideEffect: la section est rempli
*/
void sectionStrtab(Section *section, Strtab * strtab);

/*
  Fonction: Permet de fusionner par concatenation simple deux sections, elle va éraliser la fusion adéquate en fonction du type des deux section passer en parametre
  @Param: Elf32_Shdr sectionHeader1 : Header de section de la première section à fusionner
  @Param: Elf32_Shdr sectionHeader1 : Header de section de la seconde section à fusionner
  @param: FichierElf * fichierElf1 : la structure du premier fichier à fusionner
  @param: FichierElf * fichierElf2: la structure du second fichier à fusionner
  @param: FichierElf * fichierElfRes: la structure du fichier fusion resultat
  @param: Strtab * shstrtab : structure qui va contenir les noms de toutes les sections
  @param: Strtab * strtab : structure qui va contenir les noms de tous les symboles
  @Return: Une structure Section qui possède le contenu à ecrire dans le nouveaux fichier
*/
void sectionFusionSimple(Section *fusion, Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2, Strtab * shstrtab);

/*
  Fonction: Permet de fusionner deux sections, elle va réaliser la fusion adéquate en fonction du type des deux section passer en parametre
  @Param: Elf32_Shdr sectionHeader1 : Header de section de la première section à fusionner
  @Param: Elf32_Shdr sectionHeader1 : Header de section de la seconde section à fusionner
  @param: FichierElf * fichierElf1 : la structure du premier fichier à fusionner
  @param: FichierElf * fichierElf2: la structure du second fichier à fusionner
  @param: FichierElf * fichierElfRes: la structure du fichier fusion resultat
  @param: Strtab * shstrtab : structure qui va contenir les noms de toutes les sections
  @param: Strtab * strtab : structure qui va contenir les noms de tous les symboles
  @Return: Une structure Section qui possède le contenu à ecrire dans le nouveaux fichier
*/
Section sectionfusion(Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2,FichierElf * fichierElfRes, Strtab * shstrtab, Strtab * strtab);

/*
  Fonction: Permet d'ajouter une section
  @Param: Elf32_Shdr sectionHeader : Header de section de la section à ajouter
  @param: FichierElf * fichierElf : la structure du fichier cotenant la fusion à ajouter
  @param: Strtab * shstrtab : structure qui va contenir les noms de toutes les sections
  @Return: Une structure Section qui possède le contenu à ecrire dans le nouveaux fichier 
*/
Section SectionAjout(Elf32_Shdr sectionHeader, FichierElf * fichierElf, Strtab * shstrtab);

/*
  Fonction: permet de savoir si une section est présente dans un fichierElf
  @Param: FichierElf * fichierElf: le fichier dans lequel on va rechercher la section
  @Param: char * secName: le nom de la section à rechercher
  @Param: Elf32_Shdr * res: la section qui a été retrouvé
  @Param: int * present: un flag permettant de savoir si la sectiojn a été trouvé
*/
void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res, int * present);

/*
  @Fonction: permet de récupérer le nom d'une section
  // to end
*/
char * getSectionName(Elf32_Shdr section, FichierElf * fichier);

/*
  Fonction: permet de connaitre le nombre de symbole dans un fichier elf
  @Param:FichierELf *f: le fichier elf de référence
  @Return: int: le nombre de symbole
*/
int getNbSymbols(FichierElf *f);

/*
  Fonction: Permet d'écrire le nouveau fichier après la fusion
  @Param: FichierElf *ficherElfRes: la structure contenant le résultat de la fusion
  @Param: Section *sections_elfRes: le tableau de structure de section contenant toutes les sections fusionnées
  @Param: Strtab * shstrtab: La structure contenant les noms des en têtes de section
  @SideEffect: écriture dans la structure fichierElfRes du nouveau fichier elf
*/
void ecritureFichierFusionnee(FichierElf *fichierElfRes, Section * sections_elfRes, Strtab * shstrtab);

/*
  Fonction: Fonction permettant de fusionner les deux fichiers elf et d'obtenir le résultat dans un fichier fusion
  @Param: FichierElf *fichierElf1: La structure contenant le premier fichier à fusionner
  @Param: FichierElf *fichierElf2: La structure contenant le deuxième fichier à fusionner
  @Param: FichierElf *fichierElf1: La structure contenant le résultat du fichier fusionné
  @SideEffect: ecriture dans la structure contenant le fichier de la fusion
*/
void fusion(FichierElf *fichierElf1, FichierElf *fichierElf2, FichierElf *fichierElfRes);

/*=====================================================================================================================
                          GESTION FUSION SYMBOLE // elf_fusionSymbole.c
=======================================================================================================================*/

//Elf32_Shdr getSectionByName(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);

/*
  Fonction: permet de créer une nouvelle table symbole en fusionnant celles des deux fichiers d'origine
  @Param: FichierElf structFicher1: structure contenant le premier fichier à fusionner
  @Param: FichierElf structFicher2: structure contenant le deuxième fichier à fusionner
  @Param: int sizeTab1: la taille de la table des symboles du premier fichier
  @Param: int sizeTab2: la taille de la table des symboles du deuxième fichier
  @Param: Symbole *newTabSymbole: tableau de la structure qui contiendra lanouvelle table des symboles
  @Param: Strtab *strtab: pointeur vers une structure strtab
  @SideEffect: écriture de la nouvelle table de symbole et mise à jour de la table des chaines
*/
int fusionTableSymbole(FichierElf structFichier1, FichierElf structFichier2, int sizeTab1, int sizeTab2, Symbole *newTabSymbole, Strtab * strtab);


/*
  Fonction: permet d'ajouter un nom dans la table des chaines
  @Param: char * nom: le nom à ajouter
  @Param: Strtat *strtab: pointeur vers la structure contenant la table des chaines
  @Param: Elf32_Sym: pointeur vers une structure de symbole
*/
void AjoutNomStrtab(char * nom, Strtab * strtab, Elf32_Sym * symb);

/*
TODO
*/
void ajoutSymbole(Symbole *newTabSymbole, int * nbEntree, FichierElf structFichierSrc, int indexSrc, char *symbole, Strtab *strtab, int numFichier);

/*
TODO
*/
Section creerSectionTableSymbole(Symbole *tableSymbole, int sizeTableSymbole, Elf32_Shdr structFichier1, Elf32_Shdr structFichier2);

/*
  Fonction: permet de connaitre la taille d'une table section
  @Param: Symbole *tabSymbole: la table des symbole
  @Param: int sizeTableSymbole: la taille de la table des symboles
  @Return: une structure section
*/
int getSizeOfSectionTable(Symbole *tabSymbole, int sizeTableSymbole);

  
/*
  Fonction: permet de connaitre la valeur de ShInfo pour une section
  @Param: Symbole *tabSymbole: la table des symbole
  @Param: int sizeTableSymbole: la taille de la table des symboles
  @Return: la taille de la section
*/
int getShInfo(Symbole* tableSymbole, int sizeTableSymbole);

/*
  Fonction: permet d'écrire l'attribut contenu d'une structure de section
  @Param: Symbole *tableSymbole: les symboles que l'on doit écrire dans le contenu
  @Param: int sizeTab: la taille de la table des symboles
  @Param: Section *section: la section qui va contenir les symboles
  @SideEffect: écriture du contenu dans la structure section passé en paramètre
*/
void EcrireContenu(Symbole *tableSymbole, int sizeTab, Section *section);

/*
  Fonction: permet de connaitre la valeur de ShIndx pour une section
  @Param: Symbole symbol: un symbole
  @Param: fichierElf * fichierElf: un pointeur vers une structure fichierElf
  @Param: int nbSections: le nombre de section
  @Param: Strtab *shstrtab: un pointeur vers la section shstrtab
*/
int getSt_shndx(Symbole symbol, FichierElf * fichierElf, int nbSections, Strtab *shstrtab);

// ELF FUSION REL

int relsize(FichierElf * elfile);
Reloctable* crea_rel_table (FichierElf * elfile);
Section RelFusion(FichierElf* oldelf1, Elf32_Shdr OldSec1, Elf32_Shdr OldSec2, FichierElf* oldelf2, FichierElf * newelf, int nbnewsymbole);
Section RelUpdate (FichierElf* oldelf, FichierElf* newelf, Elf32_Shdr OldSec, int nbnewsymbole);
void EcritureStruct (Elf32_Rel *tabrel, int taillerel, Section *section, Elf32_Shdr* headsec);
int init_new_rel (FichierElf* new_elf, FichierElf* oldelf1, FichierElf* oldelf2);
