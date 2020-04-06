#ifndef AI_H
#define AI_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#include "Field.h"

#define BLOCK_COST 4

/**
 * Une position correspondant à une case.
 * Utilisé dans le path
 */
struct PathPoint
{
	int x;
	int y;
	bool toBomb; // ?
};
typedef struct PathPoint PathPoint;


/**
 * Point à tester (pour la map du pathfinding) avec la profondeur (distance parcouru)
 */
 struct TestPoint
{
	int x;
	int y;
	int dist;
};
typedef struct TestPoint TestPoint;


/**
 * IA de base .
 * Contient toutes les informations nécessaire et les fonctions de base
 * s'occupe des déplacement de base.
 */
class AI
{
	protected:
	
	/**
	 * Chemin jusqu'à l'objectif 
	 */
	std::vector<PathPoint> path;
	
	/**
	 * Pre-chemin comparé à l'ancien avant selection
	 */
	std::vector<PathPoint> newPath;
	
	/**
	 * Id du Bomber controlé pour savoir lequel l'ia controle
	 */
	int id;
	 
	public:
			
	/**
	 * Constructor
	 */
	 AI(int id);
	 
	 /**
	  * Fonction principale, retourne les mouvement à effectuer.
	  * Différente étapes:
	  * Prend le Field pour étude.
	  * Recalcul le chemin a effectuer (toute l'IA en fait)
	  * Effectue le prochain mouvement (moteur de mouvement, dans cette classe)
	  */
	void think(Field * field, float &x, float &y, bool &bomb);

	
	protected:
	
	/**
	 * Calcule le prochain mouvement à partir de la position et du path en cours
	 */
	void move( Field * field, float &x, float &y, bool &bomb );
	 
	 /**
	  * Function to define in the AIs
	  */
	virtual void compute_path( Field * field) ; // = 0;  //virtuel pure, mais on veut pas
	
	
};



//AIs

//Random move
class AIDummy : public AI
{
	public: 
	
	AIDummy(int id);
	
	protected:
	
	virtual void compute_path( Field * field ) ;
};

//Close quarter combat AI
class AICqc : public AI
{
	public: 
	
	AICqc(int id);
	
	
	protected:
	
	virtual void compute_path( Field * ptrField ) ;
	
	
	private:
	
	/** Attribute **/
	
	/**
	 * Pointeur vers le field
	 */
	Field * field;
	
	/**
	 * Position de départ
	 */
	int startX;
	int startY;
	
	/**
	 * Position de phare
	 */
	int objX;
	int objY;
	
	/**
	 * Map du terrain contenant les distance au point d'origine
	 * -1 = distance infini (inateignale, val par défaut)
	 */
	std::vector < std::vector<int> > distMap;
	
	/**
	 * File des points a étudier
	 */
	std::queue< TestPoint > toTestQueue;
	
	
	/** Méthodes **/
	
	/**
	 * Etudie le dernier point de la file et met à jour la map avec la profondeur correspndante
	 * S'il est bon, on ajoute ses voisions à étudier avec depth++ dans la file.
	 * full indique si on étudie maximum une fois un point ou non
	 * (s'il a déjà une distance, on l'ignore même s'il a déjà)
	 */
	void fill_map( bool full );
	
	/**
	 * Rempli le path en partant de l'objectif
	 * Lit la premiére valeur du path et rajoute la case vosine pemettant de mieux se rapprocher du départ.
	 * (distance la plus faible)
	 * @return return true si on a atteind le départ (dist = 0 ou pos = dep).
	 */
	bool generate_path();
};


//Evasive AI







#endif
