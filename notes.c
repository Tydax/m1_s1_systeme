// Exercice 1
init_hardware("config");

// Récupérer l’octet de poids fort
(cyl >> 8) &0xFF
// Récupérer l’octet de poids faible
cyl &0xFF

// Faire des _out pour envoyer les informations une à une
_out(HDA_DATAREGS+0/+1); // Passer les valeurs de cyl
_out(HDA_DATAREGS+2/+3); // Passer les valeurs de sec
_out(HDA_CMDREG, CMD_SEEK); // Indique la commande
_sleep(HDA_IRQ);

write(STDOUT_FILENO, MASTERBUFFER...);
./dmps | od -tx1


// Exercice 2
