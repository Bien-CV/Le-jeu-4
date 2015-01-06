#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <ncurses.h>	
	
	typedef struct { int or ; int argent ;}t_argent;
	typedef struct { t_argent argent ; int joyaux ;}t_richesses;
	
	typedef struct{ t_richesses money ; int age ;} t_character;


void reinitialiser_perso(t_character plateau[10][10])
{
	
	plateau->perso.money=(t_richesses){{0,0},0};
	
}

void reinitialiser_perso2(t_character* plateau[10][10])
{
	
	plateau->perso->money.argent.or=0;
	
}

void afficher_perso(t_character perso)
{
	printf("Or:%i Argent:%i Joyaux:%i Age:%i\n",perso.money.argent.or,perso.money.argent.argent,perso.money.joyaux,perso.age);
}

int main()
{

	t_richesses riche={{1000000,1000000},100000};
	t_richesses pas_riche={{2,3},1};

	
	t_character plateau[10][10];
	
	plateau[0][0].money=riche;
	plateau[0][0].age=143;
	plateau[0][1].money=pas_riche;    
	plateau[0][1].age=13;
	
	
	reinitialiser_perso(plateau);
	reinitialiser_perso2(plateau);
	afficher_perso(plateau[0][0]);
	afficher_perso(plateau[0][1]);
	
	
	return 1;
	
}
