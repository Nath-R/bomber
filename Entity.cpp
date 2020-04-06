#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	posX = 110; //debug value
	posY = 110;
	
	movX = 0;
	movY = 0;
	
	team = 0;
}

Entity::Entity(float X, float Y)
{
	posX = X;
	posY = Y;
	
	movX = 0;
	movY = 0;
	
	team = 0;
}

void Entity::update_move(int x, int y)
{
	if( x < 0 ) 
	{ movX = -1;}
	else if ( x > 0)
	{ movX = 1; }
	else
	{ movX = 0; }
	
	if( y < 0 ) 
	{ movY = -1;}
	else if ( y > 0)
	{ movY = 1; }
	else
	{ movY = 0; }
}

void Entity::update_move(float x, float y)
{
	if( x < 0 ) 
	{ movX = -1;}
	else if ( x > 0)
	{ movX = 1; }
	else
	{ movX = 0; }
	
	if( y < 0 ) 
	{ movY = -1;}
	else if ( y > 0)
	{ movY = 1; }
	else
	{ movY = 0; }
}

void Entity::set_pos(float x, float y)
{
	posX = x;
	posY = y;
}

void Entity::get_pos_mov(float &px, float &py, float &mx, float &my)
{
	px = posX;
	py = posY;
	mx = movX;
	my = movY;
}

void Entity::set_team(int i)
{
	if(i>=0)
	{ team = i; }
}

int Entity::get_team()
{
	return team;
}
