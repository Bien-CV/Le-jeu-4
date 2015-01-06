#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <ncurses.h>


#include "header.h"
#include "liste.h"
#include "file.h"
#include "pile.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


//Prototypes fonctions, à inclure dans un futur Minimale.h !
int life_check(int joueur_courant);
void joueur_liste_suivant(int nb_joueur,int* joueur_courant);
void creer_terrain_rapide(t_camp camp,int x, int y);
void vider_buffer(void);
int are_my_mates_alive(int joueur_courant);
int calcul_persos_IA(int joueur_courant);
void afficher_plateau_orientation(int joueur_courant);
void generation_nom_personnage(char * nom);
int compteur_tour=0;
int compteur_joueurs_vivants=0;
t_character Valid_chars_IA[MaxTab];

char particule_generateur_nom[][20] = 
	{
		"ogre",
		"etoileux",
		"mur",
		"elfe",
		"borgne",
		"attendrissant",
		"enfant",
		"planetaire",
		"rayon",
		"magique",
		"terrible",
		"sorcier",
		"guerrier",
		"sauvage",
		"perverti",
		"bienveillant",
		"saint"
	};
	

t_character selected_character;
void players_life_check();
t_character Plateau[TAILLE_MATRICE][TAILLE_MATRICE];
t_coord depValid[TAILLE_MATRICE*TAILLE_MATRICE][3];
int nbDepValid;

t_player player[MaxTab];



int indiceTabDepValid;

/**
 * \fn void Sauvegarder()
 * \brief Sauvegarde la partie dans un fichier
 *
 */
void Sauvegarder(int joueur_courant){
    char NomFichier[20]="",FichierSauvegarde[20]="";
    int present=1;

    FILE * fichierIndex;
    FILE * sauvegarde;
    fichierIndex=fopen("IndexSauvegarde.txt","r");
    fscanf(fichierIndex,"%s",FichierSauvegarde);
    if(!feof(fichierIndex)){
        printw("Sauvegarde deja existante :\n");

        while(!feof(fichierIndex))
        {
            printw("%s\n",FichierSauvegarde);
            fscanf(fichierIndex,"%s",FichierSauvegarde);
        }
    }else {printw("Pas de sauvegarde disponible.\n");
    }
    printw("\n");
    fclose(fichierIndex);
    fichierIndex=fopen("IndexSauvegarde.txt","r+");
    printw("Entrez un nom de sauvegarde :");
    getstr(NomFichier);
    while(!feof(fichierIndex))
    {
        fscanf(fichierIndex,"%s",FichierSauvegarde);
        if(!strcmp(NomFichier,FichierSauvegarde))
        {
            present=0;
        }
    }
    if(present) fprintf(fichierIndex,"%s ",NomFichier);
    sauvegarde=fopen(NomFichier,"wb");
    fwrite(Plateau,TAILLE_MATRICE*TAILLE_MATRICE,sizeof(t_character),sauvegarde);
    fwrite(player,sizeof(player)/sizeof(t_player),sizeof(t_player),sauvegarde);
    fwrite(&joueur_courant,sizeof(int),sizeof(int),sauvegarde); //Que faire avec &joueur_courant ?
    fclose(fichierIndex);
    fclose(sauvegarde);
}

/**
 * \fn void Charger()
 * \brief Permet de charger une partie depuis un fichier
 *
 */
void Charger(int joueur_courant){
    char NomFichier[20]="",FichierSauvegarde[20]="";

    FILE * fichierIndex;
    FILE * sauvegarde;
    fichierIndex=fopen("IndexSauvegarde.txt","r");
    fscanf(fichierIndex,"%s",FichierSauvegarde);
    if(!feof(fichierIndex)){
        printw("Sauvegarde deja existante :\n");

        while(!feof(fichierIndex))
        {
                printw("%s\n",FichierSauvegarde);
                fscanf(fichierIndex,"%s",FichierSauvegarde);
        }
        printw("\n");
        do{
                printw("Choisir un fichier : ");
                getstr(NomFichier);
                sauvegarde=fopen(NomFichier,"rb");
            if(sauvegarde!=NULL){
                fread(Plateau,TAILLE_MATRICE*TAILLE_MATRICE,sizeof(t_character),sauvegarde);
                fread(player,sizeof(player)/sizeof(t_player),sizeof(t_player),sauvegarde);
                fread(&joueur_courant,sizeof(t_camp),sizeof(t_camp),sauvegarde);
            }else {printw("Fichier incorrect\n");}
        }while(!feof(sauvegarde));
            fclose(sauvegarde);
            afficher_plateau_orientation(joueur_courant);
        }else {
printw("Pas de sauvegarde disponible.\n");
        }
        fclose(fichierIndex);
}



/**
 * \fn int generation_nombre_aleatoire(int max)
 * \brief Fonction qui renvoi un nombre aléatoire en 0 et 'max'
 *
*/
int generation_nombre_aleatoire(int max)
{
		return (rand()%max+1);
}

/**
 * \fn void deplacer_perso(t_coord case_perso)
 * \brief Déplace le personnage sur le terrain
 *
 */
void deplacer_perso(t_coord case_perso)
{
    Plateau[selected_character.position.X][selected_character.position.Y]= case_terrain ;
    if(Plateau[case_perso.X][case_perso.Y].type == TRAP_UNIT){selected_character.status.HP -= Plateau[case_perso.X][case_perso.Y].stats.ATK;}	//appliquation des dégâts du piège!!!//par défaut, le piège enlève 20 HP!!!!
    selected_character.position.X=case_perso.X;
    selected_character.position.Y=case_perso.Y;
    Plateau[case_perso.X][case_perso.Y]= selected_character; //la case du terrain (case_perso) prend le personnage "selected_chaacter"!

}

/**
 * \fn int cases_voisines_calcul(t_coord coordonnees)
 * \brief Renvoi le nombre de case voisine vide, met dans la file la liste des coordonnées voisines accessibles
 *
 */
int cases_voisines_calcul(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
    if(0 == Plateau[coordonnees.X+1][coordonnees.Y].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.X += 1;
        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }
    }
    if(0 == Plateau[coordonnees.X-1][coordonnees.Y].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }
    }
    if(0 == Plateau[coordonnees.X][coordonnees.Y+1].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }
    }
    if(0 ==Plateau[coordonnees.X][coordonnees.Y-1].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }
    }
    return(nbVois);
}


/**
 * \fn void deplacements_valides()
 * \brief Calcule les positions de déplacement valide, les met dans la liste
 *
 */
void deplacements_valides(){// permet de calculer les positions valides pour son perso.
    int mvtEffectue = 0;
    int nbVoisins=1;
    nbDepValid = 0;
    int nbBoucle;
    liste_init();
    file_init();

    int indice;
    t_coord coordonnees = selected_character.position;
    
    file_ajouter(coordonnees);
    liste_ajout_droit(coordonnees); 
    //liste_valeur_elt(&coordonnees);

    while(mvtEffectue <= selected_character.stats.MVT)
    {
        nbBoucle = nbVoisins;
        nbVoisins = 0;
        
        for(indice = 0; indice < nbBoucle; indice++)
        {



            file_retirer(&coordonnees);  
            liste_ajout_droit(coordonnees);	

            if(mvtEffectue < selected_character.stats.MVT)
                nbVoisins += cases_voisines_calcul(coordonnees);
        }
        mvtEffectue++;
    }
    liste_suppr_doublon();
    nbDepValid = liste_calculer_nombre_elements();

    //liste_afficher_contenu();
}

/**
 * \fn t_coord choix_deplacement_humain(joueur_courant)
 * \brief Permet au joueur de choisir la destination
 *
 */
t_coord choix_deplacement_humain(int joueur_courant){
	//***************/V1.2/*****************/
		int mvt_effectue = 0, x, y;
		char choix;
		x = selected_character.position.X;
		y = selected_character.position.Y;
		t_coord val = {x,y},sommet;
		pile_init(0);
		pile_empiler(val, 0);
		pile_sommet(&sommet,0);
	do
	{
		clear();
		if(Plateau[val.X][val.Y].type == TRAP_UNIT){move(TAILLE_MATRICE+3+5,0); printw("\n/!\\VOUS VOUS ETES PRIS UN PIEGE/!\\"); refresh();}
		deplacer_perso(val);
		move(0,0);
		printw("Deplacement: %i/%i\n", mvt_effectue, selected_character.stats.MVT);
		afficher_plateau_orientation(joueur_courant);
		printw("\n\nValider =\"Entrée\"\nAnnuler =\"BackSpace\"");
		mvaddch(y+2,x+3,curseur);
		//refresh();
		choix = getch();
		if(choix == 127 && mvt_effectue > 0)
		{
			mvt_effectue--;
			pile_depiler(&val, 0);
			x = val.X;
			y = val.Y;
		}
		if(choix == 68)
		{
			
			if(x > 0)
			{
				x--;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character.stats.MVT )
					{
						mvt_effectue++;
						pile_empiler(val, 0);
						val = (t_coord){x,y};
						
					}else
					{
						x++;
					}
				}else
				{
					x++;
				}
			}
		}
		if(choix == 67)
		{
			if(x < TAILLE_MATRICE)
			{
				x++;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character.stats.MVT )
					{
						mvt_effectue++;
						pile_empiler(val, 0);
						val = (t_coord){x,y};
					}else
					{
						x--;
					}
				}else
				{
					x--;
				}
			}
		}
		if(choix == 65)
		{
			if(y > 0)
			{
				y--;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character.stats.MVT )
					{
						mvt_effectue++;
						pile_empiler(val, 0);
						val = (t_coord){x,y};
					}else
					{
						y++;
					}
				}else
				{
					y++;
				}
			}
		}
		if(choix == 66)
		{
			if(y < TAILLE_MATRICE)
			{
				y++;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character.stats.MVT )
					{
						mvt_effectue++;
						pile_empiler(val, 0);
						val = (t_coord){x,y};
					}else
					{
						y--;
					}
				}else
				{
					y--;
				}
			}
		}
		pile_sommet(&sommet,0);
		
	}while(choix != '\n' );
	
	return((t_coord){x,y});
	
	//***************/V1.2/*****************/Fin
    /*int i = 0, j, choix = 1;
    vider_buffer();
    t_coord coordonnees, choisi;
    liste_en_tete();
    printw("Quel Deplacement voulez-vous effectuer ? \n");
    while(!liste_est_hors_liste()){
        liste_valeur_elt(&coordonnees);
        liste_suivant();
        printw("   %i - (%i,%i)\n",i+1, coordonnees.X, coordonnees.Y);
        i++;
    }

    while(choix){
        scanf("%i",&j);
        if(j<=nbDepValid && j > 0)
        {
            liste_en_tete();
            for(i = 0; i < j-1; i++)
                liste_suivant();
            liste_valeur_elt(&choisi);
            coordonnees.X=choisi.X;
            coordonnees.Y=choisi.Y;
            choix=0;
        }else{printw("Numero incorrect\n");}
    }
    printw("\n");
    return (coordonnees);*/
}

t_coord choix_deplacement_IA(){
	
    int i = 0, choix;
    t_coord coordonnees, choisi;
    liste_en_tete();
       while(!liste_est_hors_liste()){
        liste_valeur_elt(&coordonnees);
        liste_suivant();
        //printw("   %i - (%i,%i)\n",i+1, coordonnees.X, coordonnees.Y);
        i++;
    }
    
    //printw("nbDepValid: %i\n",nbDepValid);
	
	do 
	{
		choix=generation_nombre_aleatoire(nbDepValid);
	}while (choix>nbDepValid && choix < 0);
	
       // printw("Choix: %i\n",choix);
        liste_en_tete();
        for(i = 0; i < choix-1; i++)
			liste_suivant();
        liste_valeur_elt(&choisi);
        coordonnees.X=choisi.X;
        coordonnees.Y=choisi.Y;
        printw("Déplacement IA %s en %i,%i\n",selected_character.name,coordonnees.X,coordonnees.Y);
    return (coordonnees);
}


/**
 * \fn void init_plateau()
 * \brief Initialise le plateau en le remplissant de terrain par défaut
 *
 */
void init_plateau()
{
    int i, j;
    for(j = 0; j < TAILLE_MATRICE; j++)
    {
        for(i = 0; i < TAILLE_MATRICE; i++)
        {
			creer_terrain_rapide(terrain,i,j);
        }
    }
}

int cases_voisines_ATK(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
        coord = coordonnees;
        coord.X += 1;
		
        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);
            nbVois++;
        }

    return(nbVois);
}

void viser_case_valide(t_skill skill, int* nbAtkValid)
{
    int mvtEffectue = 0;
    int nbVoisins=1;
    int nbBoucle;
    *nbAtkValid = 0;
    liste_init();
    file_init();
    int nb_iteration;
    t_coord coordonnees = selected_character.position;
    file_ajouter(coordonnees); //dans la file, on met les coordonnées des case opour explorer le terrain sur la portée du coup
    liste_ajout_droit(coordonnees); // dans la liste, on a toutes les cases attaquables !!
    //liste_valeur_elt(&coordonnees);
    while(mvtEffectue <= skill.range)
    {
        nbBoucle = nbVoisins;
        nbVoisins = 0;
        for(nb_iteration = 0; nb_iteration < nbBoucle; nb_iteration++)
        {
            file_retirer(&coordonnees);
            if(skill.type == TRAP )
            {
				if (Plateau[coordonnees.X][coordonnees.Y].camp == terrain)
					liste_ajout_droit(coordonnees);
			}else
			liste_ajout_droit(coordonnees); //ATTENTION il ne faut ajouter la case que s'il est elle compatible avec le type d'attaque
				
            if(mvtEffectue < skill.range)
                nbVoisins += cases_voisines_ATK(coordonnees);
        }
        mvtEffectue++;
    }
    liste_suppr_doublon();
    *nbAtkValid = liste_calculer_nombre_elements();
}

t_coord choix_cible_IA(t_skill skill, int* nbAtkValid)
{
    int i =0,choix;
   // t_camp tampon_camp_cible;
  // printw("Before ViderBuffer\n");
   // printw("After ViderBuffer.\n");
    t_coord coordonnees;
    liste_en_tete();

do{
   //     printw("Debut do\n");
        choix=generation_nombre_aleatoire(*nbAtkValid);
        liste_en_tete();
        for(i = 0; i < choix-1; i++)
			liste_suivant();
        liste_valeur_elt(&coordonnees);
    //    printw("Fin do\n");
    }while (!(choix<=*nbAtkValid && choix > 0));
   printw("Cible:%i,%i\n",coordonnees.X,coordonnees.Y);
    //printw("ChoixCible:%i\n",choix);

    
    return (coordonnees);
}




/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void init_char_table(t_character chars[])
{
    int i;
    for(i=0;i<MaxTab;i++) chars[i]=character_default;
}

void afficher_skill(int skill_nb, t_skill skill){
    printw("   %s ",skill.name);
    //printw("   %i - %s ",skill_nb,skill.name);
    if(skill.range==0 && skill.type != EMPTY) printw("on self ");
    if(skill.range==1) printw("melee ");
    if(skill.range>1) printw("range=%i ",skill.range);
    if(skill.type==ATK) printw("power=%i",selected_character.stats.ATK*skill.damage_coeff);
    if(skill.type==MATK) printw("power=%i",selected_character.stats.MATK*skill.damage_coeff);
    if(skill.type==EMPTY && strcmp(skill.name, "") ==0) printw(" --- ");;
    printw("\n");
}

void afficher_skill_list(t_character perso){
		afficher_skill(1,perso.skill.a);
        afficher_skill(2,perso.skill.b);
        afficher_skill(3,perso.skill.c);
        afficher_skill(4,perso.skill.d);
		afficher_skill(5,perso.skill.e);
        afficher_skill(6,perso.skill.wait);
}

void afficher_infos_persos( t_character perso)
{
	move(1, TAILLE_MATRICE+3+10);
	printw("Nom : %s ",perso.name);
	move(2, TAILLE_MATRICE+3+10);
	//printw("Camp : %s ",perso.camp);
	//printw("Position : %i,%i ",perso.position.X,perso.position.Y);
	
	
	/*if ( perso.orientation == up )
	{
		printw("Orientation: Haut ");
	}else if ( perso.orientation == down )
	{
		printw("Orientation: Bas ");
	}else if ( perso.orientation == left )
	{
		printw("Orientation: Gauche ");
	}else if ( perso.orientation == right )
	{
		printw("Orientation: Droite ");
	}else
	{
		printw("Orientation incorrecte ");
	}*/
	move(3, TAILLE_MATRICE+3+10);
	printw("%i/%i HP - %i/%i MP",perso.status.HP,perso.status.Max_HP,perso.status.MP,perso.status.Max_MP);
	move(4, TAILLE_MATRICE+3+10);
	printw("ATK:%i - MATK:%i",perso.stats.ATK,perso.stats.MATK);
	move(5, TAILLE_MATRICE+3+10);
	printw("DEF:%i - MDEF:%i",perso.stats.DEF,perso.stats.MDEF);
	move(6, TAILLE_MATRICE+3+10);
	printw("MVT:%i - Nbre action/tour:%i ",perso.stats.MVT,perso.nb_action_tour);
	//printw("%i/%i HP %i/%i MP ATK:%i MATK:%i DEF:%i MDEF:%i MVT:%i ",perso.status.HP,perso.status.Max_HP,perso.status.MP,perso.status.Max_MP,perso.stats.ATK,perso.stats.MATK,perso.stats.DEF,perso.stats.MDEF,perso.stats.MVT);
	refresh();
	move(TAILLE_MATRICE+3+1,0);
	afficher_skill_list(perso);
}	



t_coord choix_cible_humain(t_skill skill, int joueur_courant)
{
	//***************/V1.2/*****************/
    int valid = 0;
    char choix;
    int x=selected_character.position.X, y=selected_character.position.Y;
    t_coord val;
    
    
    do
	{
		clear();
		printw("Choisissez votre cible: \n", valid);
		afficher_plateau_orientation(joueur_courant);
		printw("\nCible ");
		if(valid == 0)printw("non ");
		printw("valide");
		if(Plateau[x][y].camp>0 && Plateau[x][y].type != TRAP_UNIT){afficher_infos_persos(Plateau[x][y]);}
		mvaddch(y+2,x+3,curseur);
		//refresh();
		choix = getch();
		if(choix == 127)
		{
			x = val.X;
			y = val.Y;
		}
		if(choix == 68)
		{
			
			if(x <= 0) x = TAILLE_MATRICE;
			x--;
			val = (t_coord){x,y};
		}
		if(choix == 67)
		{
			if(x >= TAILLE_MATRICE-1) x = -1;
			x++;
			val = (t_coord){x,y};
		}
		if(choix == 65)
		{
			if(y <= 0) y = TAILLE_MATRICE;
			y--;
			val = (t_coord){x,y};
		}
		if(choix == 66)
		{
			if(y >= TAILLE_MATRICE-1) y = -1;
			y++;
			val = (t_coord){x,y};
		}
		valid = liste_element_est_present(val, 0);
		
	}while(choix != '\n' || valid == 0);
	return (val);
}
	//***************/V1.2/*****************/Fin
	/*
    int i =0,choix;
    t_camp tampon_camp_cible;
    vider_buffer();
    t_coord coordonnees;
    liste_en_tete();
    printw("Choisissez votre cible. \n\n");
    while(!liste_est_hors_liste()){
        liste_valeur_elt(&coordonnees);
        tampon_camp_cible=Plateau[coordonnees.X][coordonnees.Y].camp;
        liste_suivant();
        printw("   %i - ",i+1);
        printw("%s %iHP", Plateau[coordonnees.X][coordonnees.Y].name,Plateau[coordonnees.X][coordonnees.Y].status.HP);
        if (( tampon_camp_cible>0) && (joueur==tampon_camp_cible) ){
			printw(" - Ami");
		} else if (tampon_camp_cible>0)
		{
			printw(" - %s, joueur%i",player[tampon_camp_cible].name,tampon_camp_cible);
		}else if (tampon_camp_cible==0)
		{
			printw(" - Terrain");
		}else if (tampon_camp_cible==-1)
		{
			printw(" - Obstacle");
		}
        printw(" (%i,%i)",coordonnees.X, coordonnees.Y);
        printw("\n");
        i++;
    }
    
    do{
        
        scanf("%i",&choix);
        liste_en_tete();
        for(i = 0; i < choix-1; i++)
			liste_suivant();
        liste_valeur_elt(&coordonnees);
    }while (!(choix<=nbAtkValid && choix > 0));
    return (coordonnees);*/



/**
* \fn void selection_perso(int joueur_courant)
* \brief Fonction qui propose la liste des personnages pouvant être sélectionnés.
*
*/
void selection_perso(int joueur_courant){
	
	
	//***************/V1.2/*****************/
	int x = 0,y = 0;
	char choix;
	
	do
	{
		clear();
		printw("Select Perso: ");
				if(joueur_courant==1)attron(COLOR_PAIR(2));//Gestion de couleurs
				if(joueur_courant==2)attron(COLOR_PAIR(3));
				if(joueur_courant==3)attron(COLOR_PAIR(4));
				if(joueur_courant==4)attron(COLOR_PAIR(5));
				if(joueur_courant==5)attron(COLOR_PAIR(6));	
		printw("%s\n",player[joueur_courant].name);	
				if(joueur_courant==1)attroff(COLOR_PAIR(2));//gestion de couleurs
				if(joueur_courant==2)attroff(COLOR_PAIR(3));
				if(joueur_courant==3)attroff(COLOR_PAIR(4));
				if(joueur_courant==4)attroff(COLOR_PAIR(5));
				if(joueur_courant==5)attroff(COLOR_PAIR(6));
	afficher_plateau_orientation(joueur_courant);
		//printw("\n\n%i - %i",x, y);
		if(Plateau[x][y].camp>0  && (Plateau[x][y].type != TRAP_UNIT || Plateau[x][y].camp == joueur_courant)){afficher_infos_persos(Plateau[x][y]);}
		mvaddch(y+2,x+3,curseur);
		refresh();
		choix = getch();
		if(choix == 68)
		{
			if(x <= 0) x = TAILLE_MATRICE;
			x--;
		}
		if(choix == 67)
		{
			if(x >= TAILLE_MATRICE-1) x = -1;
			x++;
		}
		if(choix == 65)
		{
			if(y <= 0) y = TAILLE_MATRICE;
			y--;
		}
		if(choix == 66)
		{
			if(y >= TAILLE_MATRICE-1) y = -1;
			y++;
		}
		/*move(50,10);
		printw("perso: %i\njoueur: %i", Plateau[x][y].camp, joueur);
		getch();*/
		
	}while(choix != '\n' || (Plateau[x][y].camp != joueur_courant || Plateau[x][y].type == TRAP_UNIT));
	selected_character = Plateau[x][y];
	Plateau[x][y]= case_terrain;
	//***************/V1.2/*****************/Fin
	
	/*
    int i,j,nb_perso=0,choix,choix_fait=1;
    t_character Valid_chars[MaxTab];
    init_char_table(Valid_chars);
    t_character perso_selectionne;
    vider_buffer();
    printw("Quel personnage voulez-vous selectionner ? \n");
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].camp==joueur && strcmp(Plateau[i][j].name, "Trap") != 0)
            {
                Valid_chars[nb_perso]=Plateau[i][j];
                nb_perso++;
            }
        }
    }
	printw("\n");
    for(i=0;i<nb_perso;i++){
        printw("   %i - (%i,%i) %s HP: %i\n",i+1,Valid_chars[i].position.X,Valid_chars[i].position.Y,Valid_chars[i].name,Valid_chars[i].status.HP);
    }
	
    while(choix_fait){
        scanf("%i",&choix);
        if(choix<=nb_perso && choix > 0)
        {
            perso_selectionne=Valid_chars[choix-1];
            choix_fait=0;
        }else{printw("Numero incorrect");}
    }
    selected_character=perso_selectionne; //selected_character est la variable globale correspondant au perso sélectionné*/
}

/**
* \fn void passer_tour()
* \brief Fonction qui passe le tour du joueur actif
*
*
*/
void passer_tour(){//Permet au joueur de passer son tour

}

/**
* \fn void suicide(int joueur_courant)
* \brief Fonction permetant au joueur courant d'abandonner la partie
*
*/
void suicide(int joueur_courant){// Permet au joueur de quitter la partie.
    int i,j;
    for(i=0;i<TAILLE_MATRICE;i++){
        for(j=0;j<TAILLE_MATRICE;j++){
            if(Plateau[i][j].camp==joueur_courant){
            Plateau[i][j]=case_terrain;
            }
        }
    }
}


/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/

t_skill select_skill_IA(){
	 int choix=1;
        vider_buffer();

        afficher_skill(1,selected_character.skill.a);
        afficher_skill(2,selected_character.skill.b);
        afficher_skill(3,selected_character.skill.c);
        afficher_skill(4,selected_character.skill.d);
		afficher_skill(5,selected_character.skill.e);
        afficher_skill(6,selected_character.skill.wait);

        switch(choix)
        {   case 1:  printw("\n");return selected_character.skill.a;break;
            case 2:  printw("\n");return selected_character.skill.b;break;
            case 3:  printw("\n");return selected_character.skill.c;break;
            case 4:  printw("\n");return selected_character.skill.d;break;
            case 5:  printw("\n");return selected_character.skill.e;break;
            case 6:  printw("\n");return selected_character.skill.wait;break;
        }

        return skill_empty;
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
t_skill select_skill(){//selectionne le skill que le personange courant effectuera, l'execution du skill est la fonction Skill.
        
	//***************/V1.2/*****************/
        int choix, indice_curseur = 1, nb_choix_max = 6;
        
        do
        {
        clear();
        printw("Quel compétence voulez-vous utiliser ? \n\n");
            afficher_skill(1,selected_character.skill.a);
            afficher_skill(2,selected_character.skill.b);
            afficher_skill(3,selected_character.skill.c);
            afficher_skill(4,selected_character.skill.d);
            afficher_skill(5,selected_character.skill.e);
            afficher_skill(6,selected_character.skill.wait);
            mvaddch((indice_curseur+1),1, '-');
            mvaddch((indice_curseur+1),2, '>');
            refresh();
            choix = getch();
			if(choix == 65)
			{
				if(indice_curseur <= 1) indice_curseur = nb_choix_max+1; //nb_choix_max = 5 = le choix max
				indice_curseur--;
			}
			if(choix == 66)
			{
				if(indice_curseur >= nb_choix_max) indice_curseur = 0;
				indice_curseur++;
			}
            //choix = getch() -48;
        }
        while ((choix !='\n') );

            switch(indice_curseur)
            {   case 1:  printw("\n");return selected_character.skill.a;break;
                case 2:  printw("\n");return selected_character.skill.b;break;
                case 3:  printw("\n");return selected_character.skill.c;break;
                case 4:  printw("\n");return selected_character.skill.d;break;
                case 5:  printw("\n");return selected_character.skill.e;break;
                case 6:  printw("\n");return selected_character.skill.wait;break;
				default: printw("Erreur: votre choix doit être compris entre 1 et 6\n");
            }
        printw("\n");
	//***************/V1.2/*****************/Fin
        /*
        int choix;
        vider_buffer();
        printw("Quel compétence voulez-vous utiliser ? \n\n");
        do
        {
            afficher_skill(1,selected_character.skill.a);
            afficher_skill(2,selected_character.skill.b);
            afficher_skill(3,selected_character.skill.c);
            afficher_skill(4,selected_character.skill.d);
            afficher_skill(5,selected_character.skill.e);
            afficher_skill(6,selected_character.skill.wait);
            scanf("%d",&choix);

            switch(choix)
            {   case 1:  printw("\n");return selected_character.skill.a;break;
                case 2:  printw("\n");return selected_character.skill.b;break;
                case 3:  printw("\n");return selected_character.skill.c;break;
                case 4:  printw("\n");return selected_character.skill.d;break;
                case 5:  printw("\n");return selected_character.skill.e;break;
                case 6:  printw("\n");return selected_character.skill.wait;break;
  default: printw("Erreur: votre choix doit être compris entre 1 et 6\n");
            }
        }
        while ((choix<1) ||(choix>6) );
        printw("\n");*/
        return skill_empty;
}


/**
* \fn t_targetOrientation get_target_orientation (t_character perso, t_coord cible )
* \brief Fonction déterminant quelle est l'orientation de la cible par rapport au joueur.
*
*/
t_targetOrientation get_target_orientation (t_character perso, t_coord cible ){
    int xperso,yperso,xenemy,yenemy,oenemy;

    xperso=perso.position.X;
    yperso=perso.position.Y;
    xenemy=cible.X;
    yenemy=cible.Y;
    oenemy=Plateau[xenemy][yenemy].orientation;

    if((xenemy-yenemy>=xperso-yperso)&&(xenemy+yenemy<xperso+yperso))//Vérifie si l'énemi est au Nord
    {
        switch(oenemy)
        {
            case up: return back ; break;
            case right: if(xenemy-yenemy==xperso-yperso){return front;}else return side ; break;
            case down: return front ;break;
            case left: if(xenemy-yenemy==xperso-yperso){return back;}else return side ; break;
        }
    }
    else if((xenemy-yenemy>xperso-yperso)&&(xenemy+yenemy>=xperso+yperso))//Vérifie si l'énemi est à l'Est
    {
        switch(oenemy)
        {
            case up: if(xenemy+yenemy==xperso+yperso){return back;}else return side ; break;
            case right: return back ; break;
            case down: if(xenemy+yenemy==xperso+yperso){return front;}else return side ;break;
            case left: return front ; break;
        }
    }
    else if((xenemy-yenemy<=xperso-yperso)&&(xenemy+yenemy>xperso+yperso))//Vérifie si l'énemi est au Sud
    {
        switch(oenemy)
        {
            case up: return front ; break;
            case right: if(xenemy+yenemy==xperso+yperso){return back;}else return side ; break;
            case down: return back ;break;
            case left: if(xenemy+yenemy==xperso+yperso){return front;}else return side ; break;
        }
    }
    else if((xenemy-yenemy<xperso-yperso)&&(xenemy+yenemy<=xperso+yperso))//Vérifie si l'énemi est à l'Ouest
    {
        switch(oenemy)
        {
            case up: if(xenemy+yenemy==xperso+yperso){return front;}else return side ; break;
            case right: return front ; break;
            case down: if(xenemy+yenemy==xperso+yperso){return back;}else return side ;break;
            case left: return back ; break;
        }
    }
    return EXIT_FAILURE;
}



/**
* \fn void appliquer_action(t_character lanceur, t_coord cible, t_skill action)
* \brief Fonction appliquant le skill du personnage lanceur à la case cible.
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void appliquer_action(t_character lanceur, t_coord cible, t_skill action){
    int total_dmg;
    t_targetOrientation targetOrientation= get_target_orientation(lanceur,cible);
    int coefOrientation;
    //typedef struct { char name[20] ; int range ; t_skilltype type ; int damage_coeff ;} t_skill;
    //typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
    //typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
    //typedef enum {EMPTY,ATK,MATK}t_skilltype;
    if(targetOrientation==0) coefOrientation=1;
    if(targetOrientation==1) coefOrientation=1;
    if(targetOrientation==2) coefOrientation=2;

    if (action.type == MATK){
        
        printw("MATK : %d,%d\n", cible.X, cible.Y);
        total_dmg=action.damage_coeff*lanceur.stats.MATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.MDEF/10;//*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
                total_dmg*=coefOrientation;
        }
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    else if (action.type == ATK){
        printw("ATK : %d,%d\n", cible.X, cible.Y);
        total_dmg=action.damage_coeff*lanceur.stats.ATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.DEF/10;//*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
                total_dmg*=coefOrientation;
        }
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    else if (action.type == EMPTY){
		printw("NOTHING : %d,%d\n", cible.X, cible.Y);
    }

    else if(action.type == TRAP )
    {
		printw("TRAP : %d,%d\n", cible.X, cible.Y);
        Plateau[cible.X][cible.Y] = default_trap;
        Plateau[cible.X][cible.Y].type=TRAP_UNIT;
        Plateau[cible.X][cible.Y].camp = lanceur.camp;
        Plateau[cible.X][cible.Y].position.X = cible.X;
        Plateau[cible.X][cible.Y].position.Y = cible.Y;
        //printw("%s, %i",Plateau[cible.X][cible.Y].name, Plateau[cible.X][cible.Y].camp);
    }

    if (Plateau[cible.X][cible.Y].status.HP< 0) Plateau[cible.X][cible.Y].status.HP=0;
    if (Plateau[cible.X][cible.Y].status.HP>Plateau[cible.X][cible.Y].status.Max_HP) Plateau[cible.X][cible.Y].status.HP=Plateau[cible.X][cible.Y].status.Max_HP;
    if (Plateau[cible.X][cible.Y].status.HP<= 0) Plateau[cible.X][cible.Y]=case_terrain;
}



/**
* \fn void vider_buffer(void)
* \brief Vide le tampon de saisie clavier.
* 
*
*/
void vider_buffer(void)
{
  //int c;
  //while((c=getchar()) != EOF && c != '\n');
 
}

/**
* \fn void orienter_perso_numpad(int joueur_courant)
* \brief Propose une liste numérique des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void orienter_perso_numpad(int joueur_courant){
    char input;

    char curs = 'O';
    t_orientation tampon_orientation;
    //viderBuffer();
    
    while(input != '\n')
    {
        clear();
		printw("Choisissez l'orientation : \n");
		afficher_plateau_orientation(joueur_courant);
		printw("Valider =\"Entrée\"");
		mvaddch(selected_character.position.Y+2, selected_character.position.X+3, curs);
            refresh();
            input = getch();
			if(input == 65)
			{
				curs = '^';
			}
			if(input == 66)
			{
				curs = 'v';
			}
			if(input == 67)
			{
				curs = '>';
			}
			if(input == 68)
			{
				curs = '<';
			}
        //while (getchar() != '\n');
    }
        switch(curs)
        {
            case '^':tampon_orientation=up;break;
            case '>':tampon_orientation=right;break;
            case 'v':tampon_orientation=down;break;
            case '<':tampon_orientation=left;break;
            default:printw("Orientation incorrecte");
        }
    
    Plateau[selected_character.position.X][selected_character.position.Y].orientation=tampon_orientation;
    selected_character.orientation=tampon_orientation;
	//***************/V1.2/*****************/Fin
	/*
    int choix=1;
    char input;
    t_orientation tampon_orientation;
    printw("Choisissez l'orientation : \n");
    vider_buffer();
    while(choix)
    {
        
        printw("\n  8 - Nord\n  6 - Est\n  5 - Sud\n  4 - Ouest\n");
        input = getchar();
        while (getchar() != '\n');
        switch(input)
        {
            case '8':tampon_orientation=up;choix=0;break;
            case '6':tampon_orientation=right;choix=0;break;
            case '5':tampon_orientation=down;choix=0;break;
            case '4':tampon_orientation=left;choix=0;break;
            default:printw("Orientation incorrecte");
        }
    }
    
    Plateau[selected_character.position.X][selected_character.position.Y].orientation=tampon_orientation;
    selected_character.orientation=tampon_orientation;*/
    
}


/**
* \fn void orienter_perso(t_character perso)
* \brief Propose une liste des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void orienter_perso(){
    int choix=1;
    char input;
    t_orientation buffer;
    printw("Choisissez l'orientation : \n");
    vider_buffer();
    while(choix)
    {
        
        printw("\n  z - Nord\n  d - Est\n  s - Sud\n  q - Ouest\n");
        input = getchar();
        while (getchar() != '\n');
        switch(input)
        {
            case 'z':buffer=up;choix=0;break;
            case 'd':buffer=right;choix=0;break;
            case 's':buffer=down;choix=0;break;
            case 'q':buffer=left;choix=0;break;
            default:printw("Orientation incorrecte");
        }
    }
    
    Plateau[selected_character.position.X][selected_character.position.Y].orientation=buffer;
    selected_character.orientation=buffer;
    
}


/**
* \fn void joueur_liste_suivant(int nb_joueurs, int* joueur_courant)
* \brief Prend en paramètre le nombre de joueurs et incrémente le numéro de joueur de façon à ne pas dépasser le nombre de joueurs.
*
*/
void joueur_liste_suivant(int nb_joueurs, int* joueur_courant)
{
    if (*joueur_courant==nb_joueurs) *joueur_courant=1;
        else (*joueur_courant)++;

}


/**
* \fn int are_my_mates_alive(joueur_courant)
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
int are_my_mates_alive(int joueur_courant){
    int nb_perso=0,i,j;
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].camp==joueur_courant && Plateau[i][j].type!=TRAP_UNIT)
            {
                nb_perso++;
            }
        }
    }
    return nb_perso;
}

int perso_oriente_a_droite()
{
	int i,j;
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].orientation==right)
            {
                return 1;
            }
        }
    }
    return 0;
}

/**
* \fn int life_check(int joueur_courant)
* \brief Fonction qui vérifie si un joueur a encore des personnages vivants sur le terrain.
*    Appelle are_my_mates_alive(int joueur_courant) pour déterminer le joueur est encore en jeu.
*    Renvoie 1 si le joueur a encore des personnages en vie, sinon 0.
*/
int life_check(int joueur_courant){
          
    if (player[joueur_courant].alive==0 || are_my_mates_alive(joueur_courant)<1 ) 
    {
		player[joueur_courant].alive=0;
        //if (joueur_courant!=sauvage) printw("%s est mort.\n",player[joueur_courant].name);
        return 0;
    }else {
        return 1;
    }
}


/**
* \fn void players_life_check()
* \brief Actualise le fait que les joueurs soient vivants ou non.
*    
*    
*/
void players_life_check()
{	
	int i_joueur,i,j;
	for (i_joueur=1;i_joueur<=MaxTab;i_joueur++)
	{ 
		player[i_joueur].alive=0;

	}
					
	for(i=0;i<TAILLE_MATRICE;i++)
	{
		for(j=0;j<TAILLE_MATRICE;j++)
		{
			if( ( Plateau[i][j].camp>=1 ) && ( Plateau[i][j].type != TRAP_UNIT ) )
			{
				player[Plateau[i][j].camp].alive=1;
			}
		}
	}

    
}
/**
* \fn void character_hp_list()
* \brief affiche une liste avec les persos et leurs points de vie.
*    
*    
*/
void character_hp_list()
{	
	int i,j;
					
	for(i=0;i<TAILLE_MATRICE;i++)
	{
		for(j=0;j<TAILLE_MATRICE;j++)
		{
			if(Plateau[i][j].camp>=1)
			{
				printw("Joueur %i (%i,%i) %s %i/%i HP\n",Plateau[i][j].camp,Plateau[i][j].position.X,Plateau[i][j].position.Y,Plateau[i][j].name,Plateau[i][j].status.HP,Plateau[i][j].status.Max_HP);
			}
		}
	}

    
}

/**
* \fn void tour(int joueur_courant)
* \brief Fonction de déroulement d'un tour pour le joueur entré en paramètre.
*
*
*/
void tour(int joueur_courant, int* nbAtkValid)
{
	
    int choix, nb_actions_faites = 0, deplacement_fait, indice_curseur =1, nb_choix_max = 5, coord_curs;
    t_skill skill_selected;
    
        do                                            //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
        {   
			clear();
			afficher_plateau_orientation(joueur_courant);
			printw("\nTour de ");
				if(joueur_courant==1)attron(COLOR_PAIR(2));//Gestion de couleurs
				if(joueur_courant==2)attron(COLOR_PAIR(3));
				if(joueur_courant==3)attron(COLOR_PAIR(4));
				if(joueur_courant==4)attron(COLOR_PAIR(5));
				if(joueur_courant==5)attron(COLOR_PAIR(6));	
		printw("%s\n",player[joueur_courant].name);	
				if(joueur_courant==1)attroff(COLOR_PAIR(2));//gestion de couleurs
				if(joueur_courant==2)attroff(COLOR_PAIR(3));
				if(joueur_courant==3)attroff(COLOR_PAIR(4));
				if(joueur_courant==4)attroff(COLOR_PAIR(5));
				if(joueur_courant==5)attroff(COLOR_PAIR(6));
            printw("    Selectionner Perso\n");
            printw("    Passer tour\n");
            printw("    suicide\n");
            printw("    Sauvegarder\n");
            printw("    Charger\n");
            mvaddch((indice_curseur+TAILLE_MATRICE+2),2, '*');
            switch(indice_curseur)
            {   case 1: coord_curs = 23; break;
                case 2: coord_curs = 16; break;
                case 3: coord_curs = 12; break;
                case 4: coord_curs = 16; break;
                case 5: coord_curs = 12; break;
                default: printw("Erreur: votre choix doit être compris entre 1 et 5\n");
            }
            mvaddch((indice_curseur+TAILLE_MATRICE+2),coord_curs, '*');
            refresh();
            choix = getch();
			if(choix == 65)
			{
				if(indice_curseur <= 1) indice_curseur = nb_choix_max+1; //nb_choix_max = 5 = le choix max
				indice_curseur--;
			}
			if(choix == 66)
			{
				if(indice_curseur >= nb_choix_max) indice_curseur = 0;
				indice_curseur++;
			}
        }
        while ((choix != '\n') );
            switch(indice_curseur)
            {   case 1: printw("\n");selection_perso(joueur_courant); break;
                case 2: printw("\n");passer_tour(); break;
                case 3: printw("\n");suicide(joueur_courant);life_check(joueur_courant); break;//Abandon
                case 4: printw("\n");Sauvegarder(joueur_courant);break;
                case 5: printw("\n");Charger(joueur_courant);break;
                default: printw("Erreur: votre choix doit être compris entre 1 et 5\n");
			}
	/*
    vider_buffer();
    int choix, nb_actions_faites = 0, deplacement_fait=0;
    t_skill skill_selected;
    
        do                                            //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
        {   
			printw("\nTour de %s : \n",player[joueur_courant].name);
            printw("   1 - Selectionner Perso\n");
            printw("   2 - Passer tour\n");
            printw("   3 - suicide\n");
            scanf("%d",&choix);

            switch(choix)
            {   case 1: printw("\n");selection_perso(joueur_courant); break;
                case 2: printw("\n");passer_tour(); break;
                case 3: printw("\n");suicide(joueur_courant);players_life_check(); break;//Abandon
                default: printw("Erreur: votre choix doit être compris entre 1 et 3\n");
            }
        }
        while ((choix<1) ||(choix>3) );*/

	
		
    if (indice_curseur==1){   // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné
		
        //skill_selected.type = ATK;	WTF Nigga ?
        while(nb_actions_faites < selected_character.nb_action_tour /* && skill_selected.type != EMPTY*/)
        {
            if(deplacement_fait != 1)
            {
                deplacements_valides();
                //choix_deplacement_humain(joueur_courant);
                deplacer_perso(choix_deplacement_humain(joueur_courant));
                deplacement_fait = 1;
            }
            skill_selected = select_skill();//selectionne l’action que le joueur souhaite effectuer (exple: selection_perso / passer_tour…) retourne le n° de l’action à effectuer
            //tampon_skill = skill_selected;
            if(skill_selected.type != EMPTY)
            {
                viser_case_valide(skill_selected, nbAtkValid);
                appliquer_action(selected_character, choix_cible_humain(skill_selected,joueur_courant), skill_selected);
            }else 
            {
				appliquer_action(selected_character, selected_character.position, skill_selected);
			}
            nb_actions_faites++;
            orienter_perso_numpad(joueur_courant);
            //OrienterPerso();
        }

       

    }

}



/**
* \fn int all_dead_but_one(void)
* \brief Fonction déterminant si un seul joueur est vivant.
*    Appellé par la fonction principale pour déterminer si c'est la fin de la partie.
*    Renvoie 0 si plus d'un joueur survit, sinon renvoie 1.
*/
int all_dead_but_one(int nb_joueurs){
    compteur_joueurs_vivants=0;
    int i;
    for (i=2;i<=nb_joueurs;i++)
	{
		if (player[i].alive) compteur_joueurs_vivants++;
	}
	if (compteur_joueurs_vivants != 1)return 0;
    return 1;
}



/**
* \fn void afficher_plateau_orientation(int joueur_courant)
* \brief Affiche le plateau avec les caractères correspondants à l'orientation
*
*/
void afficher_plateau_orientation(int joueur_courant){
	//***************/V1.2/*****************/
	int i,j;
    for(j=-1;j<TAILLE_MATRICE;j++)
    {
			attron(COLOR_PAIR(1));//gestion de couleur de fond du plateau
			attron(A_BOLD);
	if (j!=-1) {printw("\n%i ",j);} else printw("  ");
	printw(" ");
        for(i=0;i<TAILLE_MATRICE;i++)
        {
			
			attron(COLOR_PAIR(1));//gestion de couleur de fond du plateau
			if (j==-1)
			{
				 printw("%i",i);
			/*}else if ( Plateau[i][j].camp == joueur )
            {
				if(Plateau[i][j].type == TRAP_UNIT)
					printw("%c",'X');
				else
					printw("%c",'O');*/
            }else if((Plateau[i][j].camp>0 && Plateau[i][j].type != TRAP_UNIT) || Plateau[i][j].camp == joueur_courant)
            {
				if(Plateau[i][j].camp==1)attron(COLOR_PAIR(2));//Gestion de couleurs
				if(Plateau[i][j].camp==2)attron(COLOR_PAIR(3));
				if(Plateau[i][j].camp==3)attron(COLOR_PAIR(4));
				if(Plateau[i][j].camp==4)attron(COLOR_PAIR(5));
				if(Plateau[i][j].camp==5)attron(COLOR_PAIR(6));
				if(Plateau[i][j].camp==joueur_courant) attron(A_BLINK);
				
				if(Plateau[i][j].type == TRAP_UNIT)
				{
					printw("%c",'X');
				}else
				{
					if ( Plateau[i][j].orientation == up ) printw("^");
					if ( Plateau[i][j].orientation == left ) printw("<");
					if ( Plateau[i][j].orientation == right ) printw(">");
					if ( Plateau[i][j].orientation == down ) printw("v");
				}
				
				
				if(Plateau[i][j].camp == joueur_courant) attroff(A_BLINK);
				if(Plateau[i][j].camp==1)attroff(COLOR_PAIR(2));//gestion de couleurs
				if(Plateau[i][j].camp==2)attroff(COLOR_PAIR(3));
				if(Plateau[i][j].camp==3)attroff(COLOR_PAIR(4));
				if(Plateau[i][j].camp==4)attroff(COLOR_PAIR(5));
				if(Plateau[i][j].camp==5)attroff(COLOR_PAIR(6));
            }else if ( Plateau[i][j].camp == -1 )
            {
                printw("%c",'#');
            }else if ( Plateau[i][j].camp == 0 || Plateau[i][j].type == TRAP_UNIT)
            {
                printw(" ");
            }
            
			attroff(COLOR_PAIR(1));//gestion de couleur de fond
        }
        /*if (j!=-1)*/ printw("|");
    }
	printw("\n");
	refresh();
	//***************/V1.2/*****************/Fin
	
	/*
    int i,j;
    for(j=-1;j<TAILLE_MATRICE;j++)
    {
        for(i=0;i<TAILLE_MATRICE;i++)
        {
			//printf("\x1B[1;47m");
			//printw("\x1B[1m");
			//printw("\x1B[5m");
			
			if (j==-1)
			{
				 printw("%i",i);
			}else if ( Plateau[i][j].camp == joueur_courant )
            {   
				if(strcmp(Plateau[i][j].name, "Trap") == 0)
                {
					//if(Plateau[i][j].camp==1) printf("%s",KRED);
					//if(Plateau[i][j].camp==3) printf("%s",KBLU);
					//if(Plateau[i][j].camp==4) printf("%s",KMAG);
                    printw("X");
                    //printf("%s",KNRM);
                }else
                {
					//printf("\x1B[5m");
					//if(Plateau[i][j].camp==1) printf("%s",KRED);
					//if(Plateau[i][j].camp==3) printf("%s",KBLU);
					//if(Plateau[i][j].camp==4) printf("%s",KMAG);
					if ( Plateau[i][j].orientation == up ) printw("^");
					if ( Plateau[i][j].orientation == left ) printw("<");
					if ( Plateau[i][j].orientation == right ) printw(">");
					if ( Plateau[i][j].orientation == down ) printw("v");
					//printf("%s",KNRM);
				}
            }else if(Plateau[i][j].camp>0)
            {	
				if(strcmp(Plateau[i][j].name, "Trap") == 0)
                {
					printw(" ");
				}else
				{
					//if(Plateau[i][j].camp==1) printf("%s",KRED);
					//if(Plateau[i][j].camp==3) printw("%s",KBLU);
					//if(Plateau[i][j].camp==4) printf("%s",KMAG);
					if ( Plateau[i][j].orientation == up ) printw("^");
					if ( Plateau[i][j].orientation == left ) printw("<");
					if ( Plateau[i][j].orientation == right ) printw(">");
					if ( Plateau[i][j].orientation == down ) printw("v");
					//printf("%s",KNRM);
				}
            }else if ( Plateau[i][j].camp == -1 )
            {
                printw("%c",'#');
            }else if ( Plateau[i][j].camp == 0 )
            {
                printw(" ");
            }
        //printf("%s",KNRM);
        }if (j!=-1) 
        {
			printw("  %i",j);
			printw("\n");
		}else printw("\n");
    }*/

}

/**
* \fn void creer_perso_rapide(char nom[MaxTab], t_camp camp,int x, int y)
* \brief Création brève de perso.
* 
*
*/
void creer_perso_rapide(t_camp camp,int x, int y)
{
	char chaine_tampon[MaxString];
	chaine_tampon[0]=0;
	generation_nom_personnage(chaine_tampon);
    Plateau[x][y]=character_default;
    if (camp>terrain) strcpy(Plateau[x][y].name, chaine_tampon);else strcpy(Plateau[x][y].name, "Mauvaise fonction de creation");
    Plateau[x][y].camp=camp;
    Plateau[x][y].position.X=x;
    Plateau[x][y].position.Y=y;
    
    Plateau[x][y].skill.b = (t_skill){"Placer Piege", 3, TRAP, 10};
}

/**
* \fn void creer_terrain_rapide(t_camp camp,int x, int y)
* \brief Création brève de terrain ou obstacle, possible implémentation de génération aléatoire en cas d'obstacle.
* 
*
*/
void creer_terrain_rapide(t_camp camp,int x, int y)
{
	char chaine_tampon[MaxString];
	chaine_tampon[0]=0;

    
    if (camp==obstacle)
    { 
		Plateau[x][y]=case_obstacle;
		strcat(chaine_tampon, "Obstacle ");
		strcat(chaine_tampon, particule_generateur_nom[generation_nombre_aleatoire(17)]);
		strcpy(Plateau[x][y].name, chaine_tampon);
	} else if (camp==terrain) 
	{
		Plateau[x][y]=case_terrain;
		strcat(chaine_tampon, "terrain");
		strcat(chaine_tampon, particule_generateur_nom[generation_nombre_aleatoire(17)]);
		strcpy(Plateau[x][y].name, chaine_tampon);
	}else
	{
		strcpy(Plateau[x][y].name, "Mauvaise fonction de creation");
	}
    Plateau[x][y].camp=camp;
    Plateau[x][y].position.X=x;
    Plateau[x][y].position.Y=y;
}


void generation_nom_personnage(char * nom)
{
	int i;
	char *random_name = NULL;
	random_name=malloc(sizeof(char) * (100));
	
	random_name[0]=0;
	for(i=generation_nombre_aleatoire(1)+1;i>0;i--)
	{
	strcat(random_name, particule_generateur_nom[generation_nombre_aleatoire(17)]);
	strcat(random_name, " ");
	}
	random_name[0]=toupper(random_name[0]);
	
	strcpy(nom,random_name);
	free(random_name);
}
void edit_stats( t_character perso,int HP , int Max_HP , int MP , int Max_MP,int ATK , int MATK , int DEF , int MDEF , int MVT)
{
	int x_buffer=perso.position.X,y_buffer=perso.position.Y;
	
	Plateau[x_buffer][y_buffer].status.HP=HP;//Default=100
	Plateau[x_buffer][y_buffer].status.Max_HP=Max_HP;//Default=100
	Plateau[x_buffer][y_buffer].status.MP=MP;//Default=10
	Plateau[x_buffer][y_buffer].status.Max_MP=Max_MP;//Default=10
	
	Plateau[x_buffer][y_buffer].stats.ATK=ATK;//Default=10
	Plateau[x_buffer][y_buffer].stats.MATK=MATK;//Default=10
	Plateau[x_buffer][y_buffer].stats.DEF=DEF;//Default=10
	Plateau[x_buffer][y_buffer].stats.MDEF=MDEF;//Default=10
	Plateau[x_buffer][y_buffer].stats.MVT=MVT;//Default=2

}		
		//Pour déclarer facilement : int HP=000,Max_HP=000,MP=000,Max_MP=000,ATK=000,MATK=000,DEF=000,MDEF=000,MVT=000;
		//edit_stats(HP,Max_HP,MP,Max_MP,ATK,MATK,DEF,MDEF,MVT);
		
void spawn_sauvage()
{
	player[1].alive=1;
	int x_buffer,y_buffer;
	do{
		x_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
		y_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
	}while(Plateau[x_buffer][y_buffer].camp !=0);
	printw("Coordonnées de creation du personnage chaotique : %i , %i. Camp case : %i",x_buffer,y_buffer,Plateau[x_buffer][y_buffer].camp);
	creer_perso_rapide(sauvage,x_buffer,y_buffer);
	edit_stats(Plateau[x_buffer][y_buffer],20,20,10,10,30,30,10,20,5);
	
}

void spawn_character(t_camp camp_nouveau_perso)
{
	player[camp_nouveau_perso].alive=1;
	int x_buffer,y_buffer;
	do{
		x_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
		y_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
	}while(Plateau[x_buffer][y_buffer].camp !=0);
	printw("Coordonnées de creation du personnage de %s : %i , %i.",player[camp_nouveau_perso].name,x_buffer,y_buffer);
	creer_perso_rapide(camp_nouveau_perso,x_buffer,y_buffer);
	//edit_stats(Plateau[x_buffer][y_buffer],20,20,10,10,30,30,10,20,5);
	
}

void tour_IA(int joueur_courant, int* nbAtkValid)
{
    int nb_actions_faites, i;
    int nb_perso_vivants=calcul_persos_IA(joueur_courant);
    //printw("nb_persos_vivants:%i\n",nb_perso_vivants);
    t_skill skill_selected;
    for (i=0; i<nb_perso_vivants ; i++ )
    {	
		selected_character=Plateau[Valid_chars_IA[i].position.X][Valid_chars_IA[i].position.Y];
        if (selected_character.camp==joueur_courant) //Vérifie que la case appartient à l'IA, en cas de mort d'un perso durant le tour d'un autre perso de l'IA, sinon contrôle d'une case terrain ou obstacle, ce qui serait gênant.
        {		
                for(nb_actions_faites=0;nb_actions_faites < selected_character.nb_action_tour;nb_actions_faites++)
            {

                deplacements_valides();

				
                deplacer_perso(choix_deplacement_IA());

                skill_selected = selected_character.skill.a;
                
                if(skill_selected.type != EMPTY)
                {

					
                    viser_case_valide(skill_selected, nbAtkValid);
                    appliquer_action(selected_character, choix_cible_IA(skill_selected,nbAtkValid), skill_selected);
                }else 
                {
					//printw("Skill EMPTY type.\n");
                    appliquer_action(selected_character, selected_character.position, skill_selected);
                }

            }
        }
    }

	//scanf("%d", &i);

       


}

int calcul_persos_IA(int joueur_courant){
    int i,j,nb_perso=0;
    init_char_table(Valid_chars_IA);
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].camp==joueur_courant)
            {
                Valid_chars_IA[nb_perso]=Plateau[i][j];
                nb_perso++;
            }
        }
    }
    return nb_perso;
}

void saisie_nombre_joueurs(int* nb_joueurs )
{
	*nb_joueurs=4;
}

/**
* \fn int main()
* \brief Fonction principale
* Fonction principale qui permet de jouer en mode Kill'em'all.
*
*/
int main(){
	int i;
	int nb_joueurs, joueur_courant;
	int nbAtkValid;
	saisie_nombre_joueurs(&nb_joueurs);
	srand((long)time(NULL));
	
	
	initscr();
	raw();
	curs_set(0);
	
	
    start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);

		/*if(has_colors() == FALSE)
	{	endwin();
		printw("Your terminal does not support color\n");
		refresh();
		exit(1);
	}
	getch();
	start_color();
	init_color(COLOR_RED, 700, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));*/
	

	
	
	
	strcpy(player[1].name,"la nature");
	strcpy(player[2].name,"Baptiste");
	strcpy(player[3].name,"Yann");
	strcpy(player[4].name,"Arthur");
	
	creer_terrain_rapide(obstacle,3,1);
	
	spawn_sauvage();
	for (i=2;i<nb_joueurs;i++)
	{
		spawn_character(i);
		spawn_character(i);
	}
	
	joueur_courant=2;
	
	
        
    //spawn_sauvage();
    
	players_life_check();
	for (i=1;i<=MaxTab;i++)
		{
			if(i==1)printw("%s",KRED);
			if(i==3)printw("%s",KBLU);
			if(i==4)printw("%s",KMAG);
			if(player[i].alive==1) printw("\nJoueur %i %s.\n",i,player[i].name);
			printw("%s",KNRM);
		}
		
    
    while(!all_dead_but_one(nb_joueurs)){
		
        if(player[joueur_courant].alive != 0 )
        {   
			
			
            compteur_tour++;
            //if (generation_nombre_aleatoire(4)==1) spawn_sauvage();
            
            afficher_plateau_orientation(joueur_courant);
            character_hp_list();
            if (joueur_courant==sauvage) 
            {
                tour_IA(joueur_courant,&nbAtkValid);
                
            }else tour(joueur_courant,&nbAtkValid);
            
            
        }
        
        
        players_life_check();
        joueur_liste_suivant(nb_joueurs, &joueur_courant);
    }
    clear();
	afficher_plateau_orientation(joueur_courant);


	for (i=1;i<=nb_joueurs;i++)
	{
		if (player[i].alive==1) printw("Victoire de %s !\n",player[i].name);
	}
        
    /*    
    for (i=1;i<=nb_joueurs;i++)
	{
		if (player[i].alive==1) printw("Victoire de %s !\n",player[i].name);
	}
	int statistiques_generation_random[5];
	for (i=0;i<5;i++)
	{
		statistiques_generation_random[i]=0;
	}
	for (i=0;i<2000;i++)
	{
		statistiques_generation_random[generation_nombre_aleatoire(4)]++;
	}
	for (i=0;i<5;i++)
	{
		printw("occurences de %i : %i\n",i,statistiques_generation_random[i]);
	}
	*/
	getch();
	endwin();
	
    return EXIT_SUCCESS;
}
