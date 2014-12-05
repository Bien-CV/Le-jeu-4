
typedef struct { int X ; int Y ;}t_coord;
void initFile(); //initialisation la file à vide
void ajouterFile(t_coord a);	//ajoute la valeur en queue de file
void retirerFile(t_coord * val);	//retire la valeur en tête de file et l'affecte à val
int fileVide();	//délivre vrai si la file est vide, faux sinon
void afficher_queueFile();
void afficher_elt(t_coord val);
