//Actions de base:
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#define N 10
#define MaxTab 40
//CODES ASCII
#define bloc1 178
#define bloc2 219
#define bloc3 254
//struct Default //VALEURS PAR DEFAULT
#define skill_attack {"Coup",1,ATK,1}
#define skill_empty {"",0,EMPTY,0}
#define skill_skip {"Passer le tour",0,EMPTY,0}
#define coord_default {-1,-1}
#define orientation_default up
#define status_default {100,100,10,10}
#define stats_default {10,10,10,10,2}
#define skill_list_default { skill_attack,skill_empty,skill_empty,skill_empty,skill_empty,skill_skip}
#define character_default {"Default name", coord_default, orientation_default, status_default, stats_default, skill_list_default, player1, 1}


#define case_terrain {"";coord_default ;orientation_default ;status_default ;stats_default ;skill_empty ; 0 ; 0 ;}

typedef enum {obstacle =-1,terrain=0, player1=1, player2=2, player3=3}t_camp;
typedef enum {EMPTY,ATK,MATK}t_type;
typedef enum {up,right,down,left}t_orientation;
typedef enum {front, side, back}t_targetOrientation;
typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
typedef struct { int X ; int Y ;}t_coord;
typedef struct { char name[20] ; int range ; t_type type ; int damage_coeff ;} t_skill;
typedef struct { t_skill a; t_skill b; t_skill c; t_skill d; t_skill e; t_skill wait; } t_skill_list;
typedef struct{char name[20] ; t_coord position; t_orientation orientation ; t_status status ; t_stats stats ; t_skill_list skill; t_camp camp; int nbActionTour;} t_character ;

//FIN VALEURS PAR DEFAULT

t_character selected_character;
int player1_alive,player2_alive,player3_alive;
t_character Plateau[N][N]; 
t_coord depValid[N*N][3];
int nbDepValid;

int indiceTabDepValid;



/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void init_char_table(t_char chars[])
{

}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void SelectPerso(){// Propose la liste des personnages pouvant être sélectionnés.
    int i,j,nb_perso=0,choix=1;
    t_character Valid_chars[MaxTab];
    init_char_table(Valid_chars[MaxTab]);
    t_character perso_selectionne;
    
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
    
    for(i=0;i<=nb_perso;i++){
        printf("Personnage n %i: (%i,%i) %s\n",i+1,Valid_chars[i].position.X,Valid_chars[i].position.Y,Valid_chars[i].name);
    }
    
    while(choix){
        printf("Quelle personnage voulez-vous selectionner ? \nPersonnage n ");
        scanf("%i",&j);
        if(j<nbDepValid && j > 0)
        {
            perso_selectionne=Validchars[j-1];
            choix=0;
        }else{printf("Numero incorrect");}
    }
    selected_character=perso_selectionne; //selected_character est la variable globale correspondant au perso sélectionné
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void init_char_table(t_character Valid_chars[MaxTab]){
    int i;
    for(i=0;i<MaxTab;i++){
        Valid_chars[i]=character_default;
    }
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void PasserTour(){//Permet au joueur de passer son tour.
    
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void Suicide(){// Permet au joueur de quitter la partie.
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(Plateau[i][j]->camp==joueur){
            Plateau[i][j]->status.HP=0;
            }
        }
    }
}
// à tester: CasesVoisines + DeplacementsValides

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void CasesVoisines(int L){
    int indiceValid = indiceTabDepValid;
    int indice1, indice2;
    while(depValid[indiceValid][2] == L)
    {
        for(indice1 = depValid[indiceValid][0]-1; indice1 <= depValid[indiceValid][0]+1; indice1++)
        {
            for(indice2 = depValid[indiceValid][1]-1; indice2 = depValid[indiceValid][1]+1; indice2++)
            {
                if(Plateau[indice1][indice2].camp == 0)
                {
                    indiceTabDepValid++;
                    depValid[indiceTabDepValid][0] = indice1;
                    depValid[indiceTabDepValid][1] = indice2;
                    depValid[indiceTabDepValid][2] = L+1;
                    nbDepValid++;
                }
            }
        }
        indiceValid++;
    }
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void DeplacementsValides(t_character perso){// permet de retourner au joueur les positions valides pour sont perso.
    int L, indice1;
    indiceTabDepValid = 0;
    for(indice1 = 0; indice1 < N*N; indice1++){
        depValid[indice1][0] = -1;
        depValid[indice1][1] = -1;
        depValid[indice1][2] = -1;
    }
    depValid[0][0] = perso.position.X;
    depValid[0][1] = perso.position.Y;
    depValid[0][2] = 0;
    nbDepValid = 1;
    for(L = 0; L < deplacement; L++)
    {
        CasesVoisines(L);
    }
}

/**
* \fn void init_char_table(t_char chars[])
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
t_coord ChoixDeplacement(){//Reprendre le code de choix de SelecPerso
    int i, j, choix = 1;
    t_coord coordonnees;
    for(i=0;i<= nbDepValid;i++){
        printf("Deplacement n %i: (%i,%i)\n",i+1, depValid[i][0], depValid[i][1]);
    }
    
    while(choix){
        printf("Quelle Déplacement voulez-vous effectuer ? \nDéplacement n ");
        scanf("%i",&j);
        if(j<nb_perso && j > 0)
        {
            coordonnees.X=depValid[j-1][0];
            coordonnees.Y=depValid[j-1][1];
            choix=0;
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
void afficher_skill(int skill_nb, t_skill skill){
    printf("%i- %s ",skill_nb,skill.name);
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
        do
        {   
            afficher_skill(1,selected_character.skill.a);
            afficher_skill(2,selected_character.skill.b);
            afficher_skill(3,selected_character.skill.c);
            afficher_skill(4,selected_character.skill.d);
            afficher_skill(5,selected_character.skill.e);
            afficher_skill(6,selected_character.skill.wait);
            printf("Votre choix : ");
            scanf("%d",&choix);
    
            switch(choix)
            {   case 1: return selected_character.skill.a;break;
                case 2: return selected_character.skill.b;break;
                case 3: return selected_character.skill.c;break;
                case 4: return selected_character.skill.d;break;
                case 5: return selected_character.skill.e;break;
                case 6: return selected_character.skill.wait;break;
  default: printf("Erreur: votre choix doit être compris entre 1 et 6\n");
            }
        }
        while ((choix<1) ||(choix>6) );
}


/**
* \fn void Action(t_character lanceur, t_coord cible, t_skill action)
* \brief Fonction appliquant le skill du personnage lanceur à la case cible.
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
void Action(t_character lanceur, t_coord cible, t_skill action){
    int total_dmg;
    //typedef struct { char name[20] ; int range ; t_type type ; int damage_coeff ;} t_skill;
    //typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
    //typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
    //typedef enum {EMPTY,ATK,MATK}t_type;
    if (action.type == MATK){
        total_dmg=action.damage_coeff*lanceur.stats.MATK; //degats avant réduction
        if(total_dmg<0) total_dmg*=abs(Plateau[cible.X][cible.Y].stats.MDEF-100)/100; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == ATK){
        total_dmg=action.damage_coeff*lanceur.stats.ATK; //degats avant réduction
        if(total_dmg<0) total_dmg*=abs(Plateau[cible.X][cible.Y].stats.DEF-100)/100; // réduction des dégâts, ignore les soins. Les soins sont des dégâts négatifs.
        Plateau[cible.X][cible.Y].status.HP -= total_dmg;
    }
    if (action.type == EMPTY){
    }
    if (Plateau[cible.X][cible.Y].status.HP< 0) Plateau[cible.X][cible.Y].status.HP=0;
    if (Plateau[cible.X][cible.Y].status.HP>Plateau[cible.X][cible.Y].status.Max_HP) Plateau[cible.X][cible.Y].status.HP=Plateau[cible.X][cible.Y].status.Max_HP;
}

/**
* \fn t_targetOrientation GetTargetOrientation (t_character perso, t_character cible )
* \brief Fonction déterminant quelle est l'orientation de la cible par rapport au joueur.
* 
*
*/
t_targetOrientation GetTargetOrientation (t_character perso, t_character cible ){
    int xperso,yperso,xenemy,yenemy,oenemy;
    xperso=perso.position.X;
    yperso=perso.position.Y;
    xenemy=enemy.position.X;
    yenemy=enemy.position.Y;
    oenemy=enemy.orientation;
    
    if((xenemy+yenemy<=xperso-yperso)&&(xenemy+yenemy<=xperso+yperso))
    {
        
    }
    else if((xenemy+yenemy<=xperso-yperso)&&(xenemy+yenemy>=xperso+yperso))
    {
        
    }
    else if((xenemy+yenemy>=xperso-yperso)&&(xenemy+yenemy>=xperso+yperso))
    {
        
    }
    else if((xenemy+yenemy>=xperso-yperso)&&(xenemy+yenemy<=xperso+yperso))
    {
        
    }
}

/**
* \fn void DeplacerPerso(t_character perso, t_coord case)
* \brief Fonction de déplacement d'un personnage sur une case valide.
*  Aux coordonnées du personnage, transforme la case correspondante du plateau en case de terrain.
*  Modifie les coordonnées du personnage.
*  Transforme la case du plateau aux coordonnées indiquées en le personnage entré en paramètre.
*/
void DeplacerPerso(t_character perso, t_coord case){
     
     Plateau[perso.position.X][perso.position.Y]=case_terrain;
     perso.position.X=case.X;
     perso.position.Y=case.Y;
     Plateau[case.X][case.Y]=perso;
     
}

/**
* \fn void OrienterPerso(t_character perso)
* \brief Propose une liste des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void OrienterPerso(t_character perso){// 
    int choix=1;
    char input;
    
    while(choix)
    {
        printf("z: Nord\nd: Est\ns: Sud\nq: Ouest\nchoisissez l'orentation : ");
        scanf("%c",&input);
        switch(input)
        {
            case 'z':perso.orentation=0;choix=0;break;
            case 'd':perso.orentation=1;choix=0;break;
            case 's':perso.orentation=2;choix=0;break;
            case 'q':perso.orentation=3;choix=0;break;
            default:printf("Orientation incorrect");break;
        }
    }
}


/**
* \fn void JoueurSuivant(int nb_joueur, t_camp joueur)
* \brief Fonction faisant passer la variable de joueur appartenant à la fonction principale au joueur suivant.
* Prend en paramètre le nombre de joueurs et le joueur actuel, incrémente de façon à ne pas dépasser le nombre de joueurs.
*
*/
void JoueurSuivant(int nb_joueur, t_camp* joueur){
    if (*joueur==nb_joueur) *joueur=1;
        else (*joueur)++;
}


/**
* \fn int are_my_mates_alive(t_camp joueur)
* \brief Fonction d'initialisation d'un tableau de personnages
* Remplit le tableau de personnages entré en paramètre de cases de terrain.
*
*/
int are_my_mates_alive(t_camp joueur){
    int nb_perso;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(Plateau[i][j]->camp==joueur)
            {
                nb_perso++;
            }
        }
    }
    assert(nb_perso>0);
    if (nb_perso>0){
    return 1;
    }else return 0;
}

/**
* \fn int life_check(t_camp joueur)
* \brief Fonction qui vérifie si un joueur a encore des personnages vivants sur le terrain.
*    Appelle are_my_mates_alive(t_camp joueur) pour déterminer le joueur est encore en jeu.
*    Renvoie 1 si le joueur a encore des personnages en vie, sinon 0.
*/
int life_check(t_camp joueur){


    if (joueur==player1)
    {
        player1_alive=are_my_mates_alive(joueur);
        assert(player1_alive==1 || player1_alive==0);
        if (player1_alive == 1)
        { return 1;
        }else if (player1_alive == 0 )
        { return 0;
        }
    }else if (joueur==player2)
    {
        player2_alive=are_my_mates_alive(joueur);
        assert(player2_alive==1 || player2_alive==0);
        if (player2_alive == 1)
        { return 1;
        }else if (player2_alive == 0 )
        { return 0;
        }
    }
    }else if (joueur==player3)
    {
        player3_alive=are_my_mates_alive(joueur);
        assert(player3_alive==1 || player3_alive==0);
        if (player3_alive == 1)
        { return 1;
        }else if (player3_alive == 0 )
        { return 0;
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
void tour(t_camp joueur)
{
    int choix;
    if (life_check(joueur) == 0) return;            //Vérifie si le joueur n'a plus de personnages, si oui, termine le tour

        do                                            //Propose de sélectionner un personnager et le jouer, passer le tour, ou de se suicider.
        {   printf("\nMenu :\n");
            printf(" 1 - Selectionner Perso\n");
            printf(" 2 - Passer tour\n");
            printf(" 3 - Suicide\n");
            printf("Votre choix : ");
            scanf("%d",&choix);
    
            switch(choix)
            {   case 1: SelectPerso();break;
                case 2: PasserTour(); break;
                case 3: Suicide(); break;//Abandon
                default: printf("Erreur: votre choix doit être compris entre 1 et 3\n");
            }
        }
        while ((choix<1) ||(choix>3) );


    if (choix==1){                                // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné.
        
        //Si DeplacementsValides return un coord DeplacerPerso(selected_character,DeplacementsValides(selected_character));
        DeplacementsValides(selected_character);
        DeplacerPerso(selected_character,ChoixDeplacement());
        while("compteur d'actions du perso" >0)
        {
            int SelectSkill(){//selectionne l’action que le joueur souhaite effectuer (exple: SelectPerso / PasserTour…) retourne le n° de l’action à effectuer
            int Skill(t_character attaquant, t_coord caseAtk, t_skill action){
            "Décrémentation compteur d'actions";
        }
        OrienterPerso(selected_character);
        }
}

/**
* \fn int all_dead_but_one(void)
* \brief Fonction déterminant si un seul joueur est vivant.
*    Appellé par la fonction principale pour déterminer si c'est la fin de la partie.
*    Renvoie 0 si plus d'un joueur survit, sinon renvoie 1.
*/
int all_dead_but_one(void){
    if ((player1_alive+player2_alive+player3_alive) > 1) return 0;
    return 1;
}

/**
* \fn int main()
* \brief Fonction principale
* Fonction principale qui permet de jouer en mode Kill'em'all.
*
*/
int main(){
t_camp joueur=player1;

    while(!all_dead_but_one()){
        tour(joueur);
        JoueurSuivant(2,&joueur);
    }
    assert((player1_alive+player2_alive+player3_alive) == 1);
    if (player1_alive == 1 ) printf("Victoire du joueur 1 !");
    if (player2_alive == 1 ) printf("Victoire du joueur 2 !");
    if (player3_alive == 1 ) printf("Victoire du joueur 3 !");



}
