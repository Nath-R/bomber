#ifndef BOMB_H
#define BOMB_H

#include "Entity.h"
#include "Field.h"

#include <ctime>

class Field;
class Bomber;

/**
 * Pour cette classe, la position rendu et mise est la position en case, mais est bien sotcké avec les valeur réelle
 * 
*/
class Bomb : public Entity
{
	protected:	

	/**
	 * Le moment où l'effet de la bombe a commencé
	 */
	clock_t timeStarted;
	
	/**
	 * Indique si on est en train d'exploser ou pas
	 */
	bool isExploding;
	
	/**
	 * Propriétaire de la bombe, surtout pour attribuer les proptiétaires des blocs modifiés
	 */
	Entity * owner;
	 
	
	public:
	
	Bomb();
	
	Bomb(int x, int y, int squareSize) ;
	
	/**
	 * Effet de la bombe, return s'il est fini, ou non.
	 * Pour les proprio des cases modifié, utiliser le bomber, pour éviter les fuites mémoire
	 */
	virtual bool effect(Field * F);
	
	/**
	 * Obtient la position sur le plateau
	 */
	 void get_pos(int &x, int &y, int squareSize);
	 
	/**
	 * met la position sur le plateau en position absolue dans les coord de Entity
	 */
	void set_pos(int x, int y, int squareSize);
	
	/**
	 * On attribut le proprio, n'est pas mis dans le constructeur pour simplifier les héritages
	 */
	virtual void set_owner(Entity * E);
};



class Xbomb : public Bomb
{
	public:
	
	Xbomb();
	Xbomb(int x, int y, int squareSize);
	
	/**
	 * Effet de la bombe, return s'il est fini, ou non.
	 */
	virtual bool effect(Field * F);
};


class BlastBomb : public Bomb
{
	public:
	
	BlastBomb();
	BlastBomb(int x, int y, int squareSize);
	
	/**
	 * Effet de la bombe, return s'il est fini, ou non.
	 */
	virtual bool effect(Field * F);
};


class GrowingBomb : public Bomb
{
	private:
	/**
	 * Taille de l'explosion, mis à jour lors de l'obtention d'un proprio
	 */
	int size;
	
	public:
	
	GrowingBomb();
	GrowingBomb(int x, int y, int squareSize);
	
	/**
	 * Effet de la bombe, return s'il est fini, ou non.
	 */
	virtual bool effect(Field * F);
	
	/**
	 * Surchage de la méthode set_owner
	 */
	virtual void set_owner(Entity * E);
};

#endif
