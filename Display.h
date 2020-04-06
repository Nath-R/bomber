#ifndef DISPLAY_H
#define DISPLAY_H


#include "Field.h"


#include "MySDL.h"

struct data
{
   // float x; //previous pos
   // float y;
   float ang;  //derniére direction, 0 = vers le bas
   int cmpt; // compteur pour savoir à quelle animation on en est
   clock_t stepAnimStart; //début d'un pas
};
typedef struct data AnimData;

class Display
{

	private:
	
	IMAGE squareEmp;
	IMAGE squareBlo;
	IMAGE squareInd;
	IMAGE squareExp;
	IMAGE squareBom;	
	
	//Images dans une direction, incluant l'idle et la marche
	//Face
	IMAGE playerFace[3];
	//back
	IMAGE playerBack[3];
	//lateral left
	IMAGE playerLatL[3];
	//lateral right
	IMAGE playerLatR[3];
	
	
	std::vector<AnimData> bomberData;
	
	
	/**
	 * Méthode interne pour sélectionner une image de bomber à afficher 
	 * en fonction de (sa position et) sa vitesse.
	 * L'id (i) du bomber est nécessaire pour savoir à quelle image il en est.
	 */
	IMAGE select_bomber_image(int i, float x, float y, float mx, float my);
	
	public:
	Display();
	~Display();
	
	void display_field(Field F);
};


#endif
