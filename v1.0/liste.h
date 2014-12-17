
#include "header.h"


void init_liste(void);
int liste_vide(void);
int hors_liste(void);
void en_tete(void);
void en_queue(void);
void precedent(void);
void suivant(void);
void valeur_elt( t_coord* e);
void modif_elt(t_coord v);
void oter_elt(void);
void ajout_droit(t_coord e);
void ajout_gauche(t_coord e);
void afficher_liste();
void afficher_queue();
void suppr_doublon();
int calculerElemListe();
