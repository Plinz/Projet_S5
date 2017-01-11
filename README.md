# PROG5 - Projet logiciel (groupe 6)

**Éditeur de liens - Phase de fusion**

L'objectif  de  ce  projet  est  d’implémenter  une  sous  partie  d'un  éditeur  de  liens. Plus  précisément, le projet est centré sur la première phase, dite de fusion, exécutée par l’éditeur de liens. Le projet est structuré en étapes, avec la programmation de plusieurs outils intermédiaires, permettant de mieux comprendre les principales notions et de simplifier le découpage des tâches.

***

Sales Dispo : 202/204/216


***
git clone https://github.com/Plinz/Projet_S5.git // git pull

git add nom_fich

git commit -m message

git push

## Membres du groupe

* Andre Valentin
* Defosse Benjamin
* Duquennoy Antoine
* Novel Mathias
* Marco Florian

## Retour Audit

* A partir des noms de sections, construire une structure pour recuperer leur contenus 
* Fonction pour faire les tables section dans tabSection
* Faire une table par section de Relocation -- FLO
* Si on a pas le meme type et pas le meme nom, raise une erreur
* Resultats de type fichierelf et pas File dans la fusion

## Documentation Annexe 

* Cours de Verimag en français : http://www-verimag.imag.fr/~moy/cours/ldb-apprentissage/projetC.pdf
* Les ELF Specif .C en Français : http://manpagesfr.free.fr/man/man5/elf.5.html
* Les Headers ELF Expliqués en Français : https://fr.wikipedia.org/wiki/Executable_and_Linkable_Format
* Logiciel ELF Viewer : https://sourceforge.net/p/elfviewer/code/ci/master/tree/
* Introduction to linkers : http://www.lurklurk.org/linkers/linkers.html
* "ld" et "readelf" disponible dans la librairie binutils : http://www.sourceware.org/binutils/


* ELF Parser : https://github.com/TheCodeArtist/elf-parser
* * https://github.com/TheCodeArtist/elf-parser/blob/master/elf-parser.c


* Doc qui va nous servir selon les redoublants : http://wiki.osdev.org/ELF_Tutorial
* http://wiki.osdev.org/ELF
* Document pour les sections: https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-94076/index.html
* Affichage Table des symboles ( Incomplet ) : http://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
* Fichier contenant l'ensemble des types, structures et macros ELF : http://osxr.org:8080/glibc/source/elf/elf.h?v=glibc-2.18

* Un Editeur de lien pour Android (donc ARM) en C++ : https://android.googlesource.com/platform/bionic/+/master/linker/linker.cpp

* RODATA : http://webcache.googleusercontent.com/search?q=cache:http://blog.isis.poly.edu/exploitation%2520mitigation%2520techniques/exploitation%2520techniques/2011/06/02/relro-relocation-read-only/&gws_rd=cr&ei=lKFzWLbUHcynaIervegM 
https://www.root-me.org/fr/Documentation/Applicatif/Memoire-protection-RELRO
http://tk-blog.blogspot.fr/2009/02/relro-not-so-well-known-memory.html

Autres parsers :
* ELF Parser 2 : https://github.com/sifteo/node-elf
* ELF Parser 3 : https://github.com/jacob-baines/elfparser

## PROTIP

"Dans la partie Annexe au lieu de "arm-eabi-cquevousvoulez" faut mettre "arm-none-eabi-cquevousvoulez" pour l'instant en tout cas celui qui est sur l'énoncé marche pas je sais pas pourquoi."



< gl >	en gros quand tu link tes deux .o, tu dois faire en sorte que d'une fonction a l'autre (car quand tu compiles un premier .o et un second .o t'as pas toutes les infos), quand elles sont appeles dans deux unites de translations differentes, les donnees dans les registres ont un sens

< gl >	ie. faire des sauvegardes, nettoyage, etc

< gl >	souvent c'est fait via des algorithmes de coloration de graphes, mais il y a pas mal de methodes

https://en.wikipedia.org/wiki/Register_allocation

Gestion des erreurs : http://paste.suut.in/RuhovKXy.h

## NOTE SUR LA FUSION :

* Etape 7 : Fusion/Renumérotation des symboles :
  -> Tous les symboles locaux des deux concanténés
  
  -> Symboles globaux fusionnés de la forme : 
    
    Concatene les symboles globaux unique à chacun 
    
    Même Nom : Definis // Non Definis :
     
    Les Deux définis => Echec
    
    Un defini, Un non défini => On prend le défini
    
    Deux symboles non défini => On en mets qu'un des deux. 
    
* Etape 6 (ou plus loin ) :

On voit dans le sujet qu'on doit juste s'occuper de la section progbits, matthias t'es allé trop loin. Possible que les opérations sur les headers qu'on veut faire, faut faire ça à la fin

SH_TYPE ----- SH_LINK  -----       SH_INFO

SHT_Dynamics   -----   OSEF -----??????

SHT_HASH    -----   header du symtab ou le hash s'affiche ---- 0

SHT_REL/RELA    -----  Index de l'header de tab_symb associé ------ index de l'header de là ou il y a la relocation

SHT_SYMTAB  -----      Index de l'header du strtab associé ------- 1 de plus que le dernier symbole local

AUTRE :      -----     SHN_UNDEF  ------ 0
