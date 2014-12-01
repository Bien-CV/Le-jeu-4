#include "file.h"
#define TMAX 100
static t_coord file[TMAX];	//tableau contenant les valeurs de la file
static int tete, queueFile, nbVal;	//indice de tete, queueFile et nombre de valeurs dans la file

void initFile() //initialisation la file à vide
{
	nbVal = 0;
	tete = 0;
	queueFile = -1;
	//queueFile = TMAX - 1;
}

void ajouterFile(t_coord a)	//ajoute la valeur en queueFile de file
{
	if(nbVal < TMAX)
	{
		if(queueFile == TMAX-1)
			queueFile = 0;
		else
			queueFile++;
		file[queueFile] = a;
		nbVal++;
	}
}

void retirerFile(t_coord * val)	//retire la valeur en tête de file et l'affecte à val
{
	if(nbVal>0)
	{
		*val = file[tete];
		nbVal--;
		if(tete == TMAX-1)
			tete=0;
		else
			tete++;
	}
}

int fileVide()	//délivre vrai si la file est vide, faux sinon
{
	return(nbVal==0);
}

void afficher_queueFile()
{
    printf("%i\n", queueFile);
}


void afficher_elt(t_coord val)
{
    int indice = tete;
    while(indice < nbVal)
    {
        if(nbVal>0)
        {
            val = file[indice];
            if(indice == TMAX-1)
                indice=0;
            else
                indice++;
        }
        printf("X: %i, Y: %i\n", val.X, val.Y );
    }
}
