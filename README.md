
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
\\
- **Processeur Nios II**
- **Clock**
- **JTAG** : Permet de debugger
- **Mémoire** : RAM avec un stockage de 40 Mo.

- **Eléments propre au sujet :**
\\
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
\\
1. `irqhandler_bouton_key1()` : Routine d’interruption du bouton key1, le flag d’interruption est remis à 0 à chaque fois qu'une routine est lancé. Cette routine incrémente la variable bouton lorsque qu'on appuie sur le bouton. Elle est sensible uniquement au front descendant, cela permet d'incrémenter de 1 pour un appuie et relachement du bouton (front montant et descendant). Lorsque bouton est supérieur à 2, il est remis à 0. Cela permet d'afficher les 3 axes (X,Y,Z) sachant que pour bouton : 0=X, 1=Y, 2=Z. De plus, cette routine affiche dans le terminal la valeur du bouton pour que l'utilisateur puisse savoir l'axe affiché.
\
![image](https://github.com/ESN2024/roche_lab3/assets/116710033/907828dd-f3dc-4bef-bcfd-38bb1c80dde8)


3. `irqhandler_timer()` : Routine d’interruption du timer, le flag d’interruption est remis à 0 à chaque fois qu'une routine est lancé. Cette routine est lancé toutes les secondes et rassemble plusieurs fonctions. En effet, elle communique avec le gyroscope pour lire les données des registres des valeurs de sortie (J'ai réécrit les fonctions read et write en I2C pour lire et écrire directement dans les registre du gyroscope. Les valeurs des registres sont sur 8 bits, pour un axe les valeurs sont à reconstruire avec X0 : octets de poids faible et X1 : octets de poids fort. Il faut alors reconstruire les valeurs en effectuant un décalage à droite et une addition pour obtenir les valeurs X, Y et Z sur 16 bits signé.\
Ensuite, la routine affiche les valeurs sur les 7 segments suivant la valeur du bouton (pour afficher l'axe souhaité). Cependant, avant d'afficher avec la fonction `display_segment()`, il faut vérifier si la valeurs est signé négativement et dans ce cas calculé le complément à 2 de cette valeur avec `complement_2()`.

4. `complement_2()` : Fonction qui analyse si la valeur d'entré est un entier négatif, et calcul son complément à 2. De plus, cette fonction mets à jour le pointeur "negatif" à 1 si le nombre est négatif.
5. `display_segment()` : Fonction qui affiche la valeur du gyroscope sur la carte avec les 5 digits 7 segments. 4 digits sont utilisés pour afficher le nombre (une conversion BCD sur 4 chiffres est réalisé), et un digit est utilisé pour afficher le signe "-" dans le cas d'une valeur négative, sinon ce dernier est éteint.
6. `main()` : Utiliser pour enregistrer les 2 routines d'interruptions (celle du timer et du bouton), on l'utilise aussi pour mettre les valeurs de sortie en mode fullrange en mettant le registre 0x31 à 8. Ensuite, une boucle infinie vide fait tourner le programme principal à l'attente des flags d'interruptions.

## Calibration
J'ai essayé de calculer les offset avec les formules de la documentation, dans un premier temps j'ai mis les offset à 0 sur les 3 axes (avec la fonction `ADXL345_SET_OFFSET_0()`) puis j'ai relevé la valeurs des axes sur le plan Z. En résultat je dois avoir (x,y,z) => (0,0,1000) en mg. Cependant, ma sortie n'est pas en mg ! Il faut rajouter un facteur de 3,9 sur les registres de sortie de l'acceleromètre pour avoir des résultats en mg (voir documentation). Finalement, les calculs des offset avec la formule de la documentation n'était pas très efficace. Cela m'a permis de me rapprocher de la valeur parfaite mais j'ai du finir par dichotomie en effectuant des tests pour avoir un résultat au plus proche des valeurs souhaité. 
\\
J'ai ainsi comme valeurs en offset :
-OFSX = 0
-OFSY = 5
-OFSZ = -760

## Résultat

J'ai eu du mal à comprendre le fonctionnement des valeurs entre entier signé et non-signé et comprendre la reconstruction des valeurs en full range avec le coefficient de 3,9 pour avoir les résultat en mg.

Verification du fonctionnement du système sur le plan X :

https://github.com/ESN2024/roche_lab3/assets/116710033/219fbe07-bbe2-41ad-b579-e6aee1bfeb61

Verification du fonctionnement du système sur le plan Y :

https://github.com/ESN2024/roche_lab3/assets/116710033/a15f0de2-32ba-4f61-84ce-bdbaa9a63d88

Verification du fonctionnement du système sur le plan  Z :

https://github.com/ESN2024/roche_lab3/assets/116710033/4e213f81-11a2-4b57-8630-6f05d2b760df

Finalement, les valeurs sont proches des résultats souhaité c'est à dire pour X : (1000,0,0), Y : (0,1000,0), Z : (0,0,1000).

## Conclusion

Ce projet était très intérèssant car il a repris les travaux effectué lors du lab1 et lab2. La difficulté pour moi a été de bien lire et comprendre la documentation du gyroscope, l'implémentation ensuite n'était pas très compliqué en termes de developpement sur Quartus et en C. J'ai eu du mal à comprendre correctement la documentation à propos des résolutions sur les offsets et sur les valeurs de sortie. Par conséquent, j'ai mis du temps à calculer les offsets et à avoir des valeurs cohérentes sur la carte. Par exemple, j'avais un problème de variable int16 qui se transformaient en int32, donc mes valeurs de sortie s'affichaientt avec des valeurs abberantes (8000,-9000) j'ai résolue cela en modifiant la résolution en full range.
