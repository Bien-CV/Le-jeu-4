/* Mise en oeuvre contigue d'une liste d'entiers */
#define N 100
//#include "file.h"
#include "liste.h"
#include<stdio.h>

/* Déclaration du tableau contenant les éléments de liste */
t_coord T[N];
/* Déclaration des indices de queue et d'élément courant */
int queue, ec;

void init_liste(void)
/* Initialise la liste à vide */
{
	queue = -1;
	ec = -1;
}

int liste_vide(void)
/* Rend vrai si la liste est vide, faux sinon */
{
	return (queue== -1);
}

int hors_liste(void)
/* Rend vrai si l'elt courant est hors de la liste, faux sinon */
{
	return(ec < 0 || ec > queue);
}

void en_tete(void)
/* Positionne en tete de la liste */
{
	if(!liste_vide())
		ec = 0;
}

void en_queue(void)
/* Positionne en queue de la liste */
{
	if(!liste_vide())
		ec = queue;
}

void precedent(void)
/* Positionne sur l'elt precedent*/
{
	if(!hors_liste())
		ec--;
}

void suivant(void)
/* Positionne sur l'elt suivant*/
{
	if(!hors_liste())
		ec++;
}

void valeur_elt(t_coord* e)
/* Renvoie dans e la veleur de l'elt courant */
{
	if(!hors_liste())
		*e= T[ec];
}

void modif_elt(t_coord v)
/* Affecte v à l'elt courant */
{
	if(!hors_liste())
		T[ec]=v;
}

void oter_elt(void)
/* Supprime l'elt courant et positionne sur le precedent */
{
	int i;
	if(!hors_liste())
	{
		for(i=ec;i<queue;i++)
			T[i]=T[i+1];
		ec--; queue--;
	}
}

void ajout_droit(t_coord e)
/* Ajoute e a droite de l'elt courant */
{
	int i;
    if(liste_vide())
    {
		T[0]=e;
		ec++; queue++;
    }
	if(!hors_liste())
	{
		for(i=queue;i>ec;i--)
			T[i+1]=T[i];
		T[ec+1]=e;
		ec= ec+1; queue++;
	}
}

void ajout_gauche(t_coord e)
/* Ajoute e a gauche de l'elt courant */
{
	int i;

	if(liste_vide())
		ec++;

	if(liste_vide()||!hors_liste())
	{
		for(i=queue;i>=ec;i--)
			T[i+1]=T[i];
		T[ec]=e;
		queue++;
	}
}

void afficher_liste()
{
    t_coord coord;
    en_tete();
    //suivant();
    printf("debut liste \n");
    while(!hors_liste())
    {
        valeur_elt(&coord);
        printf("%i * %i\n\n", coord.X, coord.Y);
        suivant();
    }
    printf("fin liste \n");
}
void afficher_queue()
{
    t_coord val;
    en_tete();
    valeur_elt(&val);
    printf("%i %i\n", val.X, val.Y);
	//printf("\n%i %i\n",queue,ec);
}

void suppr_doublon()
{
    int indice, indice1, indice2 = 0;
    t_coord valcour;
    t_coord verif;

    for(indice1 = 0; indice1 <= queue; indice1++)
    {
        en_tete();
        for(indice = 0; indice < indice1; indice++)
            suivant();
        valeur_elt(&valcour);
        suivant();
        while(!hors_liste())
        {
            valeur_elt(&verif);
            if(valcour.X == verif.X && valcour.Y == verif.Y)
                oter_elt();
            suivant();
            indice2++;
        }
    }
}

int calculerElemListe()
{
    int nb=0;
    if(!liste_vide())
    {
        en_tete();
        while(!hors_liste())
        {
            nb++;
            suivant();
        }
    }
    return nb;
}
