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
* Marco Florian
* Novel Mathias

## Work In Progress

* Creation du lecteur de fichiers
* Parsing des fichiers  

## Documentation Annexe 

* Cours de Verimag en français : http://www-verimag.imag.fr/~moy/cours/ldb-apprentissage/Edition-de-liens-nup.pdf
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


Autres parsers :
* ELF Parser 2 : https://github.com/sifteo/node-elf
* ELF Parser 3 : https://github.com/jacob-baines/elfparser

## PROTIP

"Dans la partie Annexe au lieu de "arm-eabi-cquevousvoulez" faut mettre "arm-none-eabi-cquevousvoulez" pour l'instant en tout cas celui qui est sur l'énoncé marche pas je sais pas pourquoi."
