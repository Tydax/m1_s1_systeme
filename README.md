##Architecture et conception des Systèmes d'Exploitation

###TP n°7

###Deuxième couche logicielle : gestion de volumes

**Authors:**
* Armand BOUR
* Tristan CAMUS

**/include :**
* block.h
* driver.h
* hardware.h
* mbr.h

**/lib :**
* libhardware.a

**/src :**
* makefile
* block.c
* dfs.c
* driver.c
* fillvolume.c
* mbr.c
* mknfs.c

Le makefile peut générer deux programmes via ces commandes :
* *make dfs.out*
* *make fillvolume.out*
* *make mknfs.out*

`dfs.out` displays all the volumes on the disk, and for the current volume, displays the free space.

`fillvolume.out` fills the disk by allocating as many blocks as it can, then frees some, then re-allocates them. This program tests the superblock library.

`mknfs.out` initialises a new current volume with its superblock on the disk (cylinder: 1, sector: 0, size: 50).

Sinon il est possible de tous les générer via la commande *make all*.

Pour utiliser le makefile, se placer dans le dossier /src.