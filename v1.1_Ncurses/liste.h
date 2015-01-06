
#include "header.h"

void liste_init(void);
int liste_est_vide(void);
int liste_est_hors_liste(void);
void liste_en_tete(void);
void liste_en_queue(void);
void liste_precedent(void);
void liste_suivant(void);
void liste_valeur_elt( t_coord* e);
void liste_modif_elt(t_coord v);
void liste_oter_elt(void);
void liste_ajout_droit(t_coord e);
void liste_ajout_gauche(t_coord e);
void liste_afficher_contenu();
void liste_afficher_queue();
void liste_suppr_doublon();
int liste_calculer_nombre_elements();
int liste_element_est_present(t_coord e, int numPile);
