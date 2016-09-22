## Architecture et conception des Systèmes d'Exploitation
### TP n°6
### Deuxième couche logicielle : gestion de volumes

**Auteurs :**
* Armand BOUR
* Tristan CAMUS

**/include :**
* driver.h
* hardware.h
* mbr.h

**/lib :**
* libhardware.a

**/src :**
* makefile
* driver.c
* dvol.c
* mbr.c
* mkvol.c

Le makefile peut générer deux programmes via ces commandes :
*`make dvol.out`
*`make mkvol.out`

`mkvol.out -s size -fc firstcylinder [-fs sector] [-n nameofvolume]` creates a new volume with the specified parameters.
`dvol.out [nameofvolumetodisplay [nameofvolumetodisplay_1]]` displays all blocks inside each volume of the disk. You can specify the name of the volume(s) you want to display.

Sinon il est possible de tous les générer via la commande `make all`.

Pour utiliser le makefile, se placer dans le dossier **/src**.