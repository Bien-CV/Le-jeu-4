/* Mise en oeuvre contigue d'une liste d'entiers */
#define N_LISTE 100
#include "liste.h"
#include "header.h"
#include<stdio.h>

/* Déclaration du tableau contenant les éléments de liste */
t_coord T[N_LISTE];
/* Déclaration des indices de queue et d'élément courant */
int queue, ec;

void liste_init(void)
/* Initialise la liste à vide */
{
	queue = -1;
	ec = -1;
}

int liste_est_vide(void)
/* Rend vrai si la liste est vide, faux sinon */
{
	return (queue== -1);
}

int liste_est_hors_liste(void)
/* Rend vrai si l'elt courant est hors de la liste, faux sinon */
{
	return(ec < 0 || ec > queue);
}

void liste_en_tete(void)
/* Positionne en tete de la liste */
{
	if(!liste_est_vide())
		ec = 0;
}

void liste_en_queue(void)
/* Positionne en queue de la liste */
{
	if(!liste_est_vide())
		ec = queue;
}

void liste_precedent(void)
/* Positionne sur l'elt liste_precedent*/
{
	if(!liste_est_hors_liste())
		ec--;
}

void liste_suivant(void)
/* Positionne sur l'elt liste_suivant*/
{
	if(!liste_est_hors_liste())
		ec++;
}

void liste_valeur_elt(t_coord* e)
/* Renvoie dans e la veleur de l'elt courant */
{
	if(!liste_est_hors_liste())
		*e= T[ec];
}

void liste_modif_elt(t_coord v)
/* Affecte v à l'elt courant */
{
	if(!liste_est_hors_liste())
		T[ec]=v;
}

void liste_oter_elt(void)
/* Supprime l'elt courant et positionne sur le liste_precedent */
{
	int i;
	if(!liste_est_hors_liste())
	{
		for(i=ec;i<queue;i++)
			T[i]=T[i+1];
		ec--; queue--;
	}
}

void liste_ajout_droit(t_coord e)
/* Ajoute e a droite de l'elt courant */
{
	int i;
    if(liste_est_vide())
    {
		T[0]=e;
		ec++; queue++;
    }
	if(!liste_est_hors_liste())
	{
		for(i=queue;i>ec;i--)
			T[i+1]=T[i];
		T[ec+1]=e;
		ec= ec+1; queue++;
	}
}

void liste_ajout_gauche(t_coord e)
/* Ajoute e a gauche de l'elt courant */
{
	int i;

	if(liste_est_vide())
		ec++;

	if(liste_est_vide()||!liste_est_hors_liste())
	{
		for(i=queue;i>=ec;i--)
			T[i+1]=T[i];
		T[ec]=e;
		queue++;
	}
}

void liste_afficher_contenu()
{
    t_coord coord;
    liste_en_tete();
    //liste_suivant();
    printf("debut liste \n");
    while(!liste_est_hors_liste())
    {
        liste_valeur_elt(&coord);
        printf("%i * %i\n\n", coord.X, coord.Y);
        liste_suivant();
    }
    printf("fin liste \n");
}
void liste_afficher_queue()
{
    t_coord val;
    liste_en_tete();
    liste_valeur_elt(&val);
    printf("%i %i\n", val.X, val.Y);
	//printf("\n%i %i\n",queue,ec);
}

void liste_suppr_doublon()
{
    int indice, indice1, indice2 = 0;
    t_coord valcour;
    t_coord verif;

    for(indice1 = 0; indice1 <= queue; indice1++)
    {
        liste_en_tete();
        for(indice = 0; indice < indice1; indice++)
            liste_suivant();
        liste_valeur_elt(&valcour);
        liste_suivant();
        while(!liste_est_hors_liste())
        {
            liste_valeur_elt(&verif);
            if(valcour.X == verif.X && valcour.Y == verif.Y)
                liste_oter_elt();
            liste_suivant();
            indice2++;
        }
    }
}

int liste_calculer_nombre_elements()
{
    int nb=0;
    if(!liste_est_vide())
    {
        liste_en_tete();
        while(!liste_est_hors_liste())
        {
            nb++;
            liste_suivant();
        }
    }
    return nb;
}
