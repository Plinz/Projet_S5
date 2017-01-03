#include "projet.h"


int nb_tab_rel(Elf32_Ehdr header_elf, Elf32_Shdr *section_elf)
{
	int i,k=0;
	for(i=0; i<(header_elf.e_shnum); i++) // parcours les sections
	{

		if(section_elf[i].sh_type == SHT_REL) // si section du type SHT_REL
		{
			k++;
		}
	}
	return k;
}

TableReimplantation* recupTabReimplantation(FILE* file, Elf32_Ehdr header_elf, Elf32_Shdr *section_elf) 
{
	int i, nb,j=0,nb_tab=nb_tab_rel(header_elf,section_elf),k=0;
	fseek(file, section_elf->sh_offset, SEEK_SET);
	TableReimplantation* pointeur_reimplantation;

	 // preparation du tableau pour le remplissage 				
	pointeur_reimplantation = malloc(sizeof(TableReimplantation)*nb_tab);
	for(i=0; i<(header_elf.e_shnum); i++) // parcours les sections
	{
		if(section_elf[i].sh_type == SHT_REL) // si section du type SHT_REL
		{
			nb=((section_elf[i].sh_size) / (section_elf[i].sh_entsize));
			fseek(file, section_elf[i].sh_offset, 0);

			pointeur_reimplantation[k].section_rel = section_elf[i];
			pointeur_reimplantation[k].reimplantation_elf = malloc(sizeof(Elf32_Rel)*nb);
			pointeur_reimplantation[k].nb_entrees=nb;


			for(j=0; j<nb; j++) // a chaque fois qu'on trouve une entree
			{

				//TableReimplantation[i].reimplantation_elf = section_elf[j];

				if(header_elf.e_ident[5] == ELFDATA2LSB)	// si little endiant
				{
					fread(&pointeur_reimplantation[j].reimplantation_elf[i].r_offset,sizeof(int), 1, file); // lecture offset de la section
					fread(&pointeur_reimplantation[j].reimplantation_elf[i].r_info,sizeof(int), 1, file);	  // lecture info de la section
				}

				else if(header_elf.e_ident[5] == ELFDATA2MSB)	// si big endiant
				{
					fread(&pointeur_reimplantation[k].reimplantation_elf[j].r_offset, sizeof(int), 1, file);  // lecture offset de la section
					pointeur_reimplantation[k].reimplantation_elf[j].r_offset = __bswap_32(pointeur_reimplantation[k].reimplantation_elf[j].r_offset);
					fread(&pointeur_reimplantation[k].reimplantation_elf[j].r_info, sizeof(int), 1, file);      // lecture info de la section
					pointeur_reimplantation[k].reimplantation_elf[j].r_info = __bswap_32(pointeur_reimplantation[k].reimplantation_elf[j].r_info);	
				}			

				else
				{
					printf("Section au format de donnée inconnu");
					exit(0);
				}
				//pointeur_reimplantation->reimplantation_elf = reimplantation_elf;	// remplissage du tableau 
		
			}
			k++;

		}
	}

	return pointeur_reimplantation;
}


void affichageTabReimplantation(Elf32_Sym *SymbolTable, FILE* elf, Elf32_Ehdr header_elf, Elf32_Shdr *SectionTable, TableReimplantation* pointeur_reimplantation, int longueurTab)
{
	int i,j;
	unsigned char sym, type;

	for(i=0; i<longueurTab; i++){

		printf("Relocation section ");
		afficherNomSection(header_elf, SectionTable, elf, pointeur_reimplantation[i].section_rel);


		printf(" at offset 0x%x contains %d entries:\n", pointeur_reimplantation[i].section_rel.sh_offset, pointeur_reimplantation[i].nb_entrees);
		printf("Offset	Info	Type		Sym.Value	Sym. Name \n");

		for(j=0; j<pointeur_reimplantation[i].nb_entrees; j++){

			sym = ((pointeur_reimplantation[i].reimplantation_elf[j].r_info) >> 8);
			type = (unsigned char) (pointeur_reimplantation[i].reimplantation_elf[j].r_info);

			printf("%x	%x	", pointeur_reimplantation[i].reimplantation_elf[j].r_offset, pointeur_reimplantation[i].reimplantation_elf[j].r_info);
			afficherTypeReimplantation(type);
			printf("	");
	
			printf("%d", SymbolTable[sym].st_value);
			printf("		");
			afficherNomSection(header_elf, SectionTable, elf, SectionTable[sym]);
			printf("\n");

		}
	printf("\n");
	}

}

void afficherTypeReimplantation(unsigned char typeReimplantation){
	switch(typeReimplantation){
		case 2:
			printf("R_ARM_ABS32");
			break;
		case 5:
			printf("R_ARM_ABS16");
			break;
		case 6:
			printf("R_ARM_ABS32");
			break;
		case 8:
			printf("R_ARM_ABS8");
			break;
		case 28:
			printf("R_ARM_CALL");
			break;
		case 29:
			printf("R_ARM_JUMP24");
			break;
		default:
			printf("Autre type");
			break;
	}
}





