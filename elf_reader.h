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
Structure permettant de sauvegarder un symbole ainsi que son fichier d'origine
  Utilse pour la fusion et connaitre l'origine d'un symbole
*/
typedef struct symbole {
  Elf32_Sym symbole;
  int fichier;
} Symbole;


/*=====================================================================================================
                        FONCTION D'EN TETE //elf_header.c
======================================================================================================= */

/*
  Fonction: permet de remplir la structure El32_Ehdr qui contientra l'en-tête du fichier elf
  @Param: FILE* f: un fichier au format elf
  @Return: Elf32_Ehdr: une structure Elf32_Ehdr rempli
*/
Elf32_Ehdr lectureheader(FILE* f);

/*
  Fonction: affiche l'en-tête d'un fichier elf
  @Param: Elf32_Ehdr header_elf: une structure elf32_Ehdr
*/
void affichageheader(Elf32_Ehdr header_elf);

/*
  Fonction: affichage du nombre magique d'un fichier elf
  @Param: Elf32_Ehdr header_elf: une structure elf32_Ehdr
*/
void affichageNumMagic(Elf32_Ehdr header_elf);


/*
  Fonction: affichage de la classe et de l'endianess
  @Param: Elf32_Ehdr header_elf: une structure elf32_Ehdr
*/
void affichageMagicClasseEndianABIType(Elf32_Ehdr header_elf);

/*
  Fonction: affichage de la machine
  @Param: Elf32_Ehdr header_elf: une structure elf32_Ehdr
*/
void affichageMachine(Elf32_Ehdr header_elf);

/*
  Fonction: affichage de la partie other
  @Param: Elf32_Ehdr header_elf: une structure elf32_Ehdr
*/
void affichageOthers(Elf32_Ehdr header_elf);


/*
  Fonction: Permet de changer l'endianess de valeur selon l'endianess, sur 32 bits
  @Param: uint32_t valeur: la valeur à changer
  @Param: char endianess: l'endianess du fichier elf
  @Return: uint32_t: la valeur changé
*/
uint32_t Swap32(uint32_t valeur, char endianess);

/*
  Fonction: Permet de changer l'endianess de valeur selon l'endianess, sur 16 bits
  @Param: uint16_t valeur: la valeur à changer
  @Param: char endianess: l'endianess du fichier elf
  @Return: uint16_t: la valeur changé
*/
uint16_t Swap16(uint16_t valeur, char endianess);



/*=========================================================================================================
                  FONCTION EN TETE DE SECTION // elf_sectionTable.c
==========================================================================================================*/

/*
  Fonction: permet de remplir un tableau de structure de Elf32_Shdr à partir d'un fichier ua format elf
  @Param: FILE* f: le fichier elf de référence
  @Param: Elf32_Ehdr header_elf: la structure de l'en-tête du ficher elf
  @Param: Elf32_Shdr *t: un pointeur sur une structure Elf32_Shdr
  @SideEffect: Remplissage du tableau de structure pointé par t
*/
void lectureTableSection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t);

/*
  Fonction: Permet d'afficher le nom d'une section
  @Param: Elf32_Ehdr header: En tête du fichier elf de référence
  @Param: Elf32_Shdr: *section_header: un pointeur de structure Elf32_Shdr
  @Param: FILE* elf: le fichier elf de référence
  @Param: Elf32_Shdr headerCourant: En tête courant pour lequel on souhaite afficher son nom
  @return: int: taille du nom de la section
*/
int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr *section_header, FILE* elf, Elf32_Shdr headerCourant);

/*
  Fonction: Permet d'afficher toute une table des en-têtes de section
  @Param: Elf32_Shdr *section_elf: un tableau d'elf32_Shdr contenant toutes les en-têtes de section
  @Param: Elf32_Ehdr header_elf: l'en-tête du fichier elf de référence
  @Param: FILE* f: le fichier elf de référence
*/
void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf);


/*=============================================================================================================
                    FONCTION D'AFFICHAGE DU CONTENU DES SECTIONS // elf_recherche_section.c
===============================================================================================================*/

/*
  Fonction: permet de retrouver une section grace à son nom
  @Param: ELf32_Ehdr fileHeader: la structure d'en-tête du fichier elf de référence
  @Param: Elf32_Shdr *sections_headers: Un tableau de structure Elf32_Shdr dans lequel on va rechercher la section
  @Param: File* elf: le fichier elf de référence
  @Param: char* secName: le nom de la section à rechercher
  @Return: Elf32_Shdr: une structure d'en-tête de section
*/
Elf32_Shdr getSectionByName(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, char * secName);


/*
  Fonction: permet de retrouver une section grace à son index dans la table
  @Param: ELf32_Ehdr fileHeader: la structure d'en-tête du fichier elf de référence
  @Param: Elf32_Shdr *sections_headers: Un tableau de structure Elf32_Shdr dans lequel on va rechercher la section
  @Param: File* elf: le fichier elf de référence
  @Param: int secNum: un indice dans le tableau
  @Return: Elf32_Shdr: une structure d'en-tête de section
*/
Elf32_Shdr getSectionByIndex(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers,int secNum);

/*
  Fonction: permet d'afficher le contenue d'une section
  @Param:Elf32_Shdr currentSection: la section dans laquelle on veut lire le contenue
  @Param: FILE* elf: le fichier elf de référence
*/
void afficheContenue(Elf32_Shdr currentSection, FILE* elf);


/*===================================================================================================================
                          FONCTION DE GESTION DE LA TABLE DES SYMBOLES // elf_symboleTable.c
=====================================================================================================================*/

/*
  Fonction: Permet d'afficher une table des symboles d'un fichier au format elf
  @Param: Symbole* tabSymbole: Le tableau de structure dans lequel on va lire les symboles pour les afficher
  @Param: int sizeTabSymbole: la taille du tableau tabSymbole
  @Param: FILE* f: le fichier f de référence
  @Param: Elf32_Shdr* tabSection: le tableau des en têtes de sections
  @Param: Elf32_Ehdr header: l'en tête du fichier elf de référence
*/
void affichageTableSymbole(Symbole* tabSymbole, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

/*
TODO
*/
void affichageTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, int sizeTabSymbole, FILE* f, Elf32_Shdr* tabSection, Elf32_Ehdr header);

/*
  Fonction: permet de remplir un tableau de structure de Symbole
  @Param: Symbole* tabSymbole: un tableau de Symbole contenant également une strucute Elf32_Sym qui sera rempli par effet de bord
  @Param: Elf32_Ehdr header: l'en tête du fichier elf de référence
  @Param: Elf32_Shdr* tabSection: Le tableau des en têtes de section à parcourir afin de retrouver la table des symboles
  @Param: FILE* f: le fichier elf de référence
  @Return: int: le nombre de symbole qui aura été ajouté dans le tableau
*/
int lectureTableSymbole(Symbole* tabSymbole, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

/*
TODO
*/
int lectureTableSymboleDynamique(Elf32_Sym* tabSymboleDynamique, Elf32_Ehdr header, Elf32_Shdr* tabSection, FILE* f);

/*
  Fonction: Permet de retrouver l'offset d'une section dans un fichier elf à partir de son nom
  @Param: Elf32_Ehdr header_elf: l'en tête du fichier elf de référence
  @Param: Elf32_Shdr *sections_table: Le tableau des en têtes de section à parcourir
  @Param: FILE* elf: le fichier elf de référence
  @Param: char * secName: Le nom de la section à retrouvé
  @Return: Elf32_Word: l'offset de la section secName ou -1 si non trouvé
*/
Elf32_Word rechercheOffsetSection(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName);

/*
  Fonction: permet de récupérer le nom d'un symbole dans la table des chaines
  @Param: Elf32_Word index: offset de la table des chaines
  @Param: FILE* f: fichier elf de référence
  @Param: Elf32_Word offset: offset du nom du symbole dans la table des chaines
  @Param: char* c: La chaine de stockage du nom du symbole
  @SideEffet: remplissage de la chaine de caractère c
*/
void recupNomSymbole(Elf32_Word index, FILE* f, Elf32_Word offset,char *c);


/*=====================================================================================================================
                          GESTION TABLE DE REIMPLANTATION // elf_relocate.c
=======================================================================================================================*/

int taillerela(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rela* lesrela, FILE* elf) ;

int taillerel(Elf32_Ehdr *file_header, Elf32_Shdr *section_headers, Elf32_Rel* lesrel, FILE* elf);

int affichage_relocation(Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers,FILE* elf);

int nbRel(Elf32_Ehdr *fileHeader, Elf32_Shdr *sections_headers);

