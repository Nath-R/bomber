#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include "Entity.h"

enum nature
{
	BLOCK,
	INDESTRUCTIBLE,
	EMPTY,
	EXPLOSION,
	BOMB
};

class Square
{
	private:
	
	/**
	 * Contenu de la case (de différentes natures)
	 */
	nature content;
	
	/**
	 * Proprio, dans le cas de bombe ou de barricade
	 */
	Entity * owner;
	
	/**
	 * Point de vie
	 */
	int health;	
	
	public:
	
	/**
	 * Constructeur
	 */
	Square();
	Square(nature n);
	
	
	nature get_content();
	void set_content(nature n);
	
	void set_owner(Entity * E);
	Entity * get_owner();
};

#endif
