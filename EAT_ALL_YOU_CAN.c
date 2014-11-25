//Actions de base:
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#define N 10;
#define MaxTab 40;
//CODES ASCII
#define bloc1 178;
#define bloc2 219;
#define bloc3 254;

typedef enum {obstacle =-1,terrain=0, player=1, opponent=2}t_camp:
typedef enum {EMPTY,ATK,MATK}t_type;
typedef enum {up,right,down,left}t_orientation:
typedef enum {front, side, back}t_targetOrientation:
typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ; 
typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
typedef struct {  int X ; int Y ;}t_coord; 
typedef struct { char name[20] ; int range ; t_type type ; int damage_coeff ;} t_skill;
typedef struct { t_skill a; t_skill b; t_skill c; t_skill d; t_skill e; t_skill wait; } t_skill_list;

typedef struct{char name[20] ; t_coord position; t_orientation orientation ; t_status status ; t_stats stats ; t_skill_list skill; t_camp camp; int nbActionTour} t_character ;

//VALEURS PAR DEFAULT
t_skill skill_attack= {"Coup",1,ATK,1};

t_skill skill_empty= {"",0,EMPTY,0};

t_skill skill_skip= {"Passer le tour",0,EMPTY,0};

t_coord coord_default={-1,-1};

t_orientation orientation_default=up;

t_status status_default={100,100,10,10};

t_stats stats_default{10,10,10,10,2};

t_skill_list skill_list_default={ skill_attack,skill_empty,skill_empty,skill_empty,skill_empty,skill_skip};

t_character character_default={"Default name",coord_default,orientation_default,status_default,stats_default,skill_list_default,player,1};
//FIN VALEURS PAR DEFAULT

t_character selected_character;
t_character Plateau[N][N]; 
t_coord depValid[N*N];
int nbDepValid;

int indiceTabDepValid;


t_character SelectPerso(){// Propose la liste des personnages pouvant être sélectionnés.
    int i,j,nb_perso=0,choix=1;
    t_character Valid_chars[MaxTab];
    init_char_table(Valid_chars[MaxTab]);
    t_character perso_selectionne;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(Plateau[i][j]->camp==joueur){
                Valid_chars[nb_perso]=Plateau[i][j];
                nb_perso;
            }
        }
    }
    for(i=0;i<=nb_perso;i++){
        printf("Personnage n %i: (%i,%i) %s\n",i+1,Valid_chars[i]->position->X,Valid_chars[i]->position->Y,Valid_chars[i]->name);
    }
    
    while(choix){
        printf("Quelle personnage voulez-vous selectionner ? \nPersonnage n ");
        scanf("%i",&j);
        if(j<nb_perso && j > 0)
        {
            perso_selectionne=Validchars[j-1];
            choix=0;
        }else{printf("Numero incorrect");}
    }
    selected_character=perso_selectionne; //selected_character est la variable globale correspondant au perso sélectionné
}

void init_char_table(t_character Valid_chars[MaxTab]){
    int i;
    for(i=0;i<MaxTab;i++){
        Valid_chars[i]=character_default;
    }
}

void PasserTour(){//Permet au joueur de passer son tour.
    
}

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
void CasesVoisines(int L,){
    int indiceValid = indiceTabDepValid;
    while(depValid[indiceValid][2] == L)
    {
        for(int indice1 = depValid[indiceValid][0]-1; indice1 <= depValid[indiceValid][0]+1; indice1++)
        {
            for(int indice2 = depValid[indiceValid][1]-1; indice2 = depValid[indiceValid][1]+1; indice2++)
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


void DeplacementsValides(t_character perso){// permet de retourner au joueur les positions valides pour sont perso.
    int L; indice1;
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

t_coord ChoixDeplacement(){//Reprendre le code de choix de SelecPerso
    int i; j; choix = 1;
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


//Action des Perso:
int Action(t_character attaquant, t_coord caseAtk, t_skill action){
    
}


t_targetOrientation GetTargetOrientation (t_character perso, t_character enemy ){
    
}


void DeplacerPerso(t_character perso, t_coord case){// met le perso sur la case “case”.
     
}


void OrienterPerso(t_character perso){// Propose une liste des orientations du perso.
    
}

void JoueurSuivant(int nb_joueur, t_camp* joueur){
    if (joueur==nb_joueur) joueur=1;
        else joueur++;
}

void tour(t_camp joueur)
{
    int choix;
    
    //Sélection de perso ou passer tour ou abandon
        do
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


    if (choix==1){
        
        //Si DeplacementsValides return un coord DeplacerPerso(selected_character,DeplacementsValides(selected_character));
        DeplacementsValides(selected_character);
        DeplacerPerso(selected_character,ChoixDeplacement());
        while("compteur d'actions du perso" >0)
        {
            int SelectSkill(){//selectionne l’action que le joueur souhaite effectuer (exple: SelectPerso / PasserTour…) retourne le n° de l’action à effectuer
            int Skill(t_character attaquant, t_coord caseAtk, t_skill action){
            "Décrémentation compteur d'actions";
        }
        Orientation ;
        }
}

int main(){
t_camp joueur=player;
    while(!victoire){
        tour(joueur);
        JoueurSuivant(2,&joueur);
    }


}
