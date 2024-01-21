
# ESN11 - Lab3

## Introduction
L'objectif de ce projet était d'implémenter un système d'affichage de données issue du gyroscope de la carte DE-10. L'affichage des données s'actualise toute les secondes sur les 7 segments des digits grâce à un timer. Pour choisir l'axe à afficher, il suffit de cliquer sur un bouton. 
Ce système est implémenter avec un soft-core NIOS II et l'interface avec le gyroscope est géré par un bloc IP I2C.

## Création d'un système NIOS II basique

### Schéma Fonctionnel
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/6ceaf3af-c7d9-4a1d-b892-a8742ecde1a7)



### Platform Designer
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/2144c48b-bbe2-4273-a274-214cbd9eaad0)



- Après la création d'un nouveau projet sur Quartus, ouvrir le Platform Designer pour ajouter tous les éléments nécessaires au projet :
- **Mémoire** : RAM avec un stockage de 40 Mo.
Nous avons besoin d'un compteur à 4 chiffre, alors :
- **PIO_0** : Correspond à la sortie Qsys du 1er compteur 4 bits.
- **PIO_1** : Correspond à la sortie Qsys du 2eme compteur 4 bits.
- **PIO_2** : Correspond à la sortie Qsys du 3eme compteur 4 bits.
- **PIO_3** : Correspond à la sortie Qsys du 4eme compteur 4 bits.
- **PIO_4** : Correspond à la sortie Qsys du 5eme compteur 4 bits, permettant de signé un chiffre négatif.
- **PIO_BOUTON** : Correspond à l'entré du bouton KEY1, permettant de changer l'affichage.
- **Timer_0** : On paramètre sa période à 1 seconde.
- **Bloc IP I2C** : Communication avec le gyroscope

### Quartus
Le système se décompose en 3 partis :
   - lab3.qsys
   - bin_to_7seg.vhd (fichier vhdl qui prend une entrée 4 bits (compteurs) et sort en sortie la correspondance (7 bits) pour allumer les 7 segments du compteurs. Attention le 7 segments est sensible au "0" c'est à dire qu'il s'allume quand le niveau logique est "0".) J'ai repris le fichier du lab2 et j'ai ajouter une ligne permettant d'afficher le segment 6 (celui du milieu) lorsque l'entrée vaut "1111". De cette façon, on peut signer un nombre car cela ajoute un "-" devant.
   - lab3_top.vhd (fichier top level, il définit l'entrée et la sortie du top_level avec 5 sortie sur 7 bits (correspondand au 7 segments des 5 compteurs)). De plus, on créé 5 signaux permettant de lier les composants lab3 et bin_to_7seg. Au final, notre système est définit par 6 composants. 

## Flux Logiciel du NIOS II
- Ouvrir un terminal NIOS II et générer le BSP en lien avec les spécifications de la carte DE-10.
- Générer le Makefile du projet.

## Langage C (main.c)
Le code C se divise en deux parties :
1. `irqhandler_timer()` : Routine d’interruption du timer, le flag d’interruption est remis à 0 à chaque fois qu'une routine est lancé. Cette routine génère en fonction d'un compteur 16 bits (i) une conversion binaire vers BCD. Cette conversion sépare en 3 variable de 8 bits le compteurs i. Sous forme d'unité, dizaine et centaine.
Cette routine est lancé à chaque interruption soit toutes les secondes.

2. `main()` : Programme principal conçu pour enregistrer l'interruptions du timer. De plus, il initialise le compteur à 0. Ensuite, le programme boucle à l'infinie.

## Résultat
Verification du fonctionnement du compteur sans timer (on vérifie ici si le compteur va jusqu'à 999 et revient à 0)

https://github.com/ESN2024/roche_lab2/assets/116710033/93f47ef4-0973-429a-8db5-6de4d433b587

Validation de fonctionnement du système

https://github.com/ESN2024/roche_lab2/assets/116710033/e39876a7-0a60-43ff-96be-42ec67c33ff5

Le système fonctionne correctement.


## Conclusion

Ce projet m'a permis d'apprendre à contrôler les 7 segments de la carte DE-10 Lite. J'ai mis du temps à comprendre l'architecture du système, on vient contrôler les PIOs sur 4 bits, ensuite un fichier .vhdl convertie ces 4 bits en sortie 7 bits pour contrôler les 7 segments. Je n'avais pas refait de VHDL depuis presque 1 an. Le fait de reprendre la création de composants et de les lier entre eux m'a permis de me replonger dans le VHDL. J'ai fait quelques erreurs d'étourderie, par exemple au début mon compteur en C était défini en 8 bits donc mon compteur compté jusqu'à 255. Ensuite, j'ai eu quelques soucis à implémenter la conversion BCD j'ai du ré-étudier les divisions euclidiennes et l'utilisation de l'opérateur modulo en C. 

Ce projet était très intéressant car il permet de revoir les concepts de base du VHDL.
