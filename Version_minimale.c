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
#define skill_attack ((t_skill){"Coup",1,ATK,1})
#define skill_empty ((t_skill){"",0,EMPTY,0})
#define skill_skip ((t_skill){"Passer le tour",0,EMPTY,0})
#define coord_default ((t_coord){-1,-1})
#define orientation_default (up)
#define status_default ((t_status){100,100,10,10})
#define stats_default ((t_stats){10,10,10,10,2})
#define skill_list_default ((t_skill_list){ skill_attack,skill_empty,skill_empty,skill_empty,skill_empty,skill_skip})
#define empty_skill_list ((t_skill_list){skill_empty,skill_empty,skill_empty,skill_empty,skill_empty,skill_empty})
#define character_default ((t_character){"Default name", coord_default, orientation_default, status_default, stats_default, skill_list_default, player1, 1})
#define default_targetOrientation ((t_targetOrientation)front)

#define case_terrain ((t_character){"",coord_default ,orientation_default ,status_default ,stats_default ,empty_skill_list, 0 , 0 })

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
int life_check();
void JoueurSuivant(int nb_joueur);
int are_my_mates_alive();

t_character selected_character;
int player1_alive,player2_alive,player3_alive;
t_character Plateau[N][N]; 
t_coord depValid[N*N][3];
int nbDepValid;
t_camp joueur;

int indiceTabDepValid;



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
    
    for(i=0;i<nb_perso;i++){
        printf("Personnage n %i: (%i,%i) %s HP: %i\n",i+1,Valid_chars[i].position.X,Valid_chars[i].position.Y,Valid_chars[i].name,Valid_chars[i].status.HP);
    }
    
    while(choix_fait){
        printf("Quel personnage voulez-vous selectionner ? \nPersonnage n ");
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
        int choix;
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
        return skill_empty;
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
* \fn void OrienterPerso(t_character perso)
* \brief Propose une liste des orientations du perso indiqué en paramètre d'entrée et change son orientation.
* 
*
*/
void OrienterPerso(t_character perso){
    int choix=1;
    char input;
    
    while(choix)
    {
        printf("z: Nord\nd: Est\ns: Sud\nq: Ouest\nchoisissez l'orentation : ");
        scanf("%c",&input);
        switch(input)
        {
            case 'z':perso.orientation=0;choix=0;break;
            case 'd':perso.orientation=1;choix=0;break;
            case 's':perso.orientation=2;choix=0;break;
            case 'q':perso.orientation=3;choix=0;break;
            default:printf("Orientation incorrect");break;
        }
    }
}


/**
* \fn void JoueurSuivant(int nb_joueur)
* \brief Prend en paramètre le nombre de joueurs et incrémente le numéro de joueur de façon à ne pas dépasser le nombre de joueurs.
*
*/
void JoueurSuivant(int nb_joueur){
    if (joueur==nb_joueur) joueur=1;
        else joueur++;
       
}


/**
* \fn int are_my_mates_alive(t_camp joueur)
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
	if (are_my_mates_alive()<1){
		printf("Joueur %i ",joueur); 
		if (joueur==player1)
		{player1_alive=0;
			printf("mort.\n");
			}else if (joueur==player2)
		{player2_alive=0;
			printf("mort.\n");
			}else if (joueur==player3)
		{player3_alive=0;
			printf("mort.\n");
			}
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
    int choix;
	printf("\nJOUEUR %i \n",joueur);
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


    if (choix==1){   // Se déclenche après sélection d'un personnage, c'est la suite d'actions liées au personnage sélectionné
            SelectSkill();//selectionne l’action que le joueur souhaite effectuer (exple: SelectPerso / PasserTour…) retourne le n° de l’action à effectuer
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
	player1_alive=1,player2_alive=1,player3_alive=1;
	
	
joueur=player1;
Plateau[0][1]=character_default;
strcpy(Plateau[0][1].name, "Arthur");
Plateau[0][1].position.X=0;
Plateau[0][1].position.Y=1;
Plateau[0][1].camp=player2;

Plateau[7][4]=character_default;
strcpy(Plateau[7][4].name, "Yann");
Plateau[7][4].position.X=7;
Plateau[7][4].position.Y=4;
Plateau[7][4].camp=player3;

Plateau[2][3]=character_default;
strcpy(Plateau[2][3].name, "Baptiste");
Plateau[2][3].position.X=2;
Plateau[2][3].position.Y=3;
Plateau[2][3].camp=player1;

    while(!all_dead_but_one()){
		if(life_check()!=0)tour(joueur);
        JoueurSuivant(3);
    }
    if (player1_alive == 1 ) printf("Victoire du joueur 1 !\n");
    if (player2_alive == 1 ) printf("Victoire du joueur 2 !\n");
    if (player3_alive == 1 ) printf("Victoire du joueur 3 !\n");

	
    return 1;
}
