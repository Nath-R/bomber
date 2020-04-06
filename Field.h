#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <fstream>

#include "Square.h"
#include "Entity.h"
#include "Bomber.h"
#include "Bomb.h"

class Bomber;

/**
 * Le terrain, stockant les cases et les entités dessus
 */
class Field
{
	private:
	
	/**
	 * Le terrain en lui même, composé de cases
	 */
	std::vector < std::vector<Square> > squares;
	
	/**
	 * Taille d'une case dans l'environement
	 */
	int squareSize;
	
	/**
	 * Taille du terrain en nombre de case selon les x
	 */
	int fieldSizeX;
	 
	 /**
	 * Taille du terrain en nombre de case selon les y
	 */
	int fieldSizeY;
	 
	 /**
	  * les joueurs, les bombes seront considéres comme des case
	  */
	std::vector <Bomber> bombers;
	 
	 
	 public:
	 
	 /**
	  * Constructor
	  */
	 Field(int x, int y);
	 Field(std::string mapFile);
	 
	 
	 //Accesors
	 int get_X_size();
	 int get_Y_size();
	 int get_square_size();
	 int get_nbr_bomber();
	 Bomber& get_bomber(int i);
	 
	 Square get_square(int x, int y);
	 
	 /**
	  * Retourne (ref) la position dans le tableau de la case corrspondant à la position absolue en question
	  * retourne -1 sur les deux si invalides.
	 */
	 void get_square_pos(int x, int y, int& px, int& py);
	 
	 //Mutator
	 
	 void set_square_content(int x, int y, nature n);
	 
	 void set_square_owner(int x, int y, Entity * E);	 
	 
	 /**
	  * Calcul le projeté d'un point sur un cercle.
	  * En gros, intersection du cercle de centre O (origine) et de la droite OP (l'autre point)
	  * Cela permet, de, peu importe le vecteur vitesse, de le mettre à la bonne distance (vitesse max)
	  */
	 void compute_point_on_circle(float x, float y, float radius, float &newX, float &newY); 
	 
	 
	 /**
	  * Mise à jour du jeu, avec collision
	  */
	  void update();
	  
	  /**
	   * Ajoute un bomberman et retourne un pointeur sur celui ci
	   * Position de départ aléatoire
	   */
	  Bomber* add_bomber();
	  
	  /**
	   * Ajoute un bomberman et retourne un pointeur sur celui ci
	   * Position fixe (0.0)
	   */
	  Bomber* add_bomber_default();
	  
	 /**
	  * Change la taille du vector
	  */
	 void vector_bombers_size(int s);
};

#endif
