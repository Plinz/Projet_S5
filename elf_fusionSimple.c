#include "elf_reader.h"
#include "elf_fusion.h"

Elf32_Ehdr header(FichierElf *fichierElf1, FichierElf *fichierElf2) {
  Elf32_Ehdr header_elfRes;


  //e_ident[EI_MAG0-3]
  int isElf = 1;
  if (fichierElf1->header_elf.e_ident[EI_MAG0] != fichierElf2->header_elf.e_ident[EI_MAG0] && fichierElf2->header_elf.e_ident[EI_MAG0] != 0x7f) isElf = 0;
  if (fichierElf1->header_elf.e_ident[EI_MAG1] != fichierElf2->header_elf.e_ident[EI_MAG1] && fichierElf2->header_elf.e_ident[EI_MAG1] != 0x45) isElf = 0;
  if (fichierElf1->header_elf.e_ident[EI_MAG2] != fichierElf2->header_elf.e_ident[EI_MAG2] && fichierElf2->header_elf.e_ident[EI_MAG2] != 0x4c) isElf = 0;
  if (fichierElf1->header_elf.e_ident[EI_MAG3] != fichierElf2->header_elf.e_ident[EI_MAG3] && fichierElf2->header_elf.e_ident[EI_MAG3] != 0x46) isElf = 0;
  if (!isElf) {
    printf("Un des deux fichiers n'est pas un fichier elf !\n");
  exit(1);
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
  char * buffer = (char * ) &header_elfRes.e_ident[EI_OSABI];
  for (int i = 0; i < 8; i++) {
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
    printf("Les fichiers 1 et 2 n'ont pas la meme taille de Program Header\n");
  } else {
    header_elfRes.e_phentsize = fichierElf1->header_elf.e_phentsize;
  }

  //e_phnum
  if (fichierElf1->header_elf.e_phnum != fichierElf2->header_elf.e_phnum) {
    printf("Les fichiers 1 et 2 n'ont pas le meme nombre d'entree dans le Program Header\n");
  } else {
    header_elfRes.e_phnum = fichierElf1->header_elf.e_phnum;
  }

  //e_shentsize
  if (fichierElf1->header_elf.e_shentsize != fichierElf2->header_elf.e_shentsize) {
    printf("Les fichiers 1 et 2 n'ont pas la meme taille pour la table des sections\n");
  } else {
    header_elfRes.e_shentsize = fichierElf1->header_elf.e_shentsize;
  }

  return header_elfRes;
}

void sectionShstrtab(Section *section, Strtab * shstrtab) {
	printf("\nCreation Shstrtab : \n");
	section->nbOctets = shstrtab->offsetCourant;
	section->contenu = malloc(section->nbOctets);
	section->header.sh_size = section->nbOctets;

	int j;
	int curseur = 0;
	for (int i = 0; i < shstrtab->nbNames; i++) {
		j = 0;
		printf("\t\t");
		while ((section->contenu[curseur] = shstrtab->names[i][j]) != '\0') {
			j++;
			printf("%c",section->contenu[curseur]);
			curseur++;
		}
	printf("\n");
	curseur++;
	}
	printf("Ok\n\n");
}

void sectionStrtab(Section *section, Strtab * strtab) {
	printf("\nCreation Strtab : \n");
	section->nbOctets = strtab->offsetCourant;
	section->contenu = malloc(section->nbOctets);
	section->header.sh_size = section->nbOctets;
	printf("====================== strtab size %d\n",section->header.sh_size);

	int j;
	int curseur = 0;
	for (int i = 0; i < strtab->nbNames; i++) {
		j = 0;
		printf("\t\t%x\t",curseur);
		while ((section->contenu[curseur] = strtab->names[i][j]) != '\0') {
			j++;
			printf("%c",section->contenu[curseur]);
			curseur++;
		}
		printf("\n");
		curseur++;
	}
	printf("Ok\n\n");
}

void sectionFusionSimple(Section *fusion, Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2) {
	fusion->nbOctets = sectionHeader1.sh_size + sectionHeader2.sh_size;
	fusion->contenu = malloc(fusion->nbOctets);

	//Concatenation
	int i;
	fseek(fichierElf1->fichierElf, sectionHeader1.sh_offset, SEEK_SET);
	for (i = 0; i < (int)sectionHeader1.sh_size; i++) {
		fusion->contenu[i] = fgetc(fichierElf1->fichierElf);
	}
	int j;
	fseek(fichierElf2->fichierElf, sectionHeader2.sh_offset, SEEK_SET);
	for (j = 0; j < (int)sectionHeader2.sh_size; j++) {
		fusion->contenu[i+j] = fgetc(fichierElf2->fichierElf);
	}

	//Creation Header
	fusion->header.sh_type = sectionHeader1.sh_type;
	fusion->header.sh_size = fusion->nbOctets;
	fusion->header.sh_addr = 0;
	fusion->header.sh_link = SHN_UNDEF;
	fusion->header.sh_info = 0;
	if (sectionHeader1.sh_addralign != sectionHeader2.sh_addralign) {
	  printf("\t\tpas le meme sh_addralign : %d et %d\n",sectionHeader1.sh_addralign,sectionHeader2.sh_addralign);
	} else {
	   fusion->header.sh_addralign = sectionHeader1.sh_addralign;
	}
	if (sectionHeader1.sh_entsize != sectionHeader2.sh_entsize) {
	  printf("\t\tpas le meme sh_entsize : %d et %d\n",sectionHeader1.sh_entsize,sectionHeader2.sh_entsize);
	} else {
	  fusion->header.sh_entsize = sectionHeader1.sh_entsize;
	}
	//sh_offset sera etabli à l'écriture du fichier
}

Section sectionfusion(Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2, FichierElf * fichierElfRes, Strtab * shstrtab, Strtab * strtab) {
	Section sectionfusionee;
	
	
	//uniquement utile pour la fusion des symboles
	int Size1 = getNbSymbols(fichierElf1);
	int Size2 = getNbSymbols(fichierElf2);
	int nbSymbolTotal = Size1+Size2;
	

	//printf("\tFusion %s\n",shstrtab->names[shstrtab->nbNames]);

	switch(sectionHeader1.sh_type) {
		case SHT_SYMTAB :

			fichierElfRes->tabSymbole = malloc(nbSymbolTotal*sizeof(Symbole));
			nbSymbolTotal = fusionTableSymbole(*fichierElf1, *fichierElf2, Size1, Size2, fichierElfRes->tabSymbole, strtab);
			sectionfusionee = creerSectionTableSymbole(fichierElfRes->tabSymbole, nbSymbolTotal, sectionHeader1, sectionHeader2);
			break;
		case SHT_REL :
			//Fusion de deux Table des Relocations 

			break;
		default : //Pour l'instant, on fait partout pareil
		
			sectionFusionSimple(&sectionfusionee, sectionHeader1, sectionHeader2, fichierElf1, fichierElf2);
		break;
	}
	//Traitement de du nom de la section
	shstrtab->names[shstrtab->nbNames] = getSectionName(sectionHeader1, fichierElf1);
	sectionfusionee.header.sh_name = shstrtab->offsetCourant;
	sectionfusionee.header.sh_flags = sectionHeader1.sh_flags;
	
	sectionfusionee.header.sh_addr = 0;
	
	printf("\t\tsh_name %d\n",shstrtab->offsetCourant);
	shstrtab->offsetCourant += strlen(shstrtab->names[shstrtab->nbNames]) + 1;
	shstrtab->nbNames++;

	printf("\t\tNbOctets %d\n",sectionfusionee.nbOctets);


	return sectionfusionee;
}

Section SectionAjout(Elf32_Shdr sectionHeader, FichierElf * fichierElf, Strtab * shstrtab) {
  Section section;
  section.nbOctets = sectionHeader.sh_size;
  section.contenu = malloc(sectionHeader.sh_size);

  //Copie de la section
  fseek(fichierElf->fichierElf, sectionHeader.sh_offset, SEEK_SET);
  for (int i =0; i < (int)sectionHeader.sh_size; i++) {
    section.contenu[i] = fgetc(fichierElf->fichierElf);
  }

  shstrtab->names[shstrtab->nbNames] = getSectionName(sectionHeader, fichierElf);

  printf("\t\tAjout %s\n",shstrtab->names[shstrtab->nbNames]);

  //header
  section.header = sectionHeader;
  section.header.sh_name = shstrtab->offsetCourant;
  section.header.sh_addr = 0;

  shstrtab->offsetCourant += strlen(shstrtab->names[shstrtab->nbNames]) + 1;
  shstrtab->nbNames++;

  return section;
}

void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res, int * present){
	int i = 0;
	char * currentName = malloc(50);
	char courant;
	Elf32_Shdr currentSection;
	*present = 0;
	int offsetNameTable = fichierElf->sectionsTable[fichierElf->header_elf.e_shstrndx].sh_offset; //On prend l'offset de la section contenant les noms de sections

	//On parcour chaque section jusqu'à trouver la bonne ou, toutes les parcourirs
	while (!*present && i < fichierElf->header_elf.e_shnum) {
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
			*present = 1;
			*res = currentSection;
		}
		i++;
	}
	free(currentName);
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

void ecritureFichierFusionnee(FichierElf *fichierElfRes, Section * sections_elfRes, Strtab * shstrtab) {
	char * buffer = malloc(1);
	char *init = buffer;	
	int i = 0, j =0;

	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture des sections
	////////////////////////
	printf("\tEcriture des %d Sections : \n", fichierElfRes->header_elf.e_shnum);

	//Ecriture du contenu des sections
	//On place le curseur sur le 53e octets, on écrira le header plus tard
	fseek(fichierElfRes->fichierElf, 52, SEEK_SET);
	for (i = 0; i < fichierElfRes->header_elf.e_shnum; i++) {
		printf("\t\t%s : \n",shstrtab->names[i]);
		fichierElfRes->sectionsTable[i] = sections_elfRes[i].header;
		fichierElfRes->sectionsTable[i].sh_offset = ftell(fichierElfRes->fichierElf);
		fichierElfRes->sectionsTable[i].sh_size = sections_elfRes[i].nbOctets;
		printf("\t\t\tsh_offset 0x%x\n", fichierElfRes->sectionsTable[i].sh_offset );
		printf("\t\t\tnbOctets %d\n",sections_elfRes[i].nbOctets);
		//Ecriture du contenu de la table
		for (j = 0; j < (int) sections_elfRes[i].nbOctets ; j++) {
			fputc(sections_elfRes[i].contenu[j], fichierElfRes->fichierElf);
		}
	}

	printf("\tOK\n");
	/////////////////////////////////////////////////////////////////////

	printf("\n\tEcriture table des sections : \n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture de la table des sections
	////////////////////////////////////
	//printf("\tEcriture de la table des Sections : \n");
	int shoff = ftell(fichierElfRes->fichierElf); //On sauvegarde l'offset du début de la table des sections pour le header
	for (i = 0; i < fichierElfRes->header_elf.e_shnum; i++) {
		buffer = (char *) &fichierElfRes->sectionsTable[i];
		for (j = 0; j < 40; j++) {
			fputc(*buffer,fichierElfRes->fichierElf);
		buffer++;
		}
	}
	printf("\tOK\n");

	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture du header pour le nouveau fichier
	////////////////////////////////////////////
	printf("\tEcriture du header : \n");

	//e_shoff
	fichierElfRes->header_elf.e_shoff = shoff;
	//On se positionne au debut du fichier
	fseek(fichierElfRes->fichierElf, 0, SEEK_SET);

	buffer = (char *) &fichierElfRes->header_elf;
	for (i = 0; i < 52; i++) {
		fputc(*buffer,fichierElfRes->fichierElf);
		buffer++;
	}

	free(init);
	printf("\tOK\nOK\n");
	/////////////////////////////////////////////////////////////////////
}

int getNbSymbols(FichierElf *f) {
	Elf32_Shdr sectionHeaderSymtab;
	int present = 0;
	RechercheSectionByName(f, ".symtab", &sectionHeaderSymtab, &present);
	int nbSymb =  sectionHeaderSymtab.sh_size / sectionHeaderSymtab.sh_entsize;
	//printf("NbSymb : %d\n", nbSymb);
	if (present) {
		return nbSymb;

	} else {
		return -1;
	}
}

void fusion(FichierElf *fichierElf1, FichierElf *fichierElf2, FichierElf *fichierElfRes) {
	getNbSymbols(fichierElf1);
	printf("\n");
	/////////////////////////////////////////////////////////////////////////
	// Initialisation
	/////////
	//printf("Initialisation de la Fusion:\n");

	//Structure fichier elf
	Section * sections_elfRes;

	//Autre
	int i;
	int nbSectionMax = fichierElf1->header_elf.e_shnum + fichierElf2->header_elf.e_shnum;
	int nbSectionEcrites = 0;

	//Tableau qui contient les index des sections de type REL et REAL
	int * indexesREL = malloc(nbSectionMax * sizeof(int)); //Il ne peut pas y avoir plus de sections dans le fichier final que dans les deux fichier à fusioner confondu
	int * indexesREL1 = malloc(nbSectionMax * sizeof(int));
	int * indexesREL2 = malloc(nbSectionMax * sizeof(int));
	int nbSectionTypeRel = 0;

	//Tableau qui contient les Sections{contenu,header} qu'on ecrira dans le fichier resultat
	sections_elfRes = malloc(nbSectionMax * sizeof(Section));

	//Structure qui va contenir le nom de toutes les sections
	Strtab *shstrtab = malloc(sizeof(Strtab));
	shstrtab->names = malloc(nbSectionMax * sizeof(char*));
	for (i = 0; i < (nbSectionMax); i++) {
	shstrtab->names[i] = malloc(50);
	}
	shstrtab->offsetCourant = 0;
	shstrtab->nbNames = 0;


	//Structure qui va contenir le nom de tous les symboles
	Strtab *strtab = malloc(sizeof(Strtab));
	strtab->names = malloc((getNbSymbols(fichierElf1) + getNbSymbols(fichierElf2))*sizeof(char*));
	for (i = 0; i < (getNbSymbols(fichierElf1) + getNbSymbols(fichierElf2)); i++) {
	strtab->names[i] = malloc(50);
	}
	strtab->offsetCourant = 0;
	strtab->nbNames = 0;


	//Différenciation en les sections à fusionner ou à ajouter
	//Si une section est présente dans les deux fichiers on la fusionne
	//Sinon on l'ajoute
	//Obligé de faire une seconde boucle pour voir les sections du fichier2 qui ne sont pas dans le fichier1
	int present = 0;
	int indexShstrtab = -1;
	int indexStrtab = -1;
	int indexSymtab = -1;
	Elf32_Shdr **SectionHeaderRel = malloc(80 * sizeof(Elf32_Shdr* ));
	for (int kl = 0; kl < 80; kl++) {
		SectionHeaderRel[kl] = malloc(sizeof(Elf32_Shdr) * 2);
	}
	Elf32_Shdr *SectionHeaderRelSeul1 = malloc(80 * sizeof(Elf32_Shdr));
	int nbSectionTypeRelSeul1 = 0;
	Elf32_Shdr *SectionHeaderRelSeul2 = malloc(80 * sizeof(Elf32_Shdr));
	int nbSectionTypeRelSeul2 = 0;

	Elf32_Shdr * tmp = malloc(sizeof(Elf32_Shdr));
	printf("Fusion sections : \n");
	for (i = 0; i < fichierElf1->header_elf.e_shnum; i++) { //Pour toutes les sections du fichier1
		RechercheSectionByName(fichierElf2, getSectionName(fichierElf1->sectionsTable[i],fichierElf1), tmp, &present); //est-ce-que cette section est dans le fichier2 ?
		if (present) { //Si oui on les fusionne
			//On vérifie que les sections de même nom sont du même type
			if (fichierElf1->sectionsTable[i].sh_type != tmp->sh_type) {
				printf("\t\t======== ERREUR FATAL=======================\n");
				printf("\t\t%s",getSectionName(fichierElf1->sectionsTable[i],fichierElf1));
				printf("\t\tpas le meme type : %d et %d\n",fichierElf1->sectionsTable[i].sh_type,tmp->sh_type);
				exit(1);
			//Et possedent les memes flags 
			} else if (fichierElf1->sectionsTable[i].sh_flags != tmp->sh_flags) {
				printf("\t\t======== ERREUR FATAL=======================\n");
				printf("\t\t%s",getSectionName(fichierElf1->sectionsTable[i],fichierElf1));
				printf("\t\tpas les memes flags : %d et %d\n",fichierElf1->sectionsTable[i].sh_flags,tmp->sh_flags);
				exit(1);
			//Pour continuer
			}else {
				//On fusionne et on sauvegarde le nouveau header
				sections_elfRes[i] = sectionfusion(fichierElf1->sectionsTable[i], *tmp, fichierElf1, fichierElf2, fichierElfRes, shstrtab, strtab); //Fonction chargee de la fusion
				sections_elfRes[i].indexHeader = i;
				fichierElfRes->sectionsTable[i] = sections_elfRes[i].header;

				//On garde les index de shstrtab ainsi que de strtab car leur écriture doit etre realise apres avoir traiter toutes les sections
				if (strcmp(getSectionName(fichierElf1->sectionsTable[i],fichierElf1),".shstrtab") == 0) { 
				indexShstrtab = nbSectionEcrites;
				} else if (strcmp(getSectionName(fichierElf1->sectionsTable[i],fichierElf1),".strtab") == 0) {
				indexStrtab = nbSectionEcrites;
				} else if (fichierElf1->sectionsTable[i].sh_type == SHT_REL ) {
					SectionHeaderRel[nbSectionTypeRel][0] = fichierElf1->sectionsTable[i];
					SectionHeaderRel[nbSectionTypeRel][1] = *tmp;
					indexesREL[nbSectionTypeRel] = i;
					nbSectionTypeRel++;
				} else 
				//On garde l'index de la table des symbole pour les realocation ainsi que pour lui attribuer correctement son sh_link
				if (strcmp(getSectionName(fichierElf1->sectionsTable[i],fichierElf1),".symtab") == 0) { //On sauvegarde l'index de la table des symbole pour la fusion des tables de realocation
				indexSymtab = nbSectionEcrites;
				}

			}
		} else { //Sinon, on ajoute simplement
 			if (fichierElf1->sectionsTable[i].sh_type == SHT_REL ) {
					SectionHeaderRelSeul1[nbSectionTypeRelSeul1] = fichierElf1->sectionsTable[i];
					indexesREL1[nbSectionTypeRelSeul1] = i;
					nbSectionTypeRelSeul1++;
			} else {
				sections_elfRes[nbSectionEcrites] = SectionAjout(fichierElf1->sectionsTable[i], fichierElf1, shstrtab); //Ajout
				sections_elfRes[nbSectionEcrites].indexHeader = nbSectionEcrites;
				fichierElfRes->sectionsTable[nbSectionEcrites] = sections_elfRes[nbSectionEcrites].header;
			}
		}
		nbSectionEcrites++;
	}
	//Meme chose ici
	for (i = 0; i < fichierElf2->header_elf.e_shnum; i++) { //Pour toutes les sections du fichier2
		RechercheSectionByName(fichierElf1, getSectionName(fichierElf2->sectionsTable[i],fichierElf2), tmp, &present); //est-ce-que cette section est dans le fichier1 ?
		if (!present) {  //Si oui, on ajoute simplement
			 if (fichierElf1->sectionsTable[i].sh_type == SHT_REL ) {
				SectionHeaderRelSeul2[nbSectionTypeRelSeul2] = fichierElf2->sectionsTable[i];
				indexesREL2[nbSectionTypeRelSeul2] = i;
				nbSectionTypeRelSeul2++;
			} else {
			sections_elfRes[nbSectionEcrites] = SectionAjout(fichierElf2->sectionsTable[i], fichierElf2, shstrtab); //Ajout
			sections_elfRes[nbSectionEcrites].indexHeader = nbSectionEcrites;
			fichierElfRes->sectionsTable[i] = sections_elfRes[nbSectionEcrites].header;
			}
			nbSectionEcrites++;
		}
	}
	printf("OK\n");

	///////////////////////////////////////////////////
	//A FAIRE
	/////////////
	//Pour fusionner les sections de realocation il faut attendre d'avoir les sections de symbole finaux
	//////////////////////////////////////////////////////

	//Placer sh_link pour la table des symboles
	sections_elfRes[indexSymtab].header.sh_link = indexStrtab;
	//Et trouver les nouveaux st_ndx pour chaque symboles
	int nbsymboles = fichierElfRes->sectionsTable[indexSymtab].sh_size/fichierElfRes->sectionsTable[indexSymtab].sh_entsize;
	for (i = 0; i < nbsymboles; i++) {
		if (fichierElfRes->tabSymbole[i].fichier == 1) {
			fichierElfRes->tabSymbole[i].symbole.st_shndx = getSt_shndx(fichierElfRes->tabSymbole[i], fichierElf1, nbsymboles, shstrtab);
		} else if (fichierElfRes->tabSymbole[i].fichier == 2) {
			fichierElfRes->tabSymbole[i].symbole.st_shndx = getSt_shndx(fichierElfRes->tabSymbole[i], fichierElf2, nbsymboles, shstrtab);
		} else {
			printf("============\nLe symbole d'index %d, n'a pas de fichier de provenance\n============\n",i);
		}
	}

	//finalement on s'occupe de shstrtab et de strtab car il a fallu parcourir toutes les sections pour avoir leur contenu
	sectionShstrtab(&sections_elfRes[indexShstrtab],shstrtab);
	sectionStrtab(&sections_elfRes[indexStrtab],strtab);

	// Pas sur de ou on doit le placer alors je mets tout ici ;)
	int relsize1 = 0;
	relsize1 = relsize(fichierElf1);
	int relsize2 = 0;
	relsize2 = relsize(fichierElf2);
	Reloctable* reloctable1 = malloc(sizeof(Reloctable)*relsize1);
	Reloctable* reloctable2 = malloc(sizeof(Reloctable)*relsize2);
	fichierElf1->tabRel = reloctable1;
	fichierElf2->tabRel = reloctable2;
	reloctable1 = crea_rel_table(fichierElf1);
	reloctable2 = crea_rel_table(fichierElf2);
	printf("CEST LA ZOUBIDA : %x \n",reloctable1[0].tablerel[0].r_offset);
	//fichierElf1->tabRel = reloctable1;
	//fichierElf2->tabRel = reloctable2;
	init_new_rel(fichierElfRes,fichierElf1,fichierElf2);
	printf("WALLAYE BILLAYE \n");
	//Fusion des Sections de type Rel :
	for (i=0; i < nbSectionTypeRel ; i++) {
		sections_elfRes[indexesREL[i]]=RelFusion(fichierElf1, SectionHeaderRel[i][0], SectionHeaderRel[i][1], fichierElf2,fichierElfRes, nbsymboles, indexesREL[i]);	
		printf("WALLAYE BILLAYE 2\n");
	}
	// Mise à jour des Sections de type Rel pour chaque fichier:
	for (i = 0; i < nbSectionTypeRelSeul1++; i++) {
		sections_elfRes[indexesREL1[i]]=RelUpdate (fichierElf1,fichierElfRes, SectionHeaderRelSeul1[i],nbsymboles);
	}
		for (i = 0; i < nbSectionTypeRelSeul2++; i++) {
		sections_elfRes[indexesREL2[i]]=RelUpdate (fichierElf2,fichierElfRes, SectionHeaderRelSeul2[i],nbsymboles);
	}

	// CA RENVOIT DES SECTIONS ET ON EN FAIT RIEN :3 JE MODIFIE JUSTE LES RELOCTABLE.. 

	/////////////////////////////////////////////////////////////////////


	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Création du header pour le nouveau fichier
	////////////////////////////////////////////
	printf("Création du header :\n");
	fichierElfRes->header_elf = header(fichierElf1, fichierElf2);
	//On complete avec :
	//e_shnum
	fichierElfRes->header_elf.e_shnum = nbSectionEcrites;
	//e_shstrndx
	fichierElfRes->header_elf.e_shstrndx = indexShstrtab;
	printf("\te_shstrndx %d \n",indexShstrtab);
	//On ne saurra e_shoff qu'à l'ecriture

	printf("OK\n");
	//////////////////////////////////////////////////////////////////////


	printf("\n");
	//On ecrit la fusion des deux fichiers
	printf("Ecriture du fichier fusionnee : \n");
	ecritureFichierFusionnee(fichierElfRes, sections_elfRes, shstrtab);


	free(tmp);
	free(strtab);
	free(shstrtab);
	free(sections_elfRes);
	free(indexesREL);
	free(indexesREL1);
	free(indexesREL2);
}
