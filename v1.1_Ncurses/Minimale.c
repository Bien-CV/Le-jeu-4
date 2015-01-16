/**
* \file Minimale.c
* \brief Code principal
* \author Arthur LEMEE, Yann GUENVER, Baptiste CANOVAS-VIRLY.
* \version 1.1
* \date 06 Janvier 2015
*
* programme de jeu: Tactics Arena
*
*/


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <ncurses.h> // librairie graphique


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
	

void players_life_check();
t_character Plateau[TAILLE_MATRICE][TAILLE_MATRICE]; 	//matrice du plateau de jeu
t_coord depValid[TAILLE_MATRICE*TAILLE_MATRICE][3];


t_player player[MaxTab];	//la liste des joueurs


int indiceTabDepValid;



/**
 * \fn void Sauvegarder(int joueur_courant)
 * \brief Sauvegarde la partie en cours dans un fichier
 *
 */
void Sauvegarder(int joueur_courant){
	
    char NomFichier[20]="",FichierSauvegarde[20]="";//Nom du fichier entré et nom des fichiers enregistrés
    int present=1;

    FILE * fichierIndex;
    FILE * sauvegarde;
    fichierIndex=fopen("IndexSauvegarde.txt","r");//Fichier contenant des noms des sauvegardes déjà existantes
    fscanf(fichierIndex,"%s",FichierSauvegarde);
    if(!feof(fichierIndex)){//On verifie que l'index n'est pas vide
        printw("Sauvegarde deja existante :\n");

        while(!feof(fichierIndex))//Et on affiche les noms des sauvegardes déjà existantes
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
    getstr(NomFichier);//On choisie un fichier dans la liste
    while(!feof(fichierIndex))//On verifie si il est déjà dans la liste
    {
        fscanf(fichierIndex,"%s",FichierSauvegarde);
        if(!strcmp(NomFichier,FichierSauvegarde))
        {
            present=0;
        }
    }
    if(present) fprintf(fichierIndex,"%s ",NomFichier);//Si il n'est pas présent son nom est écrit dans l'index
    sauvegarde=fopen(NomFichier,"wb");
    fwrite(Plateau,TAILLE_MATRICE*TAILLE_MATRICE,sizeof(t_character),sauvegarde);
    fwrite(player,sizeof(player)/sizeof(t_player),sizeof(t_player),sauvegarde);
    fwrite(&joueur_courant,sizeof(int),1,sauvegarde);
    fclose(fichierIndex);
    fclose(sauvegarde);
}

/**
 * \fn int Charger(int joueur_courant)
 * \brief Permet de charger une partie depuis un fichier de sauvegarde
 *
 */
int Charger(int joueur_courant){
	char NomFichier[20]="",FichierSauvegarde[20]="";//Nom du fichier entré et nom des fichiers enregistrés
	FILE * fichierIndex;
	FILE * sauvegarde;
	fichierIndex=fopen("IndexSauvegarde.txt","r");//Fichier contenant des noms des sauvegardes déjà existantes
	fscanf(fichierIndex,"%s",FichierSauvegarde);
	if(!feof(fichierIndex)){//On verifie que l'index n'est pas vide
		printw("Sauvegarde deja existante :\n");
		while(!feof(fichierIndex))//Et on affiche les noms des sauvegardes déjà existantes
		{
			printw("%s\n",FichierSauvegarde);
			fscanf(fichierIndex,"%s",FichierSauvegarde);
		}
		printw("\n");
		do{
			printw("Choisir un fichier : ");
			getstr(NomFichier);//On choisie un fichier dans la liste
			sauvegarde=fopen(NomFichier,"rb");
			if(sauvegarde!=NULL){
				fread(Plateau,TAILLE_MATRICE*TAILLE_MATRICE,sizeof(t_character),sauvegarde);
				fread(player,sizeof(player)/sizeof(t_player),sizeof(t_player),sauvegarde);
				fread(&joueur_courant,sizeof(t_camp),1,sauvegarde);
			}else {printw("Fichier incorrect\n");}//On retape le nom si il est incorrect
		}while(sauvegarde==NULL);
		fclose(sauvegarde);
		afficher_plateau_orientation(joueur_courant);
	}else {
		printw("Pas de sauvegarde disponible.\n");
	}
	fclose(fichierIndex);
	return joueur_courant;
}


/**
 * \fn int generation_nombre_aleatoire(int max)
 * \brief Fonction qui renvoi un nombre aléatoire entre 0 et 'max'
 *
*/
int generation_nombre_aleatoire(int max)
{
		//entre 0 et 'max' !
		return (rand()%max+1);
}

/**
 * \fn void deplacer_perso(t_coord case_perso, t_character* selected_character)
 * \brief Déplace le personnage sur le terrain
 * Remplace la case où se trouvait le perso par une case terrain, et place le perso sur la 'case_perso'.
 * Si la 'case_perso' contenait un piège, le perso perd des HP.
 */
void deplacer_perso(t_coord case_perso, t_character* selected_character)
{
    Plateau[selected_character->position.X][selected_character->position.Y]= case_terrain ;
    if(Plateau[case_perso.X][case_perso.Y].type == TRAP_UNIT){selected_character->status.HP -= Plateau[case_perso.X][case_perso.Y].stats.ATK;}	//appliquation des dégâts du piège! par défaut, le piège enlève 20 HP!
    selected_character->position.X=case_perso.X;
    selected_character->position.Y=case_perso.Y;
    Plateau[case_perso.X][case_perso.Y]= *selected_character; //la case du terrain (case_perso) prend le personnage "selected_character"!

}

/**
 * \fn int cases_voisines_calcul(t_coord coordonnees)
 * \brief Renvoi le nombre de case voisine vide; met dans la file, la liste des coordonnées voisines accessibles
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
            file_ajouter(coord); 	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;		//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }
    }
    if(0 == Plateau[coordonnees.X-1][coordonnees.Y].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord); 	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;		//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }
    }
    if(0 == Plateau[coordonnees.X][coordonnees.Y+1].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord); 	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;		//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }
    }
    if(0 ==Plateau[coordonnees.X][coordonnees.Y-1].camp || Plateau[coordonnees.X+1][coordonnees.Y].type == TRAP_UNIT )
    {
        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord); 	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;		//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }
    }
    return(nbVois);
}


/**
 * \fn void deplacements_valides(int* nbDepValid, t_character* selected_character)
 * \brief Calcule les positions de déplacement valide, les met dans la liste
 *
 */
void deplacements_valides(int* nbDepValid, t_character* selected_character){
    int mvtEffectue = 0;
    int nbVoisins=1;
    *nbDepValid = 0;
    int nbBoucle;
    liste_init();
    file_init();

    int indice;
    t_coord coordonnees = selected_character->position;
    
    file_ajouter(coordonnees);	//dans la file, on met les coordonnées des case pour explorer le terrain sur la portée du mouvement
    liste_ajout_droit(coordonnees);	//dans la liste on met toutes les cases atteignable!!

    while(mvtEffectue <= selected_character->stats.MVT)	//tant que le personnage n'a pas effectué tous ses déplacements
    {
        nbBoucle = nbVoisins;
        nbVoisins = 0;
        
        for(indice = 0; indice < nbBoucle; indice++)
        {



            file_retirer(&coordonnees);  
            liste_ajout_droit(coordonnees);	

            if(mvtEffectue < selected_character->stats.MVT)
                nbVoisins += cases_voisines_calcul(coordonnees);
        }
        mvtEffectue++;
    }
    liste_suppr_doublon();	//on supprime les doublons de la liste
    *nbDepValid = liste_calculer_nombre_elements();
}

/**
 * \fn t_coord choix_deplacement_humain(int joueur_courant, int* nbDepValid,t_character* selected_character)
 * \brief Permet au joueur de choisir la destination
 * Le joueur déplace un curseur qui représente son joueur courant, il est limité aux limites du plateau et au nombre de déplacements qu'il possède.
 *
 */
t_coord choix_deplacement_humain(int joueur_courant, int* nbDepValid,t_character* selected_character){
	
		int mvt_effectue = 0, x, y;
		char choix;
		x = selected_character->position.X;
		y = selected_character->position.Y;
		t_coord val = {x,y},sommet;
		pile_init(0);
		pile_empiler(val, 0);
		pile_sommet(&sommet,0);
	do	//on rafraîchit l'écran à chaque appui sur une touche
	{
		clear();
		if(Plateau[val.X][val.Y].type == TRAP_UNIT){move(TAILLE_MATRICE+3+5,0); printw("\n/!\\VOUS VOUS ETES PRIS UN PIEGE/!\\"); refresh();}	//si jamais le joueur se déplace sur un piège
		deplacer_perso(val,selected_character);
		move(0,0);
		printw("Deplacement: %i/%i\n", mvt_effectue, selected_character->stats.MVT);
		afficher_plateau_orientation(joueur_courant);
		printw("\n\nValider =\"Entrée\"\nAnnuler =\"BackSpace\"");
		attron(COLOR_PAIR(7));
		mvaddch(y+2,x+3,curseur);
		attroff(COLOR_PAIR(7));
		//refresh();
		choix = getch();
		if((choix == 127 || choix == 8) && mvt_effectue > 0)//touche backSpace = touche effacer
		{
			mvt_effectue--;
			pile_depiler(&val, 0);
			x = val.X;
			y = val.Y;
		}
		if(choix == 68 || choix == 22475)//flèche de gauche
		{
			
			if(x > 0)
			{
				x--;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)	//si jamais on reviens sur nos pas
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character->stats.MVT )
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
		if(choix == 67 || choix == 22477)//flèche de droite
		{
			if(x < TAILLE_MATRICE-1)
			{
				x++;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)	//si jamais on reviens sur nos pas
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character->stats.MVT )
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
		if(choix == 65 || choix == 22472)//flèche du haut
		{
			if(y > 0)
			{
				y--;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)	//si jamais on reviens sur nos pas
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character->stats.MVT )
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
		if(choix == 66 || choix == 22480)//flèche du bas
		{
			if(y < TAILLE_MATRICE-1)
			{
				y++;
				if(Plateau[x][y].camp == terrain || Plateau[x][y].type == TRAP_UNIT)
				{
					if(x == sommet.X && y == sommet.Y)	//si jamais on reviens sur nos pas
					{
						mvt_effectue--;
						pile_depiler(&val,0);
					}else if ( mvt_effectue < selected_character->stats.MVT )
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
		
	}while(choix != '\n' && choix  != 13);
	
	return((t_coord){x,y});
}



/**
 * \fn t_coord choix_deplacement_IA(int* nbDepValid,t_character* selected_character)
 * \brief cette fonction retourne les coordonnées du déplacement choisi pour l'IA
 */
t_coord choix_deplacement_IA(int* nbDepValid,t_character* selected_character){
	
    int i = 0, choix;
    t_coord coordonnees, choisi;
    liste_en_tete();
       while(!liste_est_hors_liste()){
        liste_valeur_elt(&coordonnees);
        liste_suivant();
        i++;
    }
	
	do 
	{
		choix=generation_nombre_aleatoire(*nbDepValid);
	}while (choix>*nbDepValid && choix < 0);
	
        liste_en_tete();
        for(i = 0; i < choix-1; i++)
			liste_suivant();
        liste_valeur_elt(&choisi);
        coordonnees.X=choisi.X;
        coordonnees.Y=choisi.Y;
        printw("Déplacement IA %s en %i,%i\n",selected_character->name,coordonnees.X,coordonnees.Y);
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

/**
 * \fn int cases_voisines_ATK(t_coord coordonnees)
 * \brief Renvoi le nombre de case voisine.
 * Met dans la file la liste des coordonnées voisines.
 */
int cases_voisines_ATK(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
        coord = coordonnees;
        coord.X += 1;
		
        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;				//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }

        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;				//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }

        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;				//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }

        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            file_ajouter(coord);	//ajoute à la file les coordonnées si elles sont valides et accessibles
            nbVois++;				//incrémente le nombre d'éléments AJOUTÉS à la liste!
        }

    return(nbVois);
}
/**
 * \fn void viser_case_valide(t_skill skill, int* nbAtkValid,t_character* selected_character)
 * \brief Calcule les positions d'attaques valides, les met dans la liste. une case vide peut être attaquée.
 */
void viser_case_valide(t_skill skill, int* nbAtkValid,t_character* selected_character)
{
    int mvtEffectue = 0;
    int nbVoisins=1;
    int nbBoucle;
    *nbAtkValid = 0;
    liste_init();
    file_init();
    int nb_iteration;
    t_coord coordonnees = selected_character->position;
    file_ajouter(coordonnees); //dans la file, on met les coordonnées des case pour explorer le terrain sur la portée du coup
    liste_ajout_droit(coordonnees); // dans la liste, on a toutes les cases attaquables !!
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

/**
 * \fn t_coord choix_cible_IA(t_skill skill, int* nbAtkValid)
 * \brief Choisi parmi la liste des cases à attaquer pour l'IA, une case vide peut être attaquée.
 */
t_coord choix_cible_IA(t_skill skill, int* nbAtkValid)
{
    int i =0,choix;
    t_coord coordonnees;
    liste_en_tete();

	do{
        choix=generation_nombre_aleatoire(*nbAtkValid);
        liste_en_tete();
        for(i = 0; i < choix-1; i++)
			liste_suivant();
        liste_valeur_elt(&coordonnees);
    }while (!(choix<=*nbAtkValid && choix > 0));
	printw("Cible:%i,%i\n",coordonnees.X,coordonnees.Y);

    return (coordonnees);
}


/**
 * \fn void init_char_table(t_character chars[])
 * \brief Fonction d'initialisation d'un tableau de personnages.
 * Remplit le tableau de personnages entré en paramètre de cases de terrain.
 *
 */
void init_char_table(t_character chars[])
{
    int i;
    for(i=0;i<MaxTab;i++) chars[i]=character_default;
}

/**
 * \fn void afficher_skill(int skill_nb, t_skill skill,t_character selected_character)
 * \brief Affiche le nom, la portée, la puissance d'une compétence.
 */
void afficher_skill(int skill_nb, t_skill skill,t_character selected_character){
    printw("   %s ",skill.name);
    if(skill.range==0 && skill.type != EMPTY) printw("on self ");
    if(skill.range==1) printw("melee ");
    if(skill.range>1) printw("range=%i ",skill.range);
    if(skill.type==ATK) printw("power=%i",selected_character.stats.ATK*skill.damage_coeff);
    if(skill.type==MATK) printw("power=%i",selected_character.stats.MATK*skill.damage_coeff);
    if(skill.type==EMPTY && strcmp(skill.name, "") ==0) printw(" --- ");;
    printw("\n");
}

/**
 * \fn void afficher_skill_list(t_character perso)
 * \brief Affiche la liste des compétence d'un personnage
 */
void afficher_skill_list(t_character perso){
		afficher_skill(1,perso.skill.a,perso);
        afficher_skill(2,perso.skill.b,perso);
        afficher_skill(3,perso.skill.c,perso);
        afficher_skill(4,perso.skill.d,perso);
		afficher_skill(5,perso.skill.e,perso);
        afficher_skill(6,perso.skill.wait,perso);
}

/**
 * \fn void afficher_infos_persos( t_character perso)
 * \brief Affiche les informations du personnage
 */
void afficher_infos_persos( t_character perso)
{
	move(1, TAILLE_MATRICE+3+10);
	printw("Nom : %s ",perso.name);
	move(2, TAILLE_MATRICE+3+10);
	if (perso.classe==1) printw("Classe : Berseker ");
	if (perso.classe==2) printw("Classe : Mage ");
	if (perso.classe==3) printw("Classe : Tank ");
	if (perso.classe==4) printw("Classe : Soigneur ");
	move(3, TAILLE_MATRICE+3+10);
	printw("%i/%i HP - %i/%i MP",perso.status.HP,perso.status.Max_HP,perso.status.MP,perso.status.Max_MP);
	move(4, TAILLE_MATRICE+3+10);
	printw("ATK:%i - MATK:%i",perso.stats.ATK,perso.stats.MATK);
	move(5, TAILLE_MATRICE+3+10);
	printw("DEF:%i - MDEF:%i",perso.stats.DEF,perso.stats.MDEF);
	move(6, TAILLE_MATRICE+3+10);
	printw("MVT:%i - Nbre action/tour:%i ",perso.stats.MVT,perso.nb_action_tour);
	refresh();
	move(TAILLE_MATRICE+3+1,0);
	afficher_skill_list(perso);
}	



/**
 * \fn t_coord choix_cible_humain(t_skill skill, int joueur_courant,t_character* selected_character)
 * \brief Permet au joueur de choisir, grâce à un curseur, une case à attaquer.
 */
t_coord choix_cible_humain(t_skill skill, int joueur_courant,t_character* selected_character)
{
	
    int valid = 0;
    char choix;
    int x=selected_character->position.X, y=selected_character->position.Y;
    t_coord val;
    
    
    do	//rafraîchit l'écran dès l'appui sur une touche
	{
		clear();
		printw("Choisissez votre cible: \n", valid);
		afficher_plateau_orientation(joueur_courant);
		printw("\nCible ");
		if(valid == 0)printw("non ");
		printw("valide");
		if(Plateau[x][y].camp>0 && Plateau[x][y].type != TRAP_UNIT){afficher_infos_persos(Plateau[x][y]);}
		attron(COLOR_PAIR(7));
		mvaddch(y+2,x+3,curseur);
		attroff(COLOR_PAIR(7));
		//refresh();
		choix = getch();
		if(choix == 127 || choix == 8)
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
		valid = liste_element_est_present(val, 0);	//vérifie si la case sélectionnée est présente dans la liste des cases attaquables
		
	}while((choix != '\n' && choix != 13) || valid == 0);
	return (val);
}


/**
* \fn void selection_perso(int joueur_courant,t_character* selected_character)
* \brief Cette fonction permet au joueur de choisir un de ses persos sur le plateau grâce à un curseur
*
*/
void selection_perso(int joueur_courant,t_character* selected_character){
	
	int x = 0,y = 0;
	char choix;
	
	do	//rafraîchit l'écran dès l'appui sur une touche
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
		if(Plateau[x][y].camp>0  && (Plateau[x][y].type != TRAP_UNIT || Plateau[x][y].camp == joueur_courant)){afficher_infos_persos(Plateau[x][y]);}
		attron(COLOR_PAIR(7));
		mvaddch(y+2,x+3,curseur);	//affiche le curseur
		attroff(COLOR_PAIR(7));
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
		
	}while((choix != '\n' && choix != 13) || (Plateau[x][y].camp != joueur_courant || Plateau[x][y].type == TRAP_UNIT));
	*selected_character = Plateau[x][y];
	Plateau[x][y]= case_terrain;
}

/**
* \fn void passer_tour()
* \brief Fonction qui passe le tour du joueur actif
*
*/
void passer_tour(){

}

/**
* \fn void suicide(int joueur_courant)
* \brief Fonction permetant au joueur courant d'abandonner la partie
*
*/
void suicide(int joueur_courant){
    int i,j;
    for(i=0;i<TAILLE_MATRICE;i++){
        for(j=0;j<TAILLE_MATRICE;j++){
            if(Plateau[i][j].camp==joueur_courant){
				Plateau[i][j]=case_terrain;	//Remplace tout les personnage du joueur par une case vide
            }
        }
    }
}

/**
 * \fn t_skill select_skill(t_character selected_character)
 * \brief Permet au joueur de choisir,  graphiquement, une compétence à effectuer dans un menu
 */
t_skill select_skill(t_character selected_character){
        
        int choix, indice_curseur = 1, nb_choix_max = 6;
        
        do
        {
        clear();
        printw("Quel compétence voulez-vous utiliser ? \n\n");
            afficher_skill(1,selected_character.skill.a,selected_character);
            afficher_skill(2,selected_character.skill.b,selected_character);
            afficher_skill(3,selected_character.skill.c,selected_character);
            afficher_skill(4,selected_character.skill.d,selected_character);
            afficher_skill(5,selected_character.skill.e,selected_character);
            afficher_skill(6,selected_character.skill.wait,selected_character);
            mvaddch((indice_curseur+1),1, '-');	//affiche le curseur
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
        }
        while ((choix !='\n' && choix != 13) );

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

    if((xenemy-yenemy>=xperso-yperso)&&(xenemy+yenemy<xperso+yperso))//Vérifie si la cible est au Nord
    {
        switch(oenemy)
        {
            case up: return back ; break;
            case right: if(xenemy-yenemy==xperso-yperso){return front;}else return side ; break;
            case down: return front ;break;
            case left: if(xenemy-yenemy==xperso-yperso){return back;}else return side ; break;
        }
    }
    else if((xenemy-yenemy>xperso-yperso)&&(xenemy+yenemy>=xperso+yperso))//Vérifie si la cible est à l'Est
    {
        switch(oenemy)
        {
            case up: if(xenemy+yenemy==xperso+yperso){return back;}else return side ; break;
            case right: return back ; break;
            case down: if(xenemy+yenemy==xperso+yperso){return front;}else return side ;break;
            case left: return front ; break;
        }
    }
    else if((xenemy-yenemy<=xperso-yperso)&&(xenemy+yenemy>xperso+yperso))//Vérifie si la cible est au Sud
    {
        switch(oenemy)
        {
            case up: return front ; break;
            case right: if(xenemy+yenemy==xperso+yperso){return back;}else return side ; break;
            case down: return back ;break;
            case left: if(xenemy+yenemy==xperso+yperso){return front;}else return side ; break;
        }
    }
    else if((xenemy-yenemy<xperso-yperso)&&(xenemy+yenemy<=xperso+yperso))//Vérifie si la cible est à l'Ouest
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
    int coefOrientation;
    t_targetOrientation targetOrientation= get_target_orientation(lanceur,cible);
    

    if(targetOrientation==0) coefOrientation=1;
    if(targetOrientation==1) coefOrientation=1;
    if(targetOrientation==2) coefOrientation=2;


    if (action.type == MATK){
        printw("MATK : %d,%d\n", cible.X, cible.Y);
        total_dmg=action.damage_coeff*lanceur.stats.MATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.MDEF/10; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
                total_dmg*=coefOrientation;
        }
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    else if (action.type == ATK){
        printw("ATK : %d,%d\n", cible.X, cible.Y);
        total_dmg=action.damage_coeff*lanceur.stats.ATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.DEF/10; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
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
    }

    if (Plateau[cible.X][cible.Y].status.HP< 0) Plateau[cible.X][cible.Y].status.HP=0;
    if (Plateau[cible.X][cible.Y].status.HP>Plateau[cible.X][cible.Y].status.Max_HP) Plateau[cible.X][cible.Y].status.HP=Plateau[cible.X][cible.Y].status.Max_HP;
    if (Plateau[cible.X][cible.Y].status.HP<= 0) Plateau[cible.X][cible.Y]=case_terrain;
}


/**
* \fn void orienter_perso_fleches(int joueur_courant, t_character* selected_character)
* \brief Cette fonction permet au joueur d'orienter son personnage avec les flèches directionnelles
* 
*
*/
void orienter_perso_fleches(int joueur_courant, t_character* selected_character){
    char input, curs;

	switch(selected_character->orientation)
	{
		case 0:
			curs = '^';break;
			
		case 1:
			curs = '>';break;
			
		case 2:
			curs = 'v';break;
			
		case 3:
			curs = '<';break;
	}
    t_orientation tampon_orientation;
    
    while(input != '\n' && input != 13)
    {
        clear();
		printw("Choisissez l'orientation : \n");
		afficher_plateau_orientation(joueur_courant);
		printw("Valider =\"Entrée\"");
		mvaddch(selected_character->position.Y+2, selected_character->position.X+3, curs);
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
    }
        switch(curs)
        {
            case '^':tampon_orientation=up;break;
            case '>':tampon_orientation=right;break;
            case 'v':tampon_orientation=down;break;
            case '<':tampon_orientation=left;break;
            default:printw("Orientation incorrecte");
        }
    
    Plateau[selected_character->position.X][selected_character->position.Y].orientation=tampon_orientation;
    selected_character->orientation=tampon_orientation;
    
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
* \fn int are_my_mates_alive(int joueur_courant)
* \brief Prend en paramètre le joueur courant et vérifie si tous ses personnages ne sont pas morts.
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

/**
* \fn int life_check(int joueur_courant)
* \brief Fonction qui vérifie si un joueur a encore des personnages vivants sur le terrain.
* Appelle are_my_mates_alive(int joueur_courant) pour déterminer le joueur est encore en jeu.
* Renvoie 1 si le joueur a encore des personnages en vie, sinon 0.
*/
int life_check(int joueur_courant){
          
    if (player[joueur_courant].alive==0 || are_my_mates_alive(joueur_courant)<1 ) 
    {
		player[joueur_courant].alive=0;
        //if (joueur!=sauvage) printw("%s est mort.\n",player[joueur_courant].name);
        return 0;
    }else {
        return 1;
    }
}


/**
* \fn void players_life_check()
* \brief Actualise le fait que les joueurs soient vivants ou non.
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
* \fn void tour(int joueur_courant, int* nbAtkValid,int* nbDepValid,t_character* selected_character)
* \brief Fonction de déroulement d'un tour pour le joueur entré en paramètre.
*
*
*/
void tour(int joueur_courant, int* nbAtkValid,int* nbDepValid,t_character* selected_character)
{
	
    int choix, nb_actions_faites = 0, deplacement_fait, indice_curseur =1, nb_choix_max = 5, coord_curs, est_vivant = 1;
    t_skill skill_selected;
    t_coord coordonnees_perso;
    
    do
    {
        do                     //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
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
			if(choix == 65 || choix == 72)
			{
				if(indice_curseur <= 1) indice_curseur = nb_choix_max+1; //nb_choix_max = 5 = le nombre de choix
				indice_curseur--;
			}
			if(choix == 66 || choix == 80)
			{
				if(indice_curseur >= nb_choix_max) indice_curseur = 0;
				indice_curseur++;
			}
        }
        while ((choix != '\n' && choix != 13) );
        
            switch(indice_curseur)
            {   case 1: printw("\n");selection_perso(joueur_courant,selected_character); break;
                case 2: printw("\n");passer_tour(); break;
                case 3: printw("\n");suicide(joueur_courant);life_check(joueur_courant); break;//Abandon
                case 4: printw("\n");Sauvegarder(joueur_courant);break;
                case 5: printw("\n");joueur_courant=Charger(joueur_courant);break;
                default: printw("Erreur: votre choix doit être compris entre 1 et 5\n");
		}
}while(indice_curseur>=4);
		
    if (indice_curseur==1){   // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné
		
        //skill_selected.type = ATK;	WTF Nigga ?
        while(nb_actions_faites < selected_character->nb_action_tour && est_vivant == 1)
        {
            if(deplacement_fait != 1)
            {
                deplacements_valides(nbDepValid,selected_character);
                coordonnees_perso = choix_deplacement_humain(joueur_courant, nbDepValid,selected_character);
                deplacer_perso(coordonnees_perso,selected_character);
                //deplacer_perso(choix_deplacement_humain());
                deplacement_fait = 1;
            }
            if (Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.HP< 0) {Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.HP=0; est_vivant = 0;}
			if (Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.HP>Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.Max_HP) Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.HP=Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.Max_HP;
			if (Plateau[coordonnees_perso.X][coordonnees_perso.Y].status.HP<= 0) 
			{
				Plateau[coordonnees_perso.X][coordonnees_perso.Y]=case_terrain;
				clear();
				afficher_plateau_orientation(joueur_courant);
				printw("Votre personnage est MORT!");
				getch();
			}
			else
			{
			skill_selected = select_skill(*selected_character);//selectionne l’action que le joueur souhaite effectuer (exple: selection_perso / passer_tour…) retourne le n° de l’action à effectuer
			//tampon_skill = skill_selected;
			if(skill_selected.type != EMPTY)
			{
				viser_case_valide(skill_selected, nbAtkValid,selected_character);
				appliquer_action(*selected_character, choix_cible_humain(skill_selected, joueur_courant,selected_character), skill_selected);
			}else 
			{
				appliquer_action(*selected_character, selected_character->position, skill_selected);
			}
			nb_actions_faites++;
			orienter_perso_fleches(joueur_courant,selected_character);
		}
        }
    }
}

/**
* \fn int all_dead_but_one(int nb_joueurs)
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
        printw("|");
    }
	printw("\n");
	refresh();
}

/**
* \fn void creer_perso_rapide(t_camp camp,int x, int y,t_classe classe )
* \brief Création brève de perso.
* 
*/
void creer_perso_rapide(t_camp camp,int x, int y,t_classe classe )
{
	
	char chaine_tampon[MaxString];
	chaine_tampon[0]=0;
	generation_nom_personnage(chaine_tampon);
    Plateau[x][y]=character_default;
    if (classe==0) Plateau[x][y]=character_default;
    if (classe==1) Plateau[x][y]=character_berseker;
    if (classe==2) Plateau[x][y]=character_mage;
    if (classe==3) Plateau[x][y]=character_tank;
    if (classe==4) Plateau[x][y]=character_soigneur;
    if (camp>terrain) strcpy(Plateau[x][y].name, chaine_tampon);else strcpy(Plateau[x][y].name, "Mauvaise fonction de creation");
    Plateau[x][y].camp=camp;
    Plateau[x][y].position.X=x;
    Plateau[x][y].position.Y=y;
    if (camp==2) Plateau[x][y].orientation=down;
    if (camp==3) Plateau[x][y].orientation=up;
    Plateau[x][y].classe=classe;
    
}

/**
* \fn void creer_terrain_rapide(t_camp camp,int x, int y)
* \brief Création brève de terrain ou obstacle, possible implémentation de génération aléatoire en cas d'obstacle.
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


/**
 * \fn void generation_nom_personnage(char * nom)
 * \brief génère aléatoirement un nom pour un perso
 */
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

/**
 * \fn void edit_stats( t_character perso,int HP , int Max_HP , int MP , int Max_MP,int ATK , int MATK , int DEF , int MDEF , int MVT)
 * \brief modifie les statistiques d'un personnage
 */
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
/**
 * \fn void spawn_sauvage()
 * \brief Créé un personnage du camp neutre à une position aléatoire sur le terrain
 */
void spawn_sauvage()
{
	player[1].alive=1;
	int x_buffer,y_buffer;
	do{
		x_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
		y_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
	}while(Plateau[x_buffer][y_buffer].camp !=0);
	printw("Coordonnées de creation du personnage chaotique : %i , %i. Camp case : %i",x_buffer,y_buffer,Plateau[x_buffer][y_buffer].camp);
	creer_perso_rapide(sauvage,x_buffer,y_buffer,berseker);
	//edit_stats(Plateau[x_buffer][y_buffer],20,20,10,10,30,30,10,20,5);
	Plateau[x_buffer][y_buffer].status.HP=50;
	Plateau[x_buffer][y_buffer].status.Max_HP=50;
	
}

/**
 * \fn void spawn_character(t_camp camp_nouveau_perso)
 * \brief Permet la création d'un personnage joueur à une position aléatoire
 * 
 */
void spawn_character(t_camp camp_nouveau_perso)
{
	player[camp_nouveau_perso].alive=1;
	int x_buffer,y_buffer;
	do{
		x_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
		y_buffer=generation_nombre_aleatoire(TAILLE_MATRICE)-1;
	}while(Plateau[x_buffer][y_buffer].camp !=0);
	printw("Coordonnées de creation du personnage de %s : %i , %i.",player[camp_nouveau_perso].name,x_buffer,y_buffer);
	creer_perso_rapide(camp_nouveau_perso,x_buffer,y_buffer,generation_nombre_aleatoire(3)+1);
	//edit_stats(Plateau[x_buffer][y_buffer],20,20,10,10,30,30,10,20,5);
	
}

/**
 * \fn void tour_IA(int joueur_courant, int* nbAtkValid, int* nbDepValid,t_character* selected_character)
 * \brief Fonction de déroulement d'un tour pour l'IA.
 * 
 */
void tour_IA(int joueur_courant, int* nbAtkValid, int* nbDepValid,t_character* selected_character)
{
    int nb_actions_faites, i;
    int nb_perso_vivants=calcul_persos_IA(joueur_courant);
    //printw("nb_persos_vivants:%i\n",nb_perso_vivants);
    t_skill skill_selected;
    for (i=0; i<nb_perso_vivants ; i++ )
    {	
		*selected_character=Plateau[Valid_chars_IA[i].position.X][Valid_chars_IA[i].position.Y];
        if (selected_character->camp==joueur_courant) //Vérifie que la case appartient à l'IA, en cas de mort d'un perso durant le tour d'un autre perso de l'IA, sinon contrôle d'une case terrain ou obstacle, ce qui serait gênant.
        {		
                for(nb_actions_faites=0;nb_actions_faites < selected_character->nb_action_tour;nb_actions_faites++)
            {

                deplacements_valides(nbDepValid,selected_character);

				
                deplacer_perso(choix_deplacement_IA(nbDepValid,selected_character),selected_character);
                skill_selected = selected_character->skill.a;
                
                if(skill_selected.type != EMPTY)
                {

					
                    viser_case_valide(skill_selected, nbAtkValid,selected_character);
                    appliquer_action(*selected_character, choix_cible_IA(skill_selected,nbAtkValid), skill_selected);
                }else 
                {
                    appliquer_action(*selected_character, selected_character->position, skill_selected);
                }

            }
        }
    }
}

/**
 * \fn int calcul_persos_IA(int joueur_courant){
 * \brief Calcule le nombre de personnage controlé par l'IA
 * 
 */
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
                nb_perso++;	//Incrémente si le personnage est bien du camp sauvage
            }
        }
    }
    return nb_perso;
}

/**
 * \fn void saisie_nombre_joueurs(int* nb_joueurs )
 * \brief permet à l'utilisateur de rentrer le nombre de joueur à jouer
 */
void saisie_nombre_joueurs(int* nb_joueurs )
{
	scanw("%i", &nb_joueurs);
}

/**
* \fn int main()
* \brief Fonction principale
* Fonction principale qui permet de jouer en mode Kill'em'all.
*
*/
int main(){
	int i;
	t_character selected_character;
	int nb_joueurs, joueur_courant;
	int nbAtkValid;
	int nbDepValid;
	saisie_nombre_joueurs(&nb_joueurs);
	srand((long)time(NULL));
	
	
	initscr();	//initialisation de l'écran pour le Ncurses
	raw();
	curs_set(0);	//curseur invisible
	
	
    start_color();	//initialistaion des couleurs
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	
	strcpy(player[1].name,"la nature");
	strcpy(player[2].name,"Baptiste");
	strcpy(player[3].name,"Yann");
	strcpy(player[4].name,"Arthur");
	
	creer_terrain_rapide(obstacle,3,2);

	creer_terrain_rapide(obstacle,1,2);
	creer_terrain_rapide(obstacle,0,2);

	
	creer_perso_rapide(2,4,0,berseker);
	creer_perso_rapide(2,3,0,mage);
	creer_perso_rapide(2,2,0,soigneur);
	creer_perso_rapide(2,1,0,tank);
	creer_perso_rapide(2,0,0,default_class);
		
	creer_perso_rapide(3,0,5,berseker);
	creer_perso_rapide(3,1,5,mage);
	creer_perso_rapide(3,2,5,soigneur);
	creer_perso_rapide(3,3,5,tank);
	creer_perso_rapide(3,4,5,default_class);
	joueur_courant=2;
	
    
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
            if (generation_nombre_aleatoire(4)==1) spawn_sauvage();
            
            afficher_plateau_orientation(joueur_courant);
            character_hp_list();
            if (joueur_courant==sauvage) 
            {
                tour_IA(joueur_courant,&nbAtkValid,&nbDepValid,&selected_character);
                
            }else tour(joueur_courant,&nbAtkValid,&nbDepValid,&selected_character);
            
            
        }
        
        
        players_life_check();
        joueur_liste_suivant(nb_joueurs, &joueur_courant);
    }
    clear();
	afficher_plateau_orientation(joueur_courant);


	for (i=2;i<=nb_joueurs;i++)
	{
		if (player[i].alive==1) printw("Victoire de %s !\n",player[i].name);
	}
        
	getch();
	endwin();
	
    return EXIT_SUCCESS;
}
