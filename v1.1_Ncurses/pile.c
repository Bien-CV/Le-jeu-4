<<<<<<< HEAD
/**
 *\file pile.c
 *\brief Projet informatique S3 Deplacement du personnage et colisions sur le parois
 *\author TOULMONDE Joris
 *\version 0.2
 *\date 22 Octobre 2014
*/

#include <stdio.h>
#include <stdlib.h>

#include "pile.h"
#include "header.h"

#define M 100

//typedef struct {int x, y;}t_coordonees;

static t_coord pile[M][M];
static int sommet[M];

void pile_init(int numPile){
	sommet[numPile]=-1;
}

void pile_empiler(t_coord coord, int numPile){
	if(sommet[numPile]<M-1){
		sommet[numPile]++;
		pile[numPile][sommet[numPile]].X=coord.X;
		pile[numPile][sommet[numPile]].Y=coord.Y;
		//pile[sommet] = coord;
	}
}

void pile_depiler(t_coord *coord, int numPile){
	if(sommet[numPile]>-1){
		coord->X=pile[numPile][sommet[numPile]].X;
		coord->Y=pile[numPile][sommet[numPile]].Y;
		sommet[numPile]--;
	}
}

int pile_sommet(t_coord *coord, int numPile){
	if(sommet[numPile]>-1){
		coord->X=pile[numPile][sommet[numPile]].X;
		coord->Y=pile[numPile][sommet[numPile]].Y;
		return 1;
	}
	return 0;
}

int pilevide(int numPile){
	return(sommet[numPile]==-1);
}
=======
/**
 *\file pile.c
 *\brief Projet informatique S3 Deplacement du personnage et colisions sur le parois
 *\author TOULMONDE Joris
 *\version 0.2
 *\date 22 Octobre 2014
*/

#include <stdio.h>
#include <stdlib.h>

#include "pile.h"
#include "header.h"

#define M 100

//typedef struct {int x, y;}t_coordonees;

static t_coord pile[M][M];
static int sommet[M];

void pile_init(int numPile){
	sommet[numPile]=-1;
}

void pile_empiler(t_coord coord, int numPile){
	if(sommet[numPile]<M-1){
		sommet[numPile]++;
		pile[numPile][sommet[numPile]].X=coord.X;
		pile[numPile][sommet[numPile]].Y=coord.Y;
		//pile[sommet] = coord;
	}
}

void pile_depiler(t_coord *coord, int numPile){
	if(sommet[numPile]>-1){
		coord->X=pile[numPile][sommet[numPile]].X;
		coord->Y=pile[numPile][sommet[numPile]].Y;
		sommet[numPile]--;
	}
}

int pile_sommet(t_coord *coord, int numPile){
	if(sommet[numPile]>-1){
		coord->X=pile[numPile][sommet[numPile]].X;
		coord->Y=pile[numPile][sommet[numPile]].Y;
		return 1;
	}
	return 0;
}

int pilevide(int numPile){
	return(sommet[numPile]==-1);
}
>>>>>>> 895dfc0c880b2a01564b976068bc831322e6f36d
