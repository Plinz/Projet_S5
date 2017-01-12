#include "elf_reader.h"

/*
/ Fonction de recherche d'une recherche de section par nom, puis affichage de celle-ci
/ Arguments :
/		-header_elf 		:	header du fichier elf
/		-sections_table 	:	Tableau regroupant toutes les sections
/		-elf 				:	Fichier au format elf
/		-secName 			:	Nom de la section recherchee
*/
Elf32_Shdr getSectionByName(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, FILE* elf, char * secName){
	int i = 0;
	char * currentName = malloc(40);
	char courant;
	int trouve = 0;
	Elf32_Shdr currentSection;
	
	int offsetNameTable = sections_table[header_elf.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections

	//On parcour chaque section jusqu'à trouver la bonne ou, toute les parcourirs
	while (!trouve && i < header_elf.e_shnum) {
		//On se place dans le fichier afin de retouver le nom de la section courante
		currentSection = sections_table[i];
		fseek(elf, offsetNameTable + currentSection.sh_name, SEEK_SET); //offset nous ammène au début du stockage des noms, et sh_name contient l'index du nom de la section courante

		//On recupere le nom de la section courrante
		int j =0;
		while((courant = fgetc(elf))!= '\0'){
			currentName[j] = courant;	
			j++;		
		}
		currentName[j] = '\0';

		//Si le nom corespond à la recherche on arrête
		if (strcmp(secName, currentName) == 0) {
			trouve = 1;
		}

		i++;
	}
	
	free(currentName);

	//Une fois la bonne section trouvee on affiche son contenue
	if (trouve) {
		return currentSection;
	} else {
		printf("Aucune section ne correspond à ce nom\n");
		Elf32_Shdr rrr;
		rrr.sh_size = -1;
		return rrr;
	}
}


/*
/ Fonction de recherche d'une recherche de section par son numéro, puis affichage de celle-ci
/ Arguments :
/		-header_elf 		:	header du fichier elf
/		-sections_table 	:	Tableau regroupant toutes les sections
/		-elf 				:	Fichier au format elf
/		-secNum 			:	Numero de la section recherchee
*/
Elf32_Shdr getSectionByIndex(Elf32_Ehdr header_elf, Elf32_Shdr *sections_table, int secNum) {
	if (secNum < 0 || secNum > header_elf.e_shnum-1) {
		printf("Aucune section ne correspond à ce numero\n");
		Elf32_Shdr rrr;
		rrr.sh_size = -1;
		return rrr;
	} else {
		return sections_table[secNum];
	}

}


/*
/ Fonction d'affichage du contenu d'une section
/ Arguments :
/		-currentSection :	Section dont le contenu va etre affiche
/		-elf 			:	Fichier au format elf
*/
void afficheContenue(Elf32_Shdr currentSection, FILE* elf) {
		fseek(elf, currentSection.sh_offset, SEEK_SET); //On se place au début du contenu de la section

		uint16_t hex;
		int sizeFileImage = currentSection.sh_size;
		int address = currentSection.sh_addr;

		//Jusqu'à la fin du fichier, on ecrit octet par octet en hexadecimal
		printf("0x%x ",address);
		for (int i =1; i < (sizeFileImage+1); i++) { 
			hex =(uint8_t) fgetc(elf); //On lit un octet
			
			if (hex <=15) { //Si l'octet est inférieur à 15 on ajoute 0 car sinon il ne l'affiche pas
				printf("0");
			}
			printf("%x",hex);

			//Lie à l'affichage
			if (i % 4 == 0) {
				printf(" ");
			}
			if (i % 16 == 0 && i != sizeFileImage) {
				address+=16;
				printf("\n0x%x ",address);
			}
		}
		printf("\n");
}
