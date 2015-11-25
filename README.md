##Architecture et conception des Systèmes d'Exploitation

###Système de fichier à la UNIX

**Authors:**
* Armand BOUR
* Tristan CAMUS

**/include :**
* block.h
* driver.h
* hardware.h
* inode.h
* mbr.h

**/lib :**
* libhardware.a

**/src :**
* makefile
* block.c
* dfs.c
* dmps.c
* dvol.c
* driver.c
* fillinode.c
* fillvolume.c
* frmt.c
* inode.c
* mbr.c
* mknfs.c
* mkvol.c

`dfs.out` affiche tous les volumes du disque, et pour le disque courant, il affiche également l'espace libre.

`dmps.out [cylinder] [sector]` affiche les données du secteur donné sur le cylindre spécifié.

`dvol.out [nameofvolumetodisplay [nameofvolumetodisplay_1]]` affiche tous les blocs de données sur chaque volume du disque. Il est possible de spécifier le nom du ou des volumes que l'on souhaite afficher.

`fillinode.out` crée un inode, et alloue le plus de blocs possibles via celui-ci, puis vérifie que tous les blocs sont bien alloués. Ce programme teste la bibliothèque *inode*.

`fillvolume.out` remplit le disque en allouant le plus de blocs possibles, avant d'en libérer certains, puis les réalloue. Ce programme teste la bibliothèque *superblock*.

`frmt.out` formate le disque entier.

`mknfs.out` initialise un nouveau volume courant avec son superblock sur le disque(cylinder: 1, sector: 0, size: 180).

`mkvol.out -s size -fc firstcylinder [-fs sector] [-n nameofvolume]` crée un nouveau volume avec les paramètres spécifiés.

Sinon il est possible de tous les générer via la commande `make all`, ce qui nettoyera également les .o.