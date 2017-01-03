#include "projet.h"

void affichageContenuSection(Elf32_Shdr* tabEnTeteSection, int numberSection, FILE* elf){
	int i;
	int j;
	int x;
	char car;
	int flag;
	int offsetcourant, sizecourant;

	flag = 0;
	while(!flag){
		printf("Numero de section?");
		scanf("%d", &x);
		for(i=1; i<numberSection; i++){
			if(x==i)
				flag = 1 ;
		}
	}
	

	offsetcourant=tabEnTeteSection[x].sh_offset;
	sizecourant=tabEnTeteSection[x].sh_size;

	fseek(elf,offsetcourant,SEEK_SET);
	j=1;
	car = fgetc(elf);
	printf("%x", car);
	while(j<=sizecourant){
		car = fgetc(elf);
		printf("%x", car);
		j++;
	}
	printf("\n");
	
}

