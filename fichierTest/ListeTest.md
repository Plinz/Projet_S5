Etape à faire pour créer des fichiers tests .s et .o
export PATH=/opt/gnu/arm/bin:$PATH
arm-eabi-gcc -mno-thumb-interwork -S [FILE .c]
arm-eabi-as -o [FILE .o] [FILE .s]


Liste des Tests pour chaque etapes :

# Etape 1 : Affichage de l'en-tête

Quel type d'erreur peut-il y avoir ? 

Testé en comparaison avec readelf -h

# Etape 2 : Affichage de la table des sections

Quel type d'erreur peut il y avoir ?

Testé en comparaison avec readelf -S

# Etape 3 : Affichage du contenu d'une section

Quel type d'erreur peut-il y avoir ?

Testé en comparaison avec readelf -x

# Etape 4 : Affichage de la table des symboles

Quel type d'erreur peut il y avoir ?

Testé en comparaison avec readelf -s

# Etape 5 : Affichage des tables de réimplémentation

Quel type d'erreur peut il y avoir ?

Testé en comparaison avec readelf -r

# Etape 6 : 

# Etape 7 : 

# Etape 8 : 

# Etape 9 : 
