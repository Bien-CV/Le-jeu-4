
#include "header.h"

void file_init(); //initialisation la file à vide
void file_ajouter(t_coord a);	//ajoute la valeur en queue de file
void file_retirer(t_coord * val);	//retire la valeur en tête de file et l'affecte à val
int file_vide();	//délivre vrai si la file est vide, faux sinon
void file_liste_afficher_queue();
void file_afficher_elt(t_coord val);
