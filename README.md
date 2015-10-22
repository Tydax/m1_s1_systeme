#Architecture et conception des Systèmes d'Exploitation

###TP n°6

###Deuxième couche logicielle : gestion de volumes

**Authors:**
* Armand BOUR
* Tristan CAMUS

**/include :**
* driver.h
* hardware.h

**/lib :**
* libhardware.a

**/src :**
* makefile
* driver.c
* dmps.c
* frmt.c

Le makefile peut générer deux programmes via ces commandes :
* *make dmps.out*
* *make frmt.out*

Sinon il est possible de tous les générer via la commande *make all*.

Pour utiliser le makefile, se placer dans le dossier /src.