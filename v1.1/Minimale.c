#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "header.h"
#include "liste.c"
#include "file.c"
#include "liste.h"
#include "file.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//Prototypes fonctions, à inclure dans un futur Minimale.h !
int life_check();
void joueur_suivant(int nb_joueur);
void creer_terrain_rapide(t_camp camp,int x, int y);
void vider_buffer(void);
int are_my_mates_alive();
int calcul_persos_IA();
void afficher_plateau_orientation(void);
void generation_nom(char * nom);
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
	
int nb_joueurs=4;
t_character selected_character;
void players_life_check();
t_character Plateau[TAILLE_MATRICE][TAILLE_MATRICE];
t_coord depValid[TAILLE_MATRICE*TAILLE_MATRICE][3];
int nbDepValid;
int nbAtkValid;
t_camp joueur;
t_player player[MaxTab];



int indiceTabDepValid;

/**
 * \fn void Sauvegarder()
 * \brief Sauvegarde la partie dans un fichier
 *
 */
void Sauvegarder(){
    char NomFichier[20]="",FichierSauvegarde[20]="";
    int present=1;

    FILE * fichierIndex;
    FILE * sauvegarde;
    fichierIndex=fopen("IndexSauvegarde.txt","r");
    fscanf(fichierIndex,"%s",FichierSauvegarde);
    if(!feof(fichierIndex)){
        printf("Sauvegarde deja existante :\n");

        while(!feof(fichierIndex))
        {
            printf("%s\n",FichierSauvegarde);
            fscanf(fichierIndex,"%s",FichierSauvegarde);
        }
    }else {printf("Pas de sauvegarde disponible.\n");
    }
    printf("\n");
    fclose(fichierIndex);
    fichierIndex=fopen("IndexSauvegarde.txt","r+");
    printf("Entrez un nom de sauvegarde :");
    scanf("%s",NomFichier);
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
    fwrite(&joueur,sizeof(t_camp),sizeof(t_camp),sauvegarde);
    fclose(fichierIndex);
    fclose(sauvegarde);
}

/**
 * \fn void Charger()
 * \brief Permet de charger une partie depuis un fichier
 *
 */
void Charger(){
    char NomFichier[20]="",FichierSauvegarde[20]="";

    FILE * fichierIndex;
    FILE * sauvegarde;
    fichierIndex=fopen("IndexSauvegarde.txt","r");
    fscanf(fichierIndex,"%s",FichierSauvegarde);
    if(!feof(fichierIndex)){
        printf("Sauvegarde deja existante :\n");

        while(!feof(fichierIndex))
        {
                printf("%s\n",FichierSauvegarde);
                fscanf(fichierIndex,"%s",FichierSauvegarde);
        }
        printf("\n");
        do{
                printf("Choisir un fichier : ");
                scanf("%s",NomFichier);
                sauvegarde=fopen(NomFichier,"rb");
            if(sauvegarde!=NULL){
                fread(Plateau,TAILLE_MATRICE*TAILLE_MATRICE,sizeof(t_character),sauvegarde);
                fread(player,sizeof(player)/sizeof(t_player),sizeof(t_player),sauvegarde);
                fread(&joueur,sizeof(t_camp),sizeof(t_camp),sauvegarde);
            }else {printf("Fichier incorrect\n");}
        }while(!feof(sauvegarde));
            fclose(sauvegarde);
            afficher_plateau_orientation();
        }else {
printf("Pas de sauvegarde disponible.\n");
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
    selected_character.position.X=case_perso.X;
    selected_character.position.Y=case_perso.Y;
    Plateau[case_perso.X][case_perso.Y]= selected_character;

}

/**
 * \fn int cases_voisines_calcul(t_coord coordonnees)
 * \brief Renvoi le nombre de case voisine vide
 *
 */
int cases_voisines_calcul(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
    if(Plateau[coordonnees.X+1][coordonnees.Y].camp == 0)
    {
        coord = coordonnees;
        coord.X += 1;
        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X-1][coordonnees.Y].camp == 0)
    {
        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X][coordonnees.Y+1].camp == 0)
    {
        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X][coordonnees.Y-1].camp == 0)
    {
        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    return(nbVois);
}


/**
 * \fn void deplacements_valides()
 * \brief Calcule les positions de déplacement valide
 *
 */
void deplacements_valides(){// permet de calculer les positions valides pour son perso.
    int mvtEffectue = 0;
    int nbVoisins=1;
    nbDepValid = 0;
    int nbBoucle;
    init_liste();
    initFile();

    int indice;
    t_coord coordonnees = selected_character.position;
    ajouterFile(coordonnees);
    ajout_droit(coordonnees);
    valeur_elt(&coordonnees);

    while(mvtEffectue <= selected_character.stats.MVT)
    {
        nbBoucle = nbVoisins;
        nbVoisins = 0;
        
        for(indice = 0; indice < nbBoucle; indice++)
        {
/*
 * 		int bug_detected;
		if (bug_detected==0) if (perso_oriente_a_droite()) bug_detected=1;
if (bug_detected==1) 
{
	printf("CHECKPOINT BLABBERSTRONG %i\n",indice);
	afficher_plateau_orientation();
}
*/
            retirerFile(&coordonnees);  //Problème soit là
            ajout_droit(coordonnees);	//Soit là

            if(mvtEffectue < selected_character.stats.MVT)//Soit là
                nbVoisins += cases_voisines_calcul(coordonnees);//Soit là
        }
        mvtEffectue++;
    }
    suppr_doublon();
    nbDepValid = calculerElemListe();
    //printf("CHECKPOINT 3");
    afficher_plateau_orientation();
                   //afficher_liste();
}

/**
 * \fn t_coord choix_deplacement_humain()
 * \brief Permet au joueur de choisir la destination
 *
 */
t_coord choix_deplacement_humain(){
    int i = 0, j, choix = 1;
    vider_buffer();
    t_coord coordonnees, choisi;
    en_tete();
    printf("Quel Deplacement voulez-vous effectuer ? \n");
    while(!hors_liste()){
        valeur_elt(&coordonnees);
        suivant();
        printf("   %i - (%i,%i)\n",i+1, coordonnees.X, coordonnees.Y);
        i++;
    }

    while(choix){
        scanf("%i",&j);
        if(j<=nbDepValid && j > 0)
        {
            en_tete();
            for(i = 0; i < j-1; i++)
                suivant();
            valeur_elt(&choisi);
            coordonnees.X=choisi.X;
            coordonnees.Y=choisi.Y;
            choix=0;
        }else{printf("Numero incorrect\n");}
    }
    printf("\n");
    return (coordonnees);
}

t_coord choix_deplacement_IA(){
	
    int i = 0, choix;
    t_coord coordonnees, choisi;
    en_tete();
       while(!hors_liste()){
        valeur_elt(&coordonnees);
        suivant();
        //printf("   %i - (%i,%i)\n",i+1, coordonnees.X, coordonnees.Y);
        i++;
    }
    
    //printf("nbDepValid: %i\n",nbDepValid);
	
	do 
	{
		choix=generation_nombre_aleatoire(nbDepValid);
	}while (choix>nbDepValid && choix < 0);
	
       // printf("Choix: %i\n",choix);
        en_tete();
        for(i = 0; i < choix-1; i++)
			suivant();
        valeur_elt(&choisi);
        coordonnees.X=choisi.X;
        coordonnees.Y=choisi.Y;
        //printf("Déplacement IA %s en %i,%i\n",selected_character.name,coordonnees.X,coordonnees.Y);
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
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= TAILLE_MATRICE-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

    return(nbVois);
}

void viser_case_valide(t_skill skill)
{
    int mvtEffectue = 0;
    int nbVoisins=1;
    int nbBoucle;
    nbAtkValid = 0;
    init_liste();
    initFile();
    int nb_iteration;
    t_coord coordonnees = selected_character.position;
    ajouterFile(coordonnees);
    ajout_droit(coordonnees);
    valeur_elt(&coordonnees);
    while(mvtEffectue <= skill.range)
    {
        nbBoucle = nbVoisins;
        nbVoisins = 0;
        for(nb_iteration = 0; nb_iteration < nbBoucle; nb_iteration++)
        {
            retirerFile(&coordonnees);
            ajout_droit(coordonnees);
            if(mvtEffectue < skill.range)
                nbVoisins += cases_voisines_ATK(coordonnees);
        }
        mvtEffectue++;
    }
    suppr_doublon();
    nbAtkValid = calculerElemListe();
}

t_coord choix_cible_humain(t_skill skill)
{
    int i =0,choix;
    t_camp tampon_camp_cible;
    vider_buffer();
    t_coord coordonnees;
    en_tete();
    printf("Choisissez votre cible. \n\n");
    while(!hors_liste()){
        valeur_elt(&coordonnees);
        tampon_camp_cible=Plateau[coordonnees.X][coordonnees.Y].camp;
        suivant();
        printf("   %i - ",i+1);
        printf("%s %iHP", Plateau[coordonnees.X][coordonnees.Y].name,Plateau[coordonnees.X][coordonnees.Y].status.HP);
        if (( tampon_camp_cible>0) && (joueur==tampon_camp_cible) ){
			printf(" - Ami");
		} else if (tampon_camp_cible>0)
		{
			printf(" - %s, joueur%i",player[tampon_camp_cible].name,tampon_camp_cible);
		}else if (tampon_camp_cible==0)
		{
			printf(" - Terrain");
		}else if (tampon_camp_cible==-1)
		{
			printf(" - Obstacle");
		}
        printf(" (%i,%i)",coordonnees.X, coordonnees.Y);
        printf("\n");
        i++;
    }
    
    do{
        
        scanf("%i",&choix);
        en_tete();
        for(i = 0; i < choix-1; i++)
			suivant();
        valeur_elt(&coordonnees);
    }while (!(choix<=nbAtkValid && choix > 0));
    return (coordonnees);
}

t_coord choix_cible_IA(t_skill skill)
{
    int i =0,choix;
   // t_camp tampon_camp_cible;
  // printf("Before ViderBuffer\n");
   // printf("After ViderBuffer.\n");
    t_coord coordonnees;
    en_tete();

do{
   //     printf("Debut do\n");
        choix=generation_nombre_aleatoire(nbAtkValid);
        en_tete();
        for(i = 0; i < choix-1; i++)
			suivant();
        valeur_elt(&coordonnees);
    //    printf("Fin do\n");
    }while (!(choix<=nbAtkValid && choix > 0));
   // printf("Cible:%i,%i\n",coordonnees.X,coordonnees.Y);
    //printf("ChoixCible:%i\n",choix);

    
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

/**
* \fn void selection_perso()
* \brief Fonction qui propose la liste des personnages pouvant être sélectionnés.
*
*/
void selection_perso(){
    int i,j,nb_perso=0,choix,choix_fait=1;
    t_character Valid_chars[MaxTab];
    init_char_table(Valid_chars);
    t_character perso_selectionne;
    vider_buffer();
    printf("Quel personnage voulez-vous selectionner ? \n");
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
	printf("\n");
    for(i=0;i<nb_perso;i++){
        printf("   %i - (%i,%i) %s HP: %i\n",i+1,Valid_chars[i].position.X,Valid_chars[i].position.Y,Valid_chars[i].name,Valid_chars[i].status.HP);
    }
	
    while(choix_fait){
        scanf("%i",&choix);
        if(choix<=nb_perso && choix > 0)
        {
            perso_selectionne=Valid_chars[choix-1];
            choix_fait=0;
        }else{printf("Numero incorrect");}
    }
    selected_character=perso_selectionne; //selected_character est la variable globale correspondant au perso sélectionné
}

/**
* \fn void PasserTour()
* \brief Fonction qui passe le tour du joueur actif
*
*
*/
void PasserTour(){//Permet au joueur de passer son tour

}

/**
* \fn void suicide()
* \brief Fonction permetant au joueur courant d'abandonner la partie
*
*/
void suicide(){// Permet au joueur de quitter la partie.
    int i,j;
    for(i=0;i<TAILLE_MATRICE;i++){
        for(j=0;j<TAILLE_MATRICE;j++){
            if(Plateau[i][j].camp==joueur){
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
void afficher_skill(int skill_nb, t_skill skill){
    printf("   %i - %s ",skill_nb,skill.name);
    if(skill.range==0) printf("on self ");
    if(skill.range==1) printf("melee ");
    if(skill.range>1) printf("range=%i ",skill.range);
    if(skill.type==ATK) printf("power=%i",selected_character.stats.ATK*skill.damage_coeff);
    if(skill.type==MATK) printf("power=%i",selected_character.stats.MATK*skill.damage_coeff);
    if(skill.type==EMPTY) printf(" ");;
    printf("\n");
}

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
        {   case 1:  printf("\n");return selected_character.skill.a;break;
            case 2:  printf("\n");return selected_character.skill.b;break;
            case 3:  printf("\n");return selected_character.skill.c;break;
            case 4:  printf("\n");return selected_character.skill.d;break;
            case 5:  printf("\n");return selected_character.skill.e;break;
            case 6:  printf("\n");return selected_character.skill.wait;break;
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
        int choix;
        vider_buffer();
        printf("Quel compétence voulez-vous utiliser ? \n\n");
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
            {   case 1:  printf("\n");return selected_character.skill.a;break;
                case 2:  printf("\n");return selected_character.skill.b;break;
                case 3:  printf("\n");return selected_character.skill.c;break;
                case 4:  printf("\n");return selected_character.skill.d;break;
                case 5:  printf("\n");return selected_character.skill.e;break;
                case 6:  printf("\n");return selected_character.skill.wait;break;
  default: printf("Erreur: votre choix doit être compris entre 1 et 6\n");
            }
        }
        while ((choix<1) ||(choix>6) );
        printf("\n");
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
    //typedef struct { char name[20] ; int range ; t_type type ; int damage_coeff ;} t_skill;
    //typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
    //typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
    //typedef enum {EMPTY,ATK,MATK}t_type;
    if(targetOrientation==0) coefOrientation=1;
    if(targetOrientation==1) coefOrientation=1;
    if(targetOrientation==2) coefOrientation=2;

    if (action.type == MATK){
        total_dmg=action.damage_coeff*lanceur.stats.MATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.MDEF/10;//*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
                total_dmg*=coefOrientation;
        }
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == ATK){
        total_dmg=action.damage_coeff*lanceur.stats.ATK; //degats avant réduction
        if(total_dmg>0) {
                total_dmg-=Plateau[cible.X][cible.Y].stats.DEF/10;//*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
                total_dmg*=coefOrientation;
        }
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == EMPTY){
    }

    if(action.type == LAND)
    {
        Plateau[cible.X][cible.Y] = default_trap;
        Plateau[cible.X][cible.Y].camp = lanceur.camp;
        Plateau[cible.X][cible.Y].position.X = cible.X;
        Plateau[cible.X][cible.Y].position.Y = cible.Y;
        //printf("%s, %i",Plateau[cible.X][cible.Y].name, Plateau[cible.X][cible.Y].camp);
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
  int c;
  while((c=getchar()) != EOF && c != '\n');
 
}

/**
* \fn void orienter_perso_numpad(t_character perso)
* \brief Propose une liste numérique des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void orienter_perso_numpad(){
    int choix=1;
    char input;
    t_orientation tampon_orientation;
    printf("Choisissez l'orientation : \n");
    vider_buffer();
    while(choix)
    {
        
        printf("\n  8 - Nord\n  6 - Est\n  5 - Sud\n  4 - Ouest\n");
        input = getchar();
        while (getchar() != '\n');
        switch(input)
        {
            case '8':tampon_orientation=up;choix=0;break;
            case '6':tampon_orientation=right;choix=0;break;
            case '5':tampon_orientation=down;choix=0;break;
            case '4':tampon_orientation=left;choix=0;break;
            default:printf("Orientation incorrecte");
        }
    }
    
    Plateau[selected_character.position.X][selected_character.position.Y].orientation=tampon_orientation;
    selected_character.orientation=tampon_orientation;
    
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
    printf("Choisissez l'orientation : \n");
    vider_buffer();
    while(choix)
    {
        
        printf("\n  z - Nord\n  d - Est\n  s - Sud\n  q - Ouest\n");
        input = getchar();
        while (getchar() != '\n');
        switch(input)
        {
            case 'z':buffer=up;choix=0;break;
            case 'd':buffer=right;choix=0;break;
            case 's':buffer=down;choix=0;break;
            case 'q':buffer=left;choix=0;break;
            default:printf("Orientation incorrecte");
        }
    }
    
    Plateau[selected_character.position.X][selected_character.position.Y].orientation=buffer;
    selected_character.orientation=buffer;
    
}


/**
* \fn void joueur_suivant(int nb_joueur)
* \brief Prend en paramètre le nombre de joueurs et incrémente le numéro de joueur de façon à ne pas dépasser le nombre de joueurs.
*
*/
void joueur_suivant(int nb_joueur)
{
    if (joueur==nb_joueur) joueur=1;
        else joueur++;

}


/**
* \fn int are_my_mates_alive()
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
int are_my_mates_alive(){
    int nb_perso=0,i,j;
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].camp==joueur)
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
* \fn int life_check(t_camp joueur)
* \brief Fonction qui vérifie si un joueur a encore des personnages vivants sur le terrain.
*    Appelle are_my_mates_alive() pour déterminer le joueur est encore en jeu.
*    Renvoie 1 si le joueur a encore des personnages en vie, sinon 0.
*/
int life_check(){
          
    if (player[joueur].alive==0 || are_my_mates_alive()<1 ) 
    {
		player[joueur].alive=0;
        //if (joueur!=sauvage) printf("%s est mort.\n",player[joueur].name);
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
			if(Plateau[i][j].camp>=1)
			{
				player[Plateau[i][j].camp].alive=1;
			}
		}
	}

    
}

/**
* \fn void tour(t_camp joueur)
* \brief Fonction de déroulement d'un tour pour le joueur entré en paramètre.
*
*
*/
void tour()
{
    vider_buffer();
    int choix, nb_actions_faites = 0, deplacement_fait=0;
    t_skill skill_selected;
    
        do                                            //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
        {   
			printf("\nTour de %s : \n",player[joueur].name);
            printf("   1 - Selectionner Perso\n");
            printf("   2 - Passer tour\n");
            printf("   3 - suicide\n");
            scanf("%d",&choix);

            switch(choix)
            {   case 1: printf("\n");selection_perso(); break;
                case 2: printf("\n");PasserTour(); break;
                case 3: printf("\n");suicide();players_life_check(); break;//Abandon
                default: printf("Erreur: votre choix doit être compris entre 1 et 3\n");
            }
        }
        while ((choix<1) ||(choix>3) );

	
		
    if (choix==1){   // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné
		
        //skill_selected.type = ATK;	WTF Nigga ?
        while(nb_actions_faites < selected_character.nbActionTour /* && skill_selected.type != EMPTY*/)
        {   
            if(deplacement_fait != 1)//GRos problème dans le déplacement, ptet à cause de moi.
            {
                //printf("selected_character.nbActionTour = %i nb_actions_faites =%i MVT=%i\n",selected_character.nbActionTour,nb_actions_faites,selected_character.stats.MVT);
                deplacements_valides();
                deplacer_perso(choix_deplacement_humain());
                deplacement_fait = 1;
            }
            skill_selected = select_skill();//selectionne l’action que le joueur souhaite effectuer (exple: selection_perso / PasserTour…) retourne le n° de l’action à effectuer
            if(skill_selected.type != EMPTY)
            {
                viser_case_valide(skill_selected);
                appliquer_action(selected_character, choix_cible_humain(skill_selected), skill_selected);
            }else 
            {
				appliquer_action(selected_character, selected_character.position, skill_selected);
			}
            nb_actions_faites++;
            orienter_perso_numpad();
            //orienter_perso();
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
* \fn void afficher_plateau_orientation(void)
* \brief Affiche le plateau avec les caractères correspondants à l'orientation
*
*/
void afficher_plateau_orientation(void){
    int i,j;
    for(j=-1;j<TAILLE_MATRICE;j++)
    {
        for(i=0;i<TAILLE_MATRICE;i++)
        {
			printf("\x1B[1;47m");
			//printf("\x1B[1m");
			//printf("\x1B[5m");
			
			if (j==-1)
			{
			//	 printf("%i",i);
			}else if ( Plateau[i][j].camp == joueur )
            {   
				if(strcmp(Plateau[i][j].name, "Trap") == 0)
                {
					if(Plateau[i][j].camp==1) printf("%s",KRED);
					if(Plateau[i][j].camp==3) printf("%s",KBLU);
					if(Plateau[i][j].camp==4) printf("%s",KMAG);
                    printf("X");
                    printf("%s",KNRM);
                }else
                {
					printf("\x1B[5m");
					if(Plateau[i][j].camp==1) printf("%s",KRED);
					if(Plateau[i][j].camp==3) printf("%s",KBLU);
					if(Plateau[i][j].camp==4) printf("%s",KMAG);
					if ( Plateau[i][j].orientation == up ) printf("^");
					if ( Plateau[i][j].orientation == left ) printf("<");
					if ( Plateau[i][j].orientation == right ) printf(">");
					if ( Plateau[i][j].orientation == down ) printf("v");
					printf("%s",KNRM);
				}
            }else if(Plateau[i][j].camp>0)
            {	
				if(strcmp(Plateau[i][j].name, "Trap") == 0)
                {
					printf(" ");
				}else
				{
					if(Plateau[i][j].camp==1) printf("%s",KRED);
					if(Plateau[i][j].camp==3) printf("%s",KBLU);
					if(Plateau[i][j].camp==4) printf("%s",KMAG);
					if ( Plateau[i][j].orientation == up ) printf("^");
					if ( Plateau[i][j].orientation == left ) printf("<");
					if ( Plateau[i][j].orientation == right ) printf(">");
					if ( Plateau[i][j].orientation == down ) printf("v");
					printf("%s",KNRM);
				}
            }else if ( Plateau[i][j].camp == -1 )
            {
                printf("%c",'#');
            }else if ( Plateau[i][j].camp == 0 )
            {
                printf(" ");
            }
        printf("%s",KNRM);
        }if (j!=-1) 
        {
			//printf("  %i",j);
			printf("\n");
		}else printf("\n");
    }

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
	generation_nom(chaine_tampon);
    Plateau[x][y]=character_default;
    if (camp>terrain) strcpy(Plateau[x][y].name, chaine_tampon);else strcpy(Plateau[x][y].name, "Mauvaise fonction de creation");
    Plateau[x][y].camp=camp;
    Plateau[x][y].position.X=x;
    Plateau[x][y].position.Y=y;
    
    Plateau[x][y].skill.b = (t_skill){"Placer Piege", 3, LAND, 10};
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


void generation_nom(char * nom)
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
	printf("Coordonnées de creation du personnage chaotique : %i , %i. Camp case : %i",x_buffer,y_buffer,Plateau[x_buffer][y_buffer].camp);
	creer_perso_rapide(sauvage,x_buffer,y_buffer);
	edit_stats(Plateau[x_buffer][y_buffer],20,20,10,10,30,30,10,20,5);
	
}
void tour_IA()
{
    int nb_actions_faites, i;
    int nb_perso_vivants=calcul_persos_IA();
    //printf("nb_persos_vivants:%i\n",nb_perso_vivants);
    t_skill skill_selected;
    for (i=0; i<nb_perso_vivants ; i++ )
    {	
		selected_character=Plateau[Valid_chars_IA[i].position.X][Valid_chars_IA[i].position.Y];
        if (selected_character.camp==joueur) //Vérifie que la case appartient à l'IA, en cas de mort d'un perso durant le tour d'un autre perso de l'IA, sinon contrôle d'une case terrain ou obstacle, ce qui serait gênant.
        {		
                for(nb_actions_faites=0;nb_actions_faites < selected_character.nbActionTour;nb_actions_faites++)
            {
				//printf("Plateau Checkpoint 4");
				afficher_plateau_orientation();
                deplacements_valides(); //Lololol, il y a un probleme ici !
                //printf("Plateau Checkpoint 5");
				afficher_plateau_orientation();
                deplacer_perso(choix_deplacement_IA());
                skill_selected = selected_character.skill.a;
                
                if(skill_selected.type != EMPTY)
                {
					//printf("Skill not EMPTY type.\n");
					
                    viser_case_valide(skill_selected);
                    appliquer_action(selected_character, choix_cible_IA(skill_selected), skill_selected);
                }else 
                {
					//printf("Skill EMPTY type.\n");
                    appliquer_action(selected_character, selected_character.position, skill_selected);
                }

            }
        }
    }


       


}

int calcul_persos_IA(){
    int i,j,nb_perso=0;
    init_char_table(Valid_chars_IA);
    for(i=0;i<TAILLE_MATRICE;i++)
    {
        for(j=0;j<TAILLE_MATRICE;j++)
        {
            if(Plateau[i][j].camp==joueur)
            {
                Valid_chars_IA[nb_perso]=Plateau[i][j];
                nb_perso++;
            }
        }
    }
    return nb_perso;
}

/**
* \fn int main()
* \brief Fonction principale
* Fonction principale qui permet de jouer en mode Kill'em'all.
*
*/
int main(){
	nb_joueurs++;
	srand((long)time(NULL));
	

	
	
	
	strcpy(player[1].name,"la nature");
	strcpy(player[2].name,"Arthur");
	strcpy(player[3].name,"Yann");
	strcpy(player[4].name,"Baptiste");
	
	int i;
	for (i=1;i<=nb_joueurs;i++)
	{
		player[i].alive=1;
	}
	player[1].alive=0;
	
	joueur=1;
	
	
	
	creer_perso_rapide(2,0,1);
	creer_perso_rapide(3,2,1);
	creer_perso_rapide(4,0,2);
	creer_terrain_rapide(obstacle,3,1);
        
    spawn_sauvage();
    
	players_life_check();
	for (i=1;i<=MaxTab;i++)
		{
			if(i==1)printf("%s",KRED);
			if(i==3)printf("%s",KBLU);
			if(i==4)printf("%s",KMAG);
			if(player[i].alive==1) printf("\nJoueur %i %s.\n",i,player[i].name);
			printf("%s",KNRM);
		}
		
    
    while(!all_dead_but_one(nb_joueurs)){
		
        if(player[joueur].alive != 0 )
        {   
			
			
            compteur_tour++;
            //if (generation_nombre_aleatoire(2)==1) spawn_sauvage();
            
            afficher_plateau_orientation();
            
            if (joueur==sauvage) 
            {
                tour_IA();
                
            }else tour_IA();
            
            
        }
        
        
        players_life_check();
        joueur_suivant(nb_joueurs);
    }
        
        
    /*    
    for (i=1;i<=nb_joueurs;i++)
	{
		if (player[i].alive==1) printf("Victoire de %s !\n",player[i].name);
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
		printf("occurences de %i : %i\n",i,statistiques_generation_random[i]);
	}
	*/
	
    return 1;
}
