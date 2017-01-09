#include "elf_reader.h"

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

Section sectionShstrtab(Section section, Shstrtab * shstrtab) {
  section.nbOctets = shstrtab->offsetCourant;
  section.contenu = malloc(section.nbOctets);
  section.header.sh_size = section.nbOctets;

  int j;
  int curseur = 0;
  for (int i = 0; i < shstrtab->nbNames; i++) {
    j = 0;
    while ((section.contenu[curseur] = shstrtab->names[i][j]) != '\0') {
			j++;
      curseur++;
		}
    curseur++;
  }


  return section;
}

Section sectionStrtab(Section section, Strtab * strtab) {
  section.nbOctets = strtab->offsetCourant;
  section.contenu = malloc(section.nbOctets);
  section.header.sh_size = section.nbOctets;

  int j;
  int curseur = 0;
  for (int i = 0; i < strtab->nbNames; i++) {
    j = 0;
    while ((section.contenu[curseur] = strtab->names[i][j]) != '\0') {
			j++;
      curseur++;
		}
    curseur++;
  }


  return section;
}

Section sectionfusion(Elf32_Shdr sectionHeader1, Elf32_Shdr sectionHeader2, FichierElf * fichierElf1, FichierElf * fichierElf2, Shstrtab * shstrtab, Strtab * strtab) {
 Section sectionfusionee;

 shstrtab->names[shstrtab->nbNames] = getSectionName(sectionHeader1, fichierElf1);

 printf("\t\tFusion %s\n",shstrtab->names[shstrtab->nbNames]);

 //On vérifie que les sections de même nom sont du même type
 if (sectionHeader1.sh_type != sectionHeader1.sh_type) {
   printf("\t\t\tpas le meme type : %d et %d\n",sectionHeader1.sh_type,sectionHeader2.sh_type);
   //QU ES CE QU ON FAIT ??????????????
 } else {
     switch(sectionHeader1.sh_type) {
		case SHT_SYMTAB :
			printf("Table des symboles à faire");
       default : //Pour l'instant, on fait partout pareil
         sectionfusionee.nbOctets = sectionHeader1.sh_size + sectionHeader2.sh_size;
         sectionfusionee.contenu = malloc(sectionfusionee.nbOctets);

        //Concatenation
        int i;
        fseek(fichierElf1->fichierElf, sectionHeader1.sh_offset, SEEK_SET);
        for (i = 0; i < (int)sectionHeader1.sh_size; i++) {
        sectionfusionee.contenu[i] = fgetc(fichierElf1->fichierElf);
        }
        int j;
        fseek(fichierElf2->fichierElf, sectionHeader2.sh_offset, SEEK_SET);
        for (j = 0; j < (int)sectionHeader1.sh_size; j++) {
        sectionfusionee.contenu[i+j] = fgetc(fichierElf2->fichierElf);
        }

        //Creation Header
        sectionfusionee.header.sh_type = sectionHeader1.sh_type;
        sectionfusionee.header.sh_name = shstrtab->offsetCourant;
        sectionfusionee.header.sh_size = sectionfusionee.nbOctets;
        sectionfusionee.header.sh_flags = sectionHeader1.sh_flags | sectionHeader2.sh_flags;
        sectionfusionee.header.sh_addr = 0;
        sectionfusionee.header.sh_link = SHN_UNDEF;
        sectionfusionee.header.sh_info = 0;
        if (sectionHeader1.sh_addralign != sectionHeader2.sh_addralign) {
          printf("\t\t\tpas le meme sh_addralign : %d et %d\n",sectionHeader1.sh_addralign,sectionHeader2.sh_addralign);
        } else {
           sectionfusionee.header.sh_addralign = sectionHeader1.sh_addralign;
        }
        if (sectionHeader1.sh_entsize != sectionHeader2.sh_entsize) {
          printf("\t\t\tpas le meme sh_entsize : %d et %d\n",sectionHeader1.sh_entsize,sectionHeader2.sh_entsize);
        } else {
          sectionfusionee.header.sh_entsize = sectionHeader1.sh_entsize;
        }
        //sh_offset sera etabli à l'écriture du fichier
       break;
     }
 }
  shstrtab->offsetCourant += strlen(shstrtab->names[shstrtab->nbNames]) + 1;
  shstrtab->nbNames++;

 return sectionfusionee;
}

Section SectionAjout(Elf32_Shdr sectionHeader, FichierElf * fichierElf, Shstrtab * shstrtab) {
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

  shstrtab->offsetCourant += strlen(shstrtab->names[shstrtab->nbNames]) + 1;
  shstrtab->nbNames++;

  return section;
}

void RechercheSectionByName(FichierElf * fichierElf, char * secName, Elf32_Shdr * res, int * present){
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
    *present = 1;
		*res = currentSection;
	} else {
		*present = 0;
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
	Section * sections_elfRes;

	//Autre
	int i,j;
	char * buffer = malloc(1);
  int nbSectionMax = fichierElf1->header_elf.e_shnum + fichierElf2->header_elf.e_shnum;
  int nbSectionEcrites = 0;

  //Tableau qui contient les Sections{contenu,header} qu'on ecrira dans le fichier resultat
  sections_elfRes = malloc(nbSectionMax*sizeof(Section)); //Il ne peut pas y avoir plus de sections dans le fichier final que dans les deux fichier à fusioner confondu

  //Structure qui va contenir le nom de toutes les sections
  Shstrtab *shstrtab = malloc(sizeof(Shstrtab));
  shstrtab->names = malloc((nbSectionMax)*sizeof(char*));
  for (i = 0; i < (nbSectionMax); i++) {
    shstrtab->names[i] = malloc(50);
  }
  shstrtab->offsetCourant = 0;
  shstrtab->nbNames = 0;


  //Structure qui va contenir le nom de tous les symboles
  Strtab *strtab = malloc(sizeof(Strtab));
  strtab->names = malloc((nbSectionMax)*sizeof(char*)); //A CHANGER PAR NBSYMBOLEMAX
  for (i = 0; i < (nbSectionMax); i++) {
    strtab->names[i] = malloc(50);
  }
  strtab->offsetCourant = 0;
  strtab->nbNames = 0;


	//Différenciation en les sections à fusionner ou à ajouter
	//Si une section est présente dans les deux fichiers on la fusionne
  //Sinon on l'ajoute
  //Obligé de faire une seconde boucle pour voi les sections du fichier2 qui ne sont pas dans le fichier1
  int present = 0;
  int indexShstrtab = -1;
  int indexStrtab = -1;
	Elf32_Shdr * tmp = malloc(sizeof(Elf32_Shdr));
	for (i = 0; i < fichierElf1->header_elf.e_shnum; i++) { //Pour toutes les sections du fichier1
		RechercheSectionByName(fichierElf2, getSectionName(fichierElf1->sectionsTable[i],fichierElf1), tmp, &present); //est-ce-que cette section est dans le fichier2 ?
		if (present) { //Si oui on les fusionne
      		sections_elfRes[i] = sectionfusion(fichierElf1->sectionsTable[i], *tmp, fichierElf1, fichierElf2, shstrtab, strtab); //Fusion
			if (strcmp(getSectionName(fichierElf1->sectionsTable[i],fichierElf1),".shstrtab") == 0) { //On sauvegarde l'index de shstrtab pour le header et pour ecrire cette section après les boucles
			indexShstrtab = nbSectionEcrites;
			}
			if (strcmp(getSectionName(fichierElf1->sectionsTable[i],fichierElf1),".strtab") == 0) { //On sauvegarde l'index de shstrtab pour le header et pour ecrire cette section après les boucles
			indexStrtab = nbSectionEcrites;
			}
		} else { //Sinon, on ajoute simplement
      sections_elfRes[nbSectionEcrites] = SectionAjout(fichierElf1->sectionsTable[i], fichierElf1, shstrtab); //Ajout

		}
    nbSectionEcrites++;
	}
  //Meme chose ici
	for (i = 0; i < fichierElf2->header_elf.e_shnum; i++) { //Pour toutes les sections du fichier2
		RechercheSectionByName(fichierElf1, getSectionName(fichierElf2->sectionsTable[i],fichierElf2), tmp, &present); //est-ce-que cette section est dans le fichier1 ?
		if (!present) {  //Si oui, on ajoute simplement
      sections_elfRes[nbSectionEcrites] = SectionAjout(fichierElf2->sectionsTable[i], fichierElf2, shstrtab); //Ajout
			nbSectionEcrites++;
		}
	}

  //finalement on s'occupe de shstrtab et de strtab car il a fallu parcourir toutes les sections pour avoir leur contenu
  sections_elfRes[indexShstrtab] = sectionShstrtab(sections_elfRes[indexShstrtab],shstrtab);
  sections_elfRes[indexStrtab] = sectionStrtab(sections_elfRes[indexStrtab],strtab);



	printf("OK\n");
	/////////////////////////////////////////////////////////////////////

	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture des sections
	////////////////////////
	printf("Ecriture des Sections : \n");

  //Ecriture du contenu des sections
  //On place le curseur sur le 53e octets, on écrira le header plus tard
  fseek(elfRes, 52, SEEK_SET);
  for (i = 0; i < nbSectionEcrites; i++) {
    sections_elfRes[i].header.sh_offset = ftell(elfRes);
    for (j = 0; j < sections_elfRes[i].nbOctets; j++) {
      fputc(sections_elfRes[i].contenu[j],elfRes);
    }
  }

	printf("OK\n");
	/////////////////////////////////////////////////////////////////////


	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Ecriture de la table des sections
	////////////////////////////////////
	printf("Ecriture de la table des Sections :\n");
	int shoff = ftell(elfRes); //On sauvegarde l'offset du début de la table des sections pour le header
	for (i = 0; i < (nbSectionEcrites); i++) {
		buffer = (char *) &sections_elfRes[i].header;
		for (j = 0; j < 40; j++) {
			fputc(*buffer,elfRes);
			buffer++;
		}

	}

	printf("OK\n");
	/////////////////////////////////////////////////////////////////////


	printf("\n");
	/////////////////////////////////////////////////////////////////////
	//Création du header pour le nouveau fichier
	////////////////////////////////////////////
	printf("Création du header :\n");
  header_elfRes = header(fichierElf1, fichierElf2);
  //On complete avec :
  //e_shoff
  header_elfRes.e_shoff = shoff;
  //e_shnum
  header_elfRes.e_shnum = nbSectionEcrites;
  //e_shstrndx
  header_elfRes.e_shstrndx = indexShstrtab;

	printf("OK\n");
  //////////////////////////////////////////////////////////////////////


	printf("\n");
  /////////////////////////////////////////////////////////////////////
	//Ecriture du header pour le nouveau fichier
	////////////////////////////////////////////
	printf("Ecriture du header :\n");
	//On se replace au début du fichier
	fseek(elfRes, 0, SEEK_SET);
	buffer = (char *) &header_elfRes;
	for (i = 0; i < 52; i++) {
		fputc(*buffer,elfRes);
		buffer++;
	}

	printf("OK\n");
	/////////////////////////////////////////////////////////////////////

}
