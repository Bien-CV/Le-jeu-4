#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "header.h"
#include "liste.c"
#include "file.c"

//Prototypes fonctions, à inclure dans un futur Minimale.h !
int life_check();
void JoueurSuivant(int nb_joueur);
void creer_terrain_rapide(t_camp camp,int x, int y);
void viderBuffer(void);
int are_my_mates_alive();
void generation_nom(char * nom);


char PartieNom[][20] = 
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
int player1_alive,player2_alive,player3_alive;
t_character Plateau[N][N];
t_coord depValid[N*N][3];
int nbDepValid;
int nbAtkValid;
t_camp joueur;
t_player player[MaxTab];



int indiceTabDepValid;




/**
 * \fn int NombreAleatoire(int max)
 * \brief Fonction qui renvoi un nombre aléatoire en 0 et 'max'
 * 
*/
int NombreAleatoire(int max)
{
		return (rand()%max+1);
}

/**
 * \fn void DeplacerPerso(t_coord case_perso)
 * \brief Déplace le personnage sur le terrain
 * 
 */
void DeplacerPerso(t_coord case_perso)
{
    Plateau[selected_character.position.X][selected_character.position.Y]= case_terrain ;
    selected_character.position.X=case_perso.X;
    selected_character.position.Y=case_perso.Y;
    Plateau[case_perso.X][case_perso.Y]= selected_character;

}

/**
 * \fn int CasesVoisines(t_coord coordonnees)
 * \brief Renvoi le nombre de case voisine vide
 * 
 */
int CasesVoisines(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
    if(Plateau[coordonnees.X+1][coordonnees.Y].camp == 0)
    {
        coord = coordonnees;
        coord.X += 1;
        if(coord.X >= 0 && coord.X <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X-1][coordonnees.Y].camp == 0)
    {
        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X][coordonnees.Y+1].camp == 0)
    {
        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    if(Plateau[coordonnees.X][coordonnees.Y-1].camp == 0)
    {
        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }
    }
    return(nbVois);
}

/**
 * \fn void DeplacementsValides()
 * \brief Calcule les positions de déplacement valide
 * 
 */
void DeplacementsValides(){// permet de retourner /* Commentaire de Baptiste : Calculer plutôt non ? */ au joueur les positions valides pour son perso.
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
            retirerFile(&coordonnees);
            ajout_droit(coordonnees);
            if(mvtEffectue < selected_character.stats.MVT)
                nbVoisins += CasesVoisines(coordonnees);
        }
        mvtEffectue++;
    }
    suppr_doublon();
    nbDepValid = calculerElemListe();
}

/**
 * \fn t_coord ChoixDeplacement()
 * \brief Permet au joueur de choisir la destination
 * 
 */
t_coord ChoixDeplacement(){//Reprendre le code de choix de SelecPerso
    int i = 0, j, choix = 1;
    viderBuffer();
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

/**
 * \fn void init_Plateau()
 * \brief Initialise le plateau
 * 
 */
void init_Plateau()
{
    int i, j;
    for(j = 0; j < N; j++)
    {
        for(i = 0; i < N; i++)
        {
			creer_terrain_rapide(terrain,i,j);
        }
    }
}

/**
 * \fn int CasesVoisinesATK(t_coord coordonnees)
 * \brief 
 * 
 */
int CasesVoisinesATK(t_coord coordonnees){
    int nbVois = 0;
    t_coord coord;
        coord = coordonnees;
        coord.X += 1;

        if(coord.X >= 0 && coord.X <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.X -= 1;

        if(coord.X >= 0 && coord.X <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y += 1;

        if(coord.Y >= 0 && coord.Y <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

        coord = coordonnees;
        coord.Y -= 1;

        if(coord.Y >= 0 && coord.Y <= N-1)
        {
            ajouterFile(coord);
            nbVois++;
        }

    return(nbVois);
}

/**
 * \fn void viserCaseValid(t_skill skill)
 * \brief Calcule les cases valides ciblables
 * 
 */
void viserCaseValid(t_skill skill)
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
                nbVoisins += CasesVoisinesATK(coordonnees);
        }
        mvtEffectue++;
    }
    suppr_doublon();
    nbAtkValid = calculerElemListe();
}

/**
 * \fn t_coord choixCible(t_skill skill)
 * \brief Permet au joueur de choisir une cible
 * 
 */
t_coord choixCible(t_skill skill)
{
    int i =0;
    int choix, choix_fait =1;
    t_camp tampon_camp_cible;
    viderBuffer();
    t_coord coordonnees, choisi;
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
			printf(" - %s, joueur%i.",player[tampon_camp_cible].name,tampon_camp_cible);
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

    while(choix_fait){
        
        scanf("%i",&choix);
        if(choix<=nbAtkValid && choix > 0)
        {
            en_tete();
            for(i = 0; i < choix-1; i++)
                suivant();
            valeur_elt(&choisi);
            coordonnees.X=choisi.X;
            coordonnees.Y=choisi.Y;
            choix_fait=0;
        }else{printf("Numero incorrect");}
    }
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
* \fn void SelectPerso()
* \brief Fonction qui propose la liste des personnages pouvant être sélectionnés.
*
*/
void SelectPerso(){
    int i,j,nb_perso=0,choix,choix_fait=1;
    t_character Valid_chars[MaxTab];
    init_char_table(Valid_chars);
    t_character perso_selectionne;
    viderBuffer();
    printf("Quel personnage voulez-vous selectionner ? \n");
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(Plateau[i][j].camp==joueur)
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
* \fn void Suicide()
* \brief Fonction permetant au joueur courant d'abandonner la partie
*
*/
void Suicide(){// Permet au joueur de quitter la partie.
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
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
    if(skill.type==EMPTY) ;
    printf("\n");
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
t_skill SelectSkill(){//selectionne le skill que le personange courant effectuera, l'execution du skill est la fonction Skill.
        int choix;
        viderBuffer();
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
* \fn t_targetOrientation GetTargetOrientation (t_character perso, t_coord cible )
* \brief Fonction déterminant quelle est l'orientation de la cible par rapport au joueur.
* 
*/
t_targetOrientation GetTargetOrientation (t_character perso, t_coord cible ){
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
* \fn void Action(t_character lanceur, t_coord cible, t_skill action)
* \brief Fonction appliquant le skill du personnage lanceur à la case cible.
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void Action(t_character lanceur, t_coord cible, t_skill action){
    int total_dmg;
    t_targetOrientation targetOrientation= GetTargetOrientation(lanceur,cible);
    int coefOrientation;
    //typedef struct { char name[20] ; int range ; t_type type ; int damage_coeff ;} t_skill;
    //typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
    //typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
    //typedef enum {EMPTY,ATK,MATK}t_type;
    if(targetOrientation==0) coefOrientation=1;
    if(targetOrientation==1) coefOrientation=1.5;
    if(targetOrientation==2) coefOrientation=2;
    
    if (action.type == MATK){
        total_dmg=action.damage_coeff*lanceur.stats.MATK; //degats avant réduction
        if(total_dmg>0) total_dmg*=(abs(Plateau[cible.X][cible.Y].stats.MDEF-100)/100)*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == ATK){
        total_dmg=action.damage_coeff*lanceur.stats.ATK; //degats avant réduction
        if(total_dmg>0) total_dmg*=(abs(Plateau[cible.X][cible.Y].stats.DEF-100)/100)*coefOrientation; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == EMPTY){
    }
    if (Plateau[cible.X][cible.Y].status.HP< 0) Plateau[cible.X][cible.Y].status.HP=0;
    if (Plateau[cible.X][cible.Y].status.HP>Plateau[cible.X][cible.Y].status.Max_HP) Plateau[cible.X][cible.Y].status.HP=Plateau[cible.X][cible.Y].status.Max_HP;
    if (Plateau[cible.X][cible.Y].status.HP<= 0) Plateau[cible.X][cible.Y]=case_terrain;
}

/**
* \fn void viderBuffer(void)
* \brief Vide le tampon de saisie clavier.
* 
*
*/
void viderBuffer(void)
{
  int c;
  while((c=getchar()) != EOF && c != '\n');
 
}

/**
* \fn void OrienterPersoNumpad(t_character perso)
* \brief Propose une liste numérique des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void OrienterPersoNumpad(){
    int choix=1;
    char input;
    t_orientation tampon_orientation;
    printf("Choisissez l'orientation : \n");
    viderBuffer();
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
* \fn void OrienterPerso(t_character perso)
* \brief Propose une liste des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void OrienterPerso(){
    int choix=1;
    char input;
    t_orientation buffer;
    printf("Choisissez l'orientation : \n");
    viderBuffer();
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
* \fn void JoueurSuivant(int nb_joueur)
* \brief Prend en paramètre le nombre de joueurs et incrémente le numéro de joueur de façon à ne pas dépasser le nombre de joueurs.
*
*/
void JoueurSuivant(int nb_joueur)
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
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(Plateau[i][j].camp==joueur)
            {
                nb_perso++;
            }
        }
    }
    return nb_perso;
}

/**
* \fn int life_check(t_camp joueur)
* \brief Fonction qui vérifie si un joueur a encore des personnages vivants sur le terrain.
*    Appelle are_my_mates_alive() pour déterminer le joueur est encore en jeu.
*    Renvoie 1 si le joueur a encore des personnages en vie, sinon 0.
*/
int life_check(){
    if (player[joueur].alive==0 || are_my_mates_alive()<1 ){
		player[joueur].alive=0;
        printf("%s est mort.\n",player[joueur].name);
        return 0;
    }else {
        return 1;
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
    viderBuffer();
    int choix, nb_actions_faites = 0, deplacement_fait;
    t_skill skill_selected;
    
        do                                            //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
        {   
			printf("\nTour de %s : \n",player[joueur].name);
            printf("   1 - Selectionner Perso\n");
            printf("   2 - Passer tour\n");
            printf("   3 - Suicide\n");
            scanf("%d",&choix);

            switch(choix)
            {   case 1: printf("\n");SelectPerso(); break;
                case 2: printf("\n");PasserTour(); break;
                case 3: printf("\n");Suicide();life_check(); break;//Abandon
                default: printf("Erreur: votre choix doit être compris entre 1 et 3\n");
            }
        }
        while ((choix<1) ||(choix>3) );

	
		
    if (choix==1){   // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné
		
        //skill_selected.type = ATK;	WTF Nigga ?
        while(nb_actions_faites < selected_character.nbActionTour /* && skill_selected.type != EMPTY*/)
        {
            if(deplacement_fait != 1)
            {
                DeplacementsValides();
                DeplacerPerso(ChoixDeplacement());
                deplacement_fait = 1;
            }
            skill_selected = SelectSkill();//selectionne l’action que le joueur souhaite effectuer (exple: SelectPerso / PasserTour…) retourne le n° de l’action à effectuer
            if(skill_selected.type != EMPTY)
            {
                viserCaseValid(skill_selected);
                Action(selected_character, choixCible(skill_selected), skill_selected);
            }else 
            {
				Action(selected_character, selected_character.position, skill_selected);
			}
            nb_actions_faites++;
            OrienterPersoNumpad();
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
    int compteur_joueurs_vivants=0,i;
    for (i=1;i<=nb_joueurs;i++)
	{
		compteur_joueurs_vivants+=player[i].alive;
	}
	if (compteur_joueurs_vivants> 1)return 0;
    return 1;
}



/**
* \fn void afficher_plateau_orientation(void)
* \brief Affiche le plateau avec les caractères correspondants à l'orientation
*
*/
void afficher_plateau_orientation(void){
    int i,j;
    for(j=-1;j<N;j++)
    {
        for(i=0;i<N;i++)
        {
			if (j==-1)
			{
				 printf("%i",i);
			}else if ( Plateau[i][j].camp == joueur )
            {
                printf("%c",'O');
            }else if(Plateau[i][j].camp>0)
            {
                if ( Plateau[i][j].orientation == up ) printf("^");
                if ( Plateau[i][j].orientation == left ) printf("<");
                if ( Plateau[i][j].orientation == right ) printf(">");
                if ( Plateau[i][j].orientation == down ) printf("v");
            }else if ( Plateau[i][j].camp == -1 )
            {
                printf("%c",'#');
            }else if ( Plateau[i][j].camp == 0 )
            {
                printf(" ");
            }
        }if (j!=-1) {printf("  %i\n",j);} else printf("\n");
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
		strcat(chaine_tampon, "obstacle");
		strcat(chaine_tampon, PartieNom[NombreAleatoire(17)]);
		strcpy(Plateau[x][y].name, chaine_tampon);
	} else if (camp==terrain) 
	{
		Plateau[x][y]=case_terrain;
		strcat(chaine_tampon, "terrain");
		strcat(chaine_tampon, PartieNom[NombreAleatoire(17)]);
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
	for(i=NombreAleatoire(1)+1;i>0;i--)
	{
	strcat(random_name, PartieNom[NombreAleatoire(17)]);
	strcat(random_name, " ");
	}
	random_name[0]=toupper(random_name[0]);
	
	strcpy(nom,random_name);
	free(random_name);
}
/**
* \fn int main()
* \brief Fonction principale
* Fonction principale qui permet de jouer en mode Kill'em'all.
*
*/
int main(){

	srand((long)time(NULL));
	

	
	
	
	strcpy(player[1].name,"Baptiste");
	strcpy(player[2].name,"Arthur");
	strcpy(player[3].name,"Yann");

	
	generation_nom(player[1].name);
	generation_nom(player[1].name);
	
	int nb_joueurs=3,i;
	for (i=1;i<=nb_joueurs;i++)
	{
		player[i].alive=1;
	}
	
	joueur=player1;
	
	
	creer_perso_rapide(player1,0,2);
	creer_perso_rapide(player2,0,1);
	creer_perso_rapide(player3,7,4);
	creer_terrain_rapide(obstacle,3,4);

    while(!all_dead_but_one(nb_joueurs)){
        if(life_check()!=0)
        {   afficher_plateau_orientation();
            tour();
        }
        JoueurSuivant(nb_joueurs);
    }
    for (i=1;i<=nb_joueurs;i++)
	{
		if (player[i].alive==1) printf("Victoire de %s !\n",player[i].name);
	}

    return 1;
}
