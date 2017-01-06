#include "elf_reader.h"

void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res){
	int i = 0;
	char * currentName = malloc(50);
	char courant;
	int trouve = 0;
	Elf32_Shdr currentSection;
	
	int offsetNameTable = fichierElf->sectionsTable[fichierElf->header_elf.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections

	//On parcour chaque section jusqu'à trouver la bonne ou, toutes les parcourirs
	while (!trouve && i < fichierElf->header_elf.e_shnum) {
		//On se place dans le fichier afin de retouver le nom de la section courante
		currentSection = fichierElf->sectionsTable[i];
		fseek(fichierElf->fichierElf, offsetNameTable + currentSection.sh_name, SEEK_SET); //offset nous ammène au début du stockage des noms, et sh_name contient l'index du nom de la section courante

		//On recupere le nom de la section courrante
		int j =0;
		while((courant = fgetc(fichierElf->fichierElf))!= '\0'){
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
		*res = currentSection;
	} else {
		printf("Aucune section ne correspond à %s\n",secName);
		res = NULL;
	}
}

char * getSectionName(Elf32_Shdr section, FichierElf * fichier) {
	int offsetNameTable = fichier->sectionsTable[fichier->header_elf.e_shstrndx].sh_offset;
	fseek(fichier->fichierElf, offsetNameTable + section.sh_name, SEEK_SET);
	int j =0;
	char * res = malloc(50);
	while((res[j] = fgetc(fichier->fichierElf)) != '\0'){ /////// A voir
		j++;	
	}
	return res;
}

int recupIndexByName(char * sht_name, int e_shstrndx, int nbSections, Elf32_Shdr * sections ,FILE * elf) {
	fseek(elf, sections[e_shstrndx].sh_offset, SEEK_SET);
	printf("\t\t\te_eshdfnkler %d\n",sections[e_shstrndx].sh_offset);
	int j =0, i = 0;
	char * res = malloc(50);
	for (i = 0; i < nbSections; i++) {
		while((res[j] = fgetc(elf)) != '\0'){ /////// A voir
			j++;	
		}
		if (strcmp(sht_name,res) == 0) {
			return i;
		}
	}
	return -1;

}

void fusion(FichierElf *fichierElf1, FichierElf *fichierElf2, FILE *elfRes) {
	printf("\n");
	/////////////////////////////////////////////////////////////////////////
	// Initialisation
	/////////
	printf("Initialisation de la Fusion:\n");
	
	//Structure fichier elf
	Elf32_Ehdr header_elfRes;
	Elf32_Shdr * sectionsTable_elfRes;

	//Autre
	int i,j;
	char * buffer = NULL;
	int newInfo = 0;

	//Tableaux utilisée
	Elf32_Shdr ** sectionsFusion; int nbfusion = 0;
	Elf32_Shdr * sectionsAjout1; int nbAjout1 = 0;
	Elf32_Shdr * sectionsAjout2; int nbAjout2 = 0;

	sectionsFusion = malloc(fichierElf1->header_elf.e_shnum*sizeof(Elf32_Shdr*)); //Il ne peut pas y avoir plus de fusion que le nombre de sections
	for (i = 0; i < fichierElf1->header_elf.e_shnum; i++) {
		sectionsFusion[i] = malloc(2 * sizeof(Elf32_Shdr));
	}
	
	sectionsAjout1 = malloc(fichierElf1->header_elf.e_shnum*sizeof(Elf32_Shdr)); //Il ne peut pas y avoir plus d'ajout que le nombre de sections
	sectionsAjout2 = malloc(fichierElf2->header_elf.e_shnum*sizeof(Elf32_Shdr));


	//Différenciation en les sections à fusionner ou à ajouter
	//On regarde en premier lieu les sections du fichier1 qui sont dans le fichier2
	//Celle qui le sont sont ajouter à sectionsFusion en vue d'être fusionnée (cette première boucle suffit pour récupérer l'ensemble des sections à fusionner)
	//Celle qui ne le sont pas sont ajoutées à sectionsAjout1 en vue d'être simplement copié
	//La seconde boucle for sert à récupérer uniquement les sections original du fichier2 et les placer dans sectionsAjout2
	Elf32_Shdr * tmp = malloc(sizeof(Elf32_Shdr));
	for (i = 0; i < fichierElf1->header_elf.e_shnum; i++) {
		RechercheSectionByName(fichierElf2, getSectionName(fichierElf1->sectionsTable[i],fichierElf1), tmp);
		if (tmp != NULL) {
			sectionsFusion[nbfusion][0] = fichierElf1->sectionsTable[i];
			sectionsFusion[nbfusion][1] = *tmp;
			nbfusion++;
		} else {
			sectionsAjout1[nbAjout1] = fichierElf1->sectionsTable[i];
			nbAjout1++;
		}

	}
	for (i = 0; i < fichierElf2->header_elf.e_shnum; i++) {
		RechercheSectionByName(fichierElf1, getSectionName(fichierElf2->sectionsTable[i],fichierElf2), tmp);
		if (tmp == NULL) {
			sectionsAjout2[nbAjout2] = fichierElf2->sectionsTable[i];
			nbAjout2++;
		}
	}

	printf("\tOK\n");
	/////////////////////////////////////////////////////////////////////

	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture des sections
	////////////////////////
	printf("Ecriture des Sections et Création table des sections : \n");
	printf("\tnbFusion :%d\n",nbfusion);
	//On place le curseur sur le 53e octets, on écrira le header plus tard
	fseek(elfRes, 52, SEEK_SET);
	//On commence à ecrire les sections qui doivents être fusionnées
	char ** shstrtab = malloc((nbAjout1 + nbAjout2 + nbfusion)*sizeof(char*));
	for (i = 0; i < (nbAjout1 + nbAjout2 + nbfusion); i++) {
		shstrtab[i] = malloc(50);
	}
	int offsetCourantshstrtab = 0;
	int octetsRestants = 0;
	int sectionsEcrites = 0;
	int shstrtabRencontre =0;
	int indexShstrtab = 0;
	char c;
	sectionsTable_elfRes = malloc((nbAjout1 + nbAjout2 + nbfusion)*sizeof(Elf32_Shdr));

	//Fusion des sections de meme noms (on s'occupera de shstrtab à la fin car pour l'écrire il faut parcourir toutes les autres sections)
	for (i = 0; i < nbfusion; i++) {
		shstrtab[i] = getSectionName(sectionsFusion[i][0],fichierElf1);
		printf("\t\t%s\n",shstrtab[i]);
		if (strcmp(shstrtab[i], ".shstrtab") != 0) {
			if (strcmp(shstrtab[i], ".shstrndx") != 0) {
				indexShstrtab = i-shstrtabRencontre;
			}
			sectionsTable_elfRes[i-shstrtabRencontre].sh_name = offsetCourantshstrtab; //Le offset du sh_name correspond à la somme de tous les noms des sections écrites
			sectionsTable_elfRes[i-shstrtabRencontre].sh_offset = ftell(elfRes); //On sauvegarde la position courante du fichier elfRes dans le offset de la section en cours d'écriture
			sectionsTable_elfRes[i-shstrtabRencontre].sh_size = sectionsFusion[i][0].sh_size + sectionsFusion[i][1].sh_size; //Concatenation simple donc on ajoute les deux tailles
			sectionsTable_elfRes[i-shstrtabRencontre].sh_flags = sectionsFusion[i][0].sh_flags | sectionsFusion[i][1].sh_flags; //Ou logique pour avoir les flags des deux sections
			if (sectionsFusion[i][0].sh_type != sectionsFusion[i][1].sh_type) {
				printf("\t\t\tpas le meme type : %d et %d\n",sectionsFusion[i][0].sh_type,sectionsFusion[i][1].sh_type);
			} else {
				sectionsTable_elfRes[i-shstrtabRencontre].sh_type = sectionsFusion[i][0].sh_type;
			}

			sectionsTable_elfRes[i-shstrtabRencontre].sh_addr = 0;

			if (sectionsFusion[i][0].sh_link != sectionsFusion[i][1].sh_link) {
				printf("\t\t\tpas le meme sh_link : %d et %d\n",sectionsFusion[i][0].sh_link,sectionsFusion[i][1].sh_link);
				sectionsTable_elfRes[i-shstrtabRencontre].sh_link = SHN_UNDEF;		
			} else {
				sectionsTable_elfRes[i-shstrtabRencontre].sh_type = sectionsFusion[i][0].sh_type;
				printf("\t\t\tsh_link %d\n",sectionsFusion[i][0].sh_type);
			}


			if (sectionsFusion[i][0].sh_info != sectionsFusion[i][1].sh_info) {
				printf("\t\t\tpas le meme sh_info : %d et %d\n",sectionsFusion[i][0].sh_info,sectionsFusion[i][1].sh_info);
				if (sectionsFusion[i][0].sh_type == sectionsFusion[i][1].sh_type && sectionsFusion[i][1].sh_type == SHT_SYMTAB) {
					newInfo = sectionsFusion[i][0].sh_info + sectionsFusion[i][1].sh_info;
					sectionsTable_elfRes[i-shstrtabRencontre].sh_info = newInfo;
				}
			} else {
				if (sectionsFusion[i][0].sh_type == sectionsFusion[i][1].sh_type && sectionsFusion[i][1].sh_type == SHT_SYMTAB) {
					newInfo = sectionsFusion[i][0].sh_info + sectionsFusion[i][1].sh_info;
					sectionsTable_elfRes[i-shstrtabRencontre].sh_info = newInfo;
				}
				sectionsTable_elfRes[i-shstrtabRencontre].sh_info = sectionsFusion[i][0].sh_info;
			}
			if (sectionsFusion[i][0].sh_addralign != sectionsFusion[i][1].sh_addralign) {
				printf("\t\t\tpas le meme sh_addralign : %d et %d\n",sectionsFusion[i][0].sh_addralign,sectionsFusion[i][1].sh_addralign);
			} else {
				sectionsTable_elfRes[i-shstrtabRencontre].sh_addralign = sectionsFusion[i][0].sh_addralign;
			}
			if (sectionsFusion[i][0].sh_entsize != sectionsFusion[i][1].sh_entsize) {
				printf("\t\t\tpas le meme sh_entsize : %d et %d\n",sectionsFusion[i][0].sh_entsize,sectionsFusion[i][1].sh_entsize);
			} else {
				sectionsTable_elfRes[i-shstrtabRencontre].sh_entsize = sectionsFusion[i][0].sh_entsize;
			}

			fseek(fichierElf1->fichierElf,sectionsFusion[i][0].sh_offset,SEEK_SET); //On se place au contenu de la section du fichier 1
			//Ecriture de la première section
			octetsRestants = sectionsFusion[i][0].sh_size;
			for (int i =0; i < octetsRestants; i++) { 
				fputc(fgetc(fichierElf1->fichierElf),elfRes); //On ecrit un octet
			}
			fseek(fichierElf2->fichierElf,sectionsFusion[i][1].sh_offset,SEEK_SET); //On se place au contenu de la section du fichier 2
			//Ecriture de la première section
			octetsRestants = sectionsFusion[i][1].sh_size;
			for (int i =0; i < octetsRestants; i++) { 
				fputc(fgetc(fichierElf1->fichierElf),elfRes); //On ecrit un octet
			}
		} else {
			shstrtabRencontre++;
			shstrtab[nbAjout1 + nbAjout2 + nbfusion - 1] = ".shstrtab\0";
			sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_name = offsetCourantshstrtab;
			sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_addr = 0;
			sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_link = SHN_UNDEF;
			if (sectionsFusion[i][0].sh_type != sectionsFusion[i][1].sh_type) {
				printf("\t\t\tpas le meme type : %d et %d\n",sectionsFusion[i][0].sh_type,sectionsFusion[i][1].sh_type);
			} else {
				sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_type = sectionsFusion[i][0].sh_type;
			}
			if (sectionsFusion[i][0].sh_info != sectionsFusion[i][1].sh_info) {
				printf("\t\t\tpas le meme sh_info : %d et %d\n",sectionsFusion[i][0].sh_info,sectionsFusion[i][1].sh_info);
			} else {
				sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_info = sectionsFusion[i][0].sh_info;
			}
			if (sectionsFusion[i][0].sh_addralign != sectionsFusion[i][1].sh_addralign) {
				printf("\t\t\tpas le meme sh_addralign : %d et %d\n",sectionsFusion[i][0].sh_addralign,sectionsFusion[i][1].sh_addralign);
			} else {
				sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_addralign = sectionsFusion[i][0].sh_addralign;
			}
			if (sectionsFusion[i][0].sh_entsize != sectionsFusion[i][1].sh_entsize) {
				printf("\t\t\tpas le meme sh_entsize : %d et %d\n",sectionsFusion[i][0].sh_entsize,sectionsFusion[i][1].sh_entsize);
			} else {
				sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_entsize = sectionsFusion[i][0].sh_entsize;
			}
		}
		offsetCourantshstrtab += strlen(shstrtab[i]) + 1;
	}
	sectionsEcrites = nbfusion;
	printf("\tFusion des sections realisee\n");



	printf("\tnbAjout1 :%d\n",nbAjout1);
	//Ecritures des sections uniques
	for (i = 0; i < nbAjout1; i++) {
		shstrtab[i] = getSectionName(sectionsAjout1[i],fichierElf1); //On récupère son nom pour l'écriture de la section shstrtab
		printf("\t\tHeader %d   offset name %d\n",i+sectionsEcrites,offsetCourantshstrtab);
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_name = offsetCourantshstrtab; //Le offset du sh_name correspond à la somme de tous les noms des sections écrites
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_offset = ftell(elfRes); //On sauvegarde la position courante du fichier elfRes dans le offset de la section en cours d'écriture
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_size = sectionsAjout1[i].sh_size; //simple copie de la tailles
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_flags = sectionsAjout1[i].sh_flags; //simple copie des flags

		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_addr = 0;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_link = SHN_UNDEF;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_info = sectionsAjout1[i].sh_info;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_addralign = sectionsAjout1[i].sh_addralign;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_entsize = sectionsAjout1[i].sh_entsize;

		fseek(fichierElf1->fichierElf,sectionsAjout1[i].sh_offset,SEEK_SET); //On se place au contenu de la section du fichier 1
		//Ecriture de la section
		octetsRestants = sectionsAjout1[i].sh_size;
		for (int i =0; i < octetsRestants; i++) { 
			fputc(fgetc(fichierElf1->fichierElf),elfRes); //On ecrit un octet*
		}
		printf("\t\t\tIncrementation de l'offset name de %ld\n",strlen(shstrtab[i+sectionsEcrites]) + 1);
		offsetCourantshstrtab += strlen(shstrtab[i+sectionsEcrites]) + 1 ;
	}
	sectionsEcrites+=nbAjout1;
	printf("\tAjout des sections1 realisee\n");

	printf("\tnbAjout2 :%d\n",nbAjout2);
	for (i = 0; i < nbAjout2; i++) {
		shstrtab[i] = getSectionName(sectionsAjout2[i],fichierElf2); //On récupère son nom pour l'écriture de la section shstrtab
		printf("\t\tHeader %d   offset name %d\n",i+sectionsEcrites,offsetCourantshstrtab);
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_name = offsetCourantshstrtab; //Le offset du sh_name correspond à la somme de tous les noms des sections écrites
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_offset = ftell(elfRes); //On sauvegarde la position courante du fichier elfRes dans le offset de la section en cours d'écriture
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_size = sectionsAjout2[i].sh_size; //simple copie de la tailles
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_flags = sectionsAjout2[i].sh_flags; //simple copie des flags

		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_addr = 0;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_link = SHN_UNDEF;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_info = sectionsAjout1[i].sh_info;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_addralign = sectionsAjout1[i].sh_addralign;
		sectionsTable_elfRes[i+sectionsEcrites-shstrtabRencontre].sh_entsize = sectionsAjout1[i].sh_entsize;

		fseek(fichierElf2->fichierElf,sectionsAjout2[i].sh_offset,SEEK_SET); //On se place au contenu de la section du fichier 2
		//Ecriture de la section
		octetsRestants = sectionsAjout2[i].sh_size;
		for (int i =0; i < octetsRestants; i++) { 
			fputc(fgetc(fichierElf1->fichierElf),elfRes); //On ecrit un octet
		}
		//inutile d'incrémenter sectionsEcrites, on en a plus besoin
		offsetCourantshstrtab += strlen(shstrtab[i+sectionsEcrites]) + 1 ;
	}
	printf("\tAjout des sections2 realisee\n");
	

	//finalement on s'occupe de shstrtab
	sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_offset = ftell(elfRes); //On sauvegarde la position courante du fichier elfRes dans le offset de la section en cours d'écriture
	sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_size = offsetCourantshstrtab + strlen(shstrtab[nbAjout1 + nbAjout2 + nbfusion - 1]); //Sa taille correspond au nom de toutes les sections, elle comprise
	printf("\t\toffset shstrtab %d   offset name shstrtab : %d\n",sectionsTable_elfRes[nbAjout1 + nbAjout2 + nbfusion - 1].sh_offset,offsetCourantshstrtab);
	for (i = 0; i < (nbAjout1 + nbAjout2 + nbfusion); i++) {
		j = 0;
		while ((c = shstrtab[i][j]) != '\0') {
			fputc(c,elfRes);
			j++;
		}
		fputc(c,elfRes);
	}						 


	printf("\tOK\n");
	/////////////////////////////////////////////////////////////////////


	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture de la table des sections
	////////////////////////////////////
	printf("Ecriture de la table des Sections :\n");
	int shoff = ftell(elfRes); //On sauvegarde l'offset du début de la table des sections pour le header
	for (i = 0; i < (nbAjout1 + nbAjout2 + nbfusion); i++) {
		buffer = (char *) &sectionsTable_elfRes[i];
		for (j = 0; j < 40; j++) {
			fputc(*buffer,elfRes);
			buffer++;
		}

	}
/*
	int tmp2 = recupIndexByName(".strtab", indexShstrtab, header_elfRes.e_shnum,sectionsTable_elfRes, elfRes);
	if (tmp2 != -1) {
		printf("\t\tIndex strtab %d",tmp2);
	} else {
		printf("\t\trecupIndexByName index non trouve");
	}*/

	printf("\tOK\n");
	/////////////////////////////////////////////////////////////////////

	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Création du header pour le nouveau fichier
	////////////////////////////////////////////
	printf("Création du header :\n");
	//e_ident[EI_MAG0-3]
	int isElf = 1;
	if (fichierElf1->header_elf.e_ident[EI_MAG0] != fichierElf2->header_elf.e_ident[EI_MAG0] && fichierElf2->header_elf.e_ident[EI_MAG0] != 0x7f) isElf = 0;
	if (fichierElf1->header_elf.e_ident[EI_MAG1] != fichierElf2->header_elf.e_ident[EI_MAG1] && fichierElf2->header_elf.e_ident[EI_MAG1] != 0x45) isElf = 0;
	if (fichierElf1->header_elf.e_ident[EI_MAG2] != fichierElf2->header_elf.e_ident[EI_MAG2] && fichierElf2->header_elf.e_ident[EI_MAG2] != 0x4c) isElf = 0;
	if (fichierElf1->header_elf.e_ident[EI_MAG3] != fichierElf2->header_elf.e_ident[EI_MAG3] && fichierElf2->header_elf.e_ident[EI_MAG3] != 0x46) isElf = 0;
	if (!isElf) {
		printf("Un des deux fichiers n'est pas un fichier elf !\n");
	} else {
		header_elfRes.e_ident[EI_MAG0] = 0x7f;
		header_elfRes.e_ident[EI_MAG1] = 0x45;
		header_elfRes.e_ident[EI_MAG2] = 0x4c;
		header_elfRes.e_ident[EI_MAG3] = 0x46;
	}

	//e_ident[EI_CLASS]
	if (fichierElf1->header_elf.e_ident[EI_CLASS] != 1) {
		printf("Le fichier 1 n'est pas en 32 bit\n");
	}else if (fichierElf2->header_elf.e_ident[EI_CLASS] != 1){
		printf("Le fichier 2 n'est pas en 32 bit\n");
	} else {
		header_elfRes.e_ident[EI_CLASS] = 1;
	}

	//e_ident[EI_DATA]
	header_elfRes.e_ident[EI_DATA] = 1; //Ballec des vérif pour l'endianness

	//e_ident[EI_VERSION]
	if (fichierElf1->header_elf.e_ident[EI_VERSION] != fichierElf2->header_elf.e_ident[EI_VERSION]) {
		printf("Les e_ident[EI_VERSION] du fichier 1 et 2 n'ont pas la meme version. Qué qu'on fait ??????????\n");
	} else {
		header_elfRes.e_ident[EI_VERSION] = fichierElf1->header_elf.e_ident[EI_VERSION];
	}

	//e_ident[EI_OSABI]
	header_elfRes.e_ident[EI_OSABI] = 0; //Ballec des vérif pour le target OS


	//Bourrage pour e_ident[EI_ABIVERSION] et e_ident[EI_PAD] 
	buffer = (char * ) &header_elfRes.e_ident[EI_OSABI];
	for (i = 0; i < 8; i++) {
		*buffer = 0;
		buffer++;
	}


	//e_type
	if (fichierElf1->header_elf.e_type != fichierElf2->header_elf.e_type) {
		printf("Les e_type du fichier 1 et 2 ne sont pas les memes\n");
	} else {
		header_elfRes.e_type = fichierElf1->header_elf.e_type;
	}

	//e_machine
	if (fichierElf1->header_elf.e_machine != fichierElf2->header_elf.e_machine) {
		printf("Les fichiers 1 et 2 n'ont pas la meme cible d'architecture\n");
	} else {
		header_elfRes.e_machine = fichierElf1->header_elf.e_machine;
	}

	//e_version
	if (fichierElf1->header_elf.e_version != fichierElf2->header_elf.e_version) {
		printf("Les e_type du fichier 1 et 2 ne sont pas les meme. Qué qu'on fait ??????????\n");
	} else {
		header_elfRes.e_version = fichierElf1->header_elf.e_version;
	}

	//e_entry
	if (fichierElf1->header_elf.e_entry != fichierElf2->header_elf.e_entry) {
		printf("Les fichiers 1 et 2 n'ont pas le meme e_entry\n");
	} else {
		header_elfRes.e_entry = fichierElf1->header_elf.e_entry;
	}

	//e_phoff
	if (fichierElf1->header_elf.e_phoff != fichierElf2->header_elf.e_phoff) {
		printf("Les fichiers 1 et 2 n'ont pas le meme offset pour le Program Header, Normal non ?????????????????\n");
	} else {
		header_elfRes.e_phoff = fichierElf1->header_elf.e_phoff;
	}
	//JE SAIS PAS COMMENT CETTE TABLE FONCTIONNE

	//e_shoff
	header_elfRes.e_shoff = shoff;

	//e_flags
	if (fichierElf1->header_elf.e_flags != fichierElf2->header_elf.e_flags) {
		printf("Les fichiers 1 et 2 n'ont pas les memes flags, concatenation ?????????????????\n");
	} else {
		header_elfRes.e_flags = fichierElf1->header_elf.e_flags;
	}

	//e_ehsize
	if (fichierElf1->header_elf.e_ehsize != 52) {
		printf("Le header du fichier 1 n'est pas à 52 octets\n");
	} else if (fichierElf2->header_elf.e_ehsize != 52) {
		printf("Le header du fichier 1 n'est pas à 52 octets\n");
	}else {
		header_elfRes.e_ehsize = fichierElf1->header_elf.e_ehsize;
	}

	//e_phentsize
	if (fichierElf1->header_elf.e_phentsize != fichierElf2->header_elf.e_phentsize) {
		printf("Les fichiers 1 et 2 n'ont pas la meme taille de Program Header, Normal non ??????????????\n");
	} else {
		header_elfRes.e_phentsize = fichierElf1->header_elf.e_phentsize;
	}
	//JE SAIS PAS COMMENT CETTE TABLE FONCTIONNE

	//e_phnum
	if (fichierElf1->header_elf.e_phnum != fichierElf2->header_elf.e_phnum) {
		printf("Les fichiers 1 et 2 n'ont pas le meme nombre d'entree dans le Program Header, Normal non ??????????????\n");
	} else {
		header_elfRes.e_phnum = fichierElf1->header_elf.e_phnum;
	}
	//JE SAIS PAS COMMENT CETTE TABLE FONCTIONNE

	//e_shentsize
	if (fichierElf1->header_elf.e_shentsize != fichierElf2->header_elf.e_shentsize) {
		printf("Les fichiers 1 et 2 n'ont pas la meme taille pour la table des sections, Normal non ??????????????\n");
	} else {
		header_elfRes.e_shentsize = fichierElf1->header_elf.e_shentsize;
		printf("\te_shentsize %d      %d\n",header_elfRes.e_shentsize,fichierElf2->header_elf.e_shentsize);
	}

	//e_shnum
	header_elfRes.e_shnum = nbfusion + nbAjout1 + nbAjout2;

	//e_shstrndx
	header_elfRes.e_shstrndx = nbfusion + nbAjout1 + nbAjout2 -1;

	printf("\tOK\n");

	printf("\n");
	//ECRITURE
	printf("Ecriture du header :\n");
	//On se replace au début du fichier
	fseek(elfRes, 0, SEEK_SET);
	/*fputc(0x7f,elfRes);
	fputc(0x45,elfRes);
	fputc(0x4c,elfRes);
	fputc(0x46,elfRes);
	fputc(header_elfRes.e_ident[EI_CLASS],elfRes);
	fputc(header_elfRes.e_ident[EI_DATA],elfRes);
	fputc(header_elfRes.e_ident[EI_VERSION],elfRes);
	fputc(header_elfRes.e_ident[EI_OSABI],elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);
	fputc(0,elfRes);

	char * buffer =(char *) &header_elfRes.e_type;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_machine;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_version;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes); buffer++;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_entry;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes); buffer++;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_phoff;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes); buffer++;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_shoff;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes); buffer++;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);

	buffer =(char *) &header_elfRes.e_flags;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes); buffer++;
	fputc(*buffer,elfRes); 	buffer++;	fputc(*buffer,elfRes);
*/
	buffer = (char *) &header_elfRes;
	for (i = 0; i < 52; i++) {
		fputc(*buffer,elfRes);
		buffer++;
	}



	printf("\tOK\n");
	/////////////////////////////////////////////////////////////////////

}