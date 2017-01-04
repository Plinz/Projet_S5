#include "elf_reader.h"

void afficheSectionByName(Elf32_Ehdr fileHeader, Elf32_Shdr *sections_headers, FILE* elf, char * secName){
	int i = 0;
	char * currentName = malloc(40);
	char courant;
	int trouve = 0;
	Elf32_Shdr currentSection;
	
	int offsetNameTable = sections_headers[fileHeader.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections

	//On parour chaque section jusqu'à trouver la bonne ou, toute les parcourir
	while (!trouve && i < fileHeader.e_shnum) {
		//On se place dans le fichier afin de retouver le nom de la section courante
		currentSection = sections_headers[i];
		fseek(elf, offsetNameTable + currentSection.sh_name, SEEK_SET); //offset nous ammène au début du stockage des noms, et sh_name contient l'index du nom de la section courante

		//On récupère le nom de la section courrante
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

	//Une fois la bonne section trouvee on affiche son contenue
	if (trouve) {
		fseek(elf, currentSection.sh_offset, SEEK_SET); //On se place au début du contenu de la section
		uint8_t hex;
		int sizeFileImage = currentSection.sh_size;
		//Jusqu'à la fin du fichier, on écrit octet par octet en hexadecimal
		printf("0x");
		for (i =1; i < sizeFileImage+1; i++) { 
			/*if (i % 20 == 0) {
				printf("\n0x");
			}else */if (i % 4 == 0) {
				printf(" ");
			}
			//printf("\n");
			hex =(uint8_t) fgetc(elf);
			if (hex <=15) {
				printf("0");
			}
			printf("%x",hex);
		}

	} else {
		printf("Aucune section ne correspond à ce nom\n");
	}
}


/*
void afficheSectionByNum(int secNum) {

}*/