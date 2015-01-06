#ifndef H_HEADER
#define H_HEADER
 
		#define TAILLE_MATRICE 5
		#define MaxTab 40
		#define MaxString 40
		//CODES ASCII
		#define bloc1 ((char)178)
		#define bloc2 ((char)219)
		#define bloc3 ((char)254)
		//struct Default //VALEURS PAR DEFAULT
		#define skill_attack ((t_skill){"Coup",1,ATK,1})
		#define skill_empty ((t_skill){"",0,EMPTY,0})
		#define skill_skip ((t_skill){"Passer le tour",0,EMPTY,0})
		#define coord_default ((t_coord){-1,-1})
		#define orientation_default (up)
		#define status_default ((t_status){30,30,10,10})
		#define status__terrain_default ((t_status){0,0,0,0})
		#define stats_default ((t_stats){10,10,10,10,2})
		#define stats_terrain_default ((t_stats){0,0,0,0,0})
		#define skill_list_default ((t_skill_list){ skill_attack,skill_empty,skill_empty,skill_empty,skill_empty,skill_skip})
		#define empty_skill_list ((t_skill_list){skill_empty,skill_empty,skill_empty,skill_empty,skill_empty,skill_empty})
		#define character_default ((t_character){"Default name", coord_default, orientation_default, status_default, stats_default, skill_list_default, sauvage, 1})
		#define default_targetOrientation ((t_targetOrientation)front)
		#define default_trap ((t_character){"Trap", coord_default, orientation_default, status_default, stats_default, skill_list_default, 1, 1})
		
		#define curseur (char)254
		
		#define case_terrain ((t_character){"",coord_default ,orientation_default ,status__terrain_default ,stats_terrain_default ,empty_skill_list, 0 , 0 })
		#define case_obstacle ((t_character){"",coord_default ,orientation_default ,status_default ,stats_default ,empty_skill_list, -1 , 0 })

		typedef enum {obstacle =-1,terrain=0, sauvage=1}t_camp;
		typedef enum {EMPTY,ATK,MATK,TRAP}t_skilltype;
		typedef enum {up,right,down,left}t_orientation;
		typedef enum {front, side, back}t_targetOrientation;
		typedef struct { int HP ; int Max_HP ; int MP ; int Max_MP ;} t_status ;
		typedef struct { int ATK ; int MATK ; int DEF ; int MDEF ; int MVT ;} t_stats;
		typedef struct { int X ; int Y ;}t_coord;
		typedef struct { char name[MaxString] ; int range ; t_skilltype type ; int damage_coeff ;} t_skill;
		typedef struct { t_skill a; t_skill b; t_skill c; t_skill d; t_skill e; t_skill wait; } t_skill_list;
		typedef enum {LAND_UNIT,AIR_UNIT,JUMPING_UNIT,TRAP_UNIT}t_character_type;
		typedef struct{char name[MaxString] ; t_coord position; t_orientation orientation ; t_status status ; t_stats stats ; t_skill_list skill; t_camp camp; int nb_action_tour; t_character_type type;} t_character ;
		
		typedef struct { int alive ; char name[MaxString] ;} t_player;
 
 
		
#endif


