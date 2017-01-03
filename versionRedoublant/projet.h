#include <stdio.h>
#include <elf.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>



/*****************************************************************************************************************
 * Structure TableReimplantation
 * Cette structure permet un meilleure manipulation de la table de réimplantion
 * Elle comporte: un type Elf32_Shdr, un type Elf32_Rel, un entier permettant de compter le nombre d'entrées dans la 
 * 			     table
 * *****************************************************************************************************************/
 
 
typedef struct {
	Elf32_Shdr section_rel;
	Elf32_Rel *reimplantation_elf;
	int nb_entrees;
} TableReimplantation;


/****************************************************************************************************************
 * recupEnTete
 * description: permet de récupérer l'en-Tete d'un fichier Elf et de stocker le résultat dans une structure
 * 				Elf32_Ehdr, structure fourni par elf.h
 * Paramètre: un fichier au format elf
 * Valeur de retour: une structure Elf32_hdr
 * Effet de bord: aucun
 * **************************************************************************************************************/
 Elf32_Ehdr recupEnTete(FILE* f);



/****************************************************************************************************************
 * affichageEnTete
 * description: Permet d'afficher l'en-Tete d'un fichier Elf
 * Paramètre: une structure Elf32_Ehdr
 * Valeur de retour: aucune
 * Effet de bord: aucun
 * **************************************************************************************************************/
void affichageEnTete(Elf32_Ehdr header_elf);



/****************************************************************************************************************
 * recupTabSection
 * description: permet de récupérer les en-Tetes de section d'un fichier Elf et de stocker le résultat dans un tableau de 
 * 				structure Elf32_Shdr, structure fourni par elf.h
 * Paramètre: un fichier au format elf, une structure d'en-tete de fichier Elf32_Ehdr, un pointeur de structure Elf32_Shdr
 * Valeur de retour: aucun
 * Effet de bord: insère du contenu dans Elf32_Shdr
 * **************************************************************************************************************/
void recupTabsection(FILE* f, Elf32_Ehdr header_elf, Elf32_Shdr *t);


/****************************************************************************************************************
 * affichageTabSection
 * description: Permet d'afficher les en-têtes de sections d'un fichier elf
 * Paramètre: un pointeur de structure Elf32_Shdr, une structure Elf32_Ehdr, un fichier au format elf
 * Valeur de retour: aucune
 * Effet de bord: aucun
 * **************************************************************************************************************/
void affichageTabsection(Elf32_Shdr *section_elf, Elf32_Ehdr header_elf, FILE* elf);




/****************************************************************************************************************
 * affichageContenuSection
 * description: Permet d'afficher le contenu du section choisi par l'utilisateur
 * Paramètre: un pointeur de structure Elf32_Shdr, un numéro de section, un fichier au format elf
 * Valeur de retour: aucune
 * Effet de bord: aucun
 * **************************************************************************************************************/
void affichageContenuSection(Elf32_Shdr* tabEnTeteSection, int numberSection, FILE* elf);




/****************************************************************************************************************
 * getTableauSymboles
 * description: permet de récupérer, grace à un passage par adresse, la table des symboles d'un fichier elf
 * Paramètre: un fichier au format elf, une structure Elf32_Ehdr, un pointeur de structure Elf32_Shdr,
 * 			  un pointeur de pointeur d'une structure Elf32_Sym
 * Valeur de retour: un entier, indiquant le nombre d'entrée dans le tableau de symbole
 * Effet de bord: remplit le tableau de structure Elf32_Sym
 * **************************************************************************************************************/
int getTableauSymboles(FILE* file, Elf32_Ehdr header_elf, Elf32_Shdr *section_elf, Elf32_Sym **symbole_elf);




/****************************************************************************************************************
 * affichageSymbole
 * description: Permet d'afficher la table des symboles d'un fichier ELF passé en paramètre.
 * Paramètre: un fichier au format elf, une structure Elf32_Ehdr, un pointeur de structure Elf32_Sym, un pointeur
 * 		      de structure Elf32_Shdr, une entier indiquant la taille de la section
 * Valeur de retour: une structure Elf32_hdr
 * Effet de bord: aucun
 * **************************************************************************************************************/
void affichageSymbole(FILE* elf, Elf32_Ehdr header_elf, Elf32_Sym *symboltable, Elf32_Shdr *section_elf, int size);




/****************************************************************************************************************
 * afficherNomSection
 * description: Cette fonction permet de récupérer le nom d'une section
 * Paramètre: une structure Elf32_Ehdr, un pointeur de structure Elf32_Shdr, un entier indiquand le nombre de section
 * 			  un fichier au format elf, l'en-tete de section courant Elf32_Shdr
 * Valeur de retour: la taille de la chaine de caractere
 * Effet de bord: aucun
 * **************************************************************************************************************/
int afficherNomSection(Elf32_Ehdr header, Elf32_Shdr* section_header, FILE* elf, Elf32_Shdr headerCourant);


void afficherNomSymboles(Elf32_Ehdr header_elf, Elf32_Sym *symboltable, Elf32_Shdr *section_elf);






/****************************************************************************************************************
 * recupTableReimplantation
 * description: Cette fonction permet de récupérer la table de réimplantation d'un fichier elf
 * Paramètre: une fichier elf, une structure Elf32_Ehdr, un pointeur de structure Elf32_Shdr
 * Valeur de retour: une structure tableReimplantion
 * Effet de bord: aucun
 * **************************************************************************************************************/
TableReimplantation* recupTabReimplantation(FILE* file, Elf32_Ehdr header_elf, Elf32_Shdr *section_elf);



/****************************************************************************************************************
 * affichageTabReimplantation
 * description: Cette fonction permet d'afficher la table de réimplantation d'un fichier elf
 * Paramètre: un pointeur de structure TableReimplantation, un entier indiquand la longueur de la table
 * Valeur de retour: aucune
 * Effet de bord: aucun
 * **************************************************************************************************************/
void affichageTabReimplantation(Elf32_Sym *SymbolTable, FILE* elf, Elf32_Ehdr header_elf, Elf32_Shdr *SectionTable, TableReimplantation* pointeur_reimplantation, int longueurTab);



/****************************************************************************************************************
 * recupTableReimplantation
 * description: Cette fonction permet de récupérer la table de réimplantation d'un fichier elf
 * Paramètre: une fichier elf, une structure Elf32_Ehdr, un pointeur de structure Elf32_Shdr
 * Valeur de retour: une structure tableReimplantion
 * Effet de bord: aucun
 * **************************************************************************************************************/
TableReimplantation* recupTabReimplantation(FILE* file, Elf32_Ehdr header_elf, Elf32_Shdr *section_elf);



/****************************************************************************************************************
 * affichageTypeReimplantation
 * description: Cette fonction permet d'afficher le type de réimplantation dans un format lisible
 * Paramètre: un char correspondant au type de répimplatation sous son format brut
 * Valeur de retour: aucune
 * Effet de bord: aucun
 * **************************************************************************************************************/
void afficherTypeReimplantation(unsigned char typeReimplantation);


/****************************************************************************************************************
 * nb_tab_rel
 * description: permet de récupérer le nombre de section de type REl
 * Paramètre: une structure Elf32_Ehdr, un pointeur de structure Elf32_Shdr
 * Valeur de retour: un entier
 * Effet de bord: aucun
 * **************************************************************************************************************/
int nb_tab_rel(Elf32_Ehdr header_elf, Elf32_Shdr *section_elf);


/****************************************************************************************************************
 * recupererValeurSwap32
 * description: Cette fonction permet de récupérer automatiquement la valeur dans son bon format
 * 				lu en fonction de l'endianess passé en paramètre pour les types sur 32 bits
 * Paramètre: une valeur entièrere sur 32 bits, l'endianness du fichier
 * Valeur de retour: un entier sur 32 bits
 * Effet de bord: aucun
 * **************************************************************************************************************/
uint32_t recupererValeurSwap32(uint32_t valeur, char endianess);


/****************************************************************************************************************
 * recupererValeurSwap16
 * description: Cette fonction permet de récupérer automatiquement la valeur dans son bon format
 * 				lu en fonction de l'endianess passé en paramètre pour les types sur 16 bits
 * Paramètre: une valeur entièrere sur 16 bits, l'endianness du fichier
 * Valeur de retour: un entier sur 16 bits
 * Effet de bord: aucun
 * **************************************************************************************************************/
uint16_t recupererValeurSwap16(uint16_t valeur, char endianess);


void renumerotationSection(Elf32_Shdr* tabAncienneSections, Elf32_Shdr* tabNouvelleSections, Elf32_Ehdr enTete);
