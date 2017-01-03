#include "projet.h"

void renumerotationSection(Elf32_Shdr* tabAncienneSections, Elf32_Shdr* tabNouvelleSections, Elf32_Ehdr enTete){
	int i;
	int j =0;
	
	for(i=0; i<enTete.e_shnum; i++){
		if((tabAncienneSections[i].sh_type != SHT_REL ) || (tabAncienneSections[i].sh_size != 0)){
			tabNouvelleSections[i] = tabNouvelleSections[j];
			j++;
		}
	}
}
