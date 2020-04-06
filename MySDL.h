#ifndef MYSDL_H_INCLUDED
#define MYSDL_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>


struct point
{
    float x;
    float y;
};
typedef struct point POSITION;


/**affichage**/

struct image
{
    POSITION pos; // centre
    double dir; // angle d'orientation
    SDL_Surface* pict; // == picture, pointeur sur l'image
};
typedef struct image IMAGE;


/**
 * initialise la SDL et la fenetre
 */
void init_window(int width, int height, char name[]);

/**
 * vide l'�cran en y remmetant la couleur de fond initiale
 */
void clean_screen();

/**
 * dessine un pixel de couleur RGB
 */
void display_pixel(POSITION pos, int R, int G, int B);

/**
 * Dessine une segment
 */
void display_segment(POSITION C, POSITION D, int R, int G, int B);

/**
 * assez clair non?
 */
IMAGE load_image(const char nameBMP[]);

/**
 * affiche une image:
 * transp degr� de transparence de l'image | RGB, couleur � mettre transparente
 */
void display_image(IMAGE image, int transp, int R, int G, int B); 

/**
 * m�j de l'�cran
 */
void synchro();


/**evenements**/

//OLD

void wait_event();

POSITION get_arrow();//retourne une position, avec  le sens d'incr�mentation en x et en y (par bool�en)

POSITION get_mouse_pos();

/**
 * Si on demande � fermer le programme
 */
int get_close_request();

//GOOD

/**
 * Gestion des �venements, stocke toute les touches press�es sous forme de bool�en.
 * Acc�s sous diff�rentes forme. 
 * Bool � true, si event ou bouton press�
 */
class EventSDL
{
	private:
	
	//Les Fl�ches
	bool arrowLeft;
	bool arrowRight;
	bool arrowUp;
	bool arrowDown;
	
	//Barre d'espace
	bool spaceBar;
	
	//Exit
	bool exitRequest;
	
	//Position actuel de la souris
	POSITION mousePos;
	
	public:
	
	/**
	 * COnstructeur, just des initialisations
	 */
	EventSDL();
	
	/**
	 * Detection des event SDL, et changement en cons�quence
	 */
	void update();	
	
	/**
	 * Retourne une position (x, y)
	 * Avec les sens d'incr�mentation de x et y
	 */
	 void get_arrow(float& x, float& y);
	 
	/**
	 * Si on demande � fermer le programme
	 */
	bool get_close_request();
	
	/**
	 * Accesseur standard
	 */	 
	bool press_spaceBar();
};




#endif // MYSDL_H_INCLUDED
