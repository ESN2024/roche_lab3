
# ESN12 - Lab3

## Introduction
L'objectif de ce projet était d'implémenter un système d'affichage de données issue du gyroscope de la carte DE-10. L'affichage des données s'actualise toute les secondes sur les 7 segments des digits grâce à un timer. Pour choisir l'axe à afficher, il suffit de cliquer sur un bouton. 
Ce système est implémenter avec un soft-core NIOS II et l'interface avec le gyroscope est géré par un bloc IP I2C.

## Création d'un système NIOS II basique

### Schéma Fonctionnel
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/6ceaf3af-c7d9-4a1d-b892-a8742ecde1a7)



### Platform Designer
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/2144c48b-bbe2-4273-a274-214cbd9eaad0)



- Après la création d'un nouveau projet sur Quartus, ouvrir le Platform Designer pour ajouter tous les éléments nécessaires au projet :

- **Eléments basique du système :**
- **Processeur Nios II**
- **Clock**
- **JTAG** : Permet de debugger
- **Mémoire** : RAM avec un stockage de 40 Mo.

- **Eléments propre au sujet :**
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
Le code C se divise en plusieurs parties :
1. `irqhandler_bouton_key1()` : Routine d’interruption du bouton key1, le flag d’interruption est remis à 0 à chaque fois qu'une routine est lancé. Cette routine incrémente la variable bouton lorsque qu'on appuie sur le bouton. Elle est sensible uniquement au front descendant, cela permet d'incrémenter de 1 pour un appuie et relachement du bouton (front montant et descendant). Lorsque bouton est supérieur à 2, il est remis à 0. Cela permet d'afficher les 3 axes (X,Y,Z) sachant que pour bouton : 0=X, 1=Y, 2=Z. De plus, cette routine affiche dans le terminal la valeur du bouton pour que l'utilisateur puisse savoir l'axe affiché.
\
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/907828dd-f3dc-4bef-bcfd-38bb1c80dde8)


3. `irqhandler_timer()` : Routine d’interruption du timer, le flag d’interruption est remis à 0 à chaque fois qu'une routine est lancé. Cette routine est lancé toutes les secondes et rassemble plusieurs fonctions. En effet, elle communique avec le gyroscope pour lire les données des registres des valeurs de sortie (J'ai réécrit les fonctions read et write en I2C pour lire et écrire directement dans les registre du gyroscope. Les valeurs des registre sont sur 8 bits, pour un axe données les valeurs sont à reconstruire avec X0 octets de poids faible et X1 octets de poids fort. Il faut alors reconstruire les valeurs en effectuant un décalage à droite et une addition pour obtenir les valeurs X, Y et Z sur 16 bits signé.\
Ensuite, la routine affiche les valeurs sur les 7 segments suivant la valeur du bouton (pour afficher l'axe souhaité). Cependant, avant d'afficher avec la fonction `display_segment()`, il faut vérifier si la valeurs est signé négativement et dans ce cas calculé le complément à 2 de cette valeur avec `complement_2()`.

4. `complement_2()` : Fonction qui analyse si la valeur d'entré est un entier négatif, et calcul son complément à 2. De plus, cette fonction mets à jour le pointeur "negatif" à 1 si le nombre est négatif.
5. `display_segment()` : Fonction qui affiche la valeur du gyroscope sur la carte avec les 5 digits 7 segments. 4 digits sont utilisés pour afficher le nombre (une conversion BCD sur 4 chiffres est réalisé), et un digit est utilisé pour afficher le signe "-" dans le cas d'une valeur négative, sinon ce dernier est éteint.
6. `main()` : Utiliser pour enregistrer les 2 routines d'interruptions (celle du timer et du bouton) 

## Calibration

## Résultat
Verification du fonctionnement du compteur sans timer (on vérifie ici si le compteur va jusqu'à 999 et revient à 0)

https://github.com/ESN2024/roche_lab2/assets/116710033/93f47ef4-0973-429a-8db5-6de4d433b587

Validation de fonctionnement du système

https://github.com/ESN2024/roche_lab2/assets/116710033/e39876a7-0a60-43ff-96be-42ec67c33ff5

Le système fonctionne correctement.

## Conclusion

Ce projet m'a permis d'apprendre à contrôler les 7 segments de la carte DE-10 Lite. J'ai mis du temps à comprendre l'architecture du système, on vient contrôler les PIOs sur 4 bits, ensuite un fichier .vhdl convertie ces 4 bits en sortie 7 bits pour contrôler les 7 segments. Je n'avais pas refait de VHDL depuis presque 1 an. Le fait de reprendre la création de composants et de les lier entre eux m'a permis de me replonger dans le VHDL. J'ai fait quelques erreurs d'étourderie, par exemple au début mon compteur en C était défini en 8 bits donc mon compteur compté jusqu'à 255. Ensuite, j'ai eu quelques soucis à implémenter la conversion BCD j'ai du ré-étudier les divisions euclidiennes et l'utilisation de l'opérateur modulo en C. 

Ce projet était très intéressant car il permet de revoir les concepts de base du VHDL.
