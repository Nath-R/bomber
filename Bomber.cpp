#include "Bomber.h"

Bomber::Bomber() : Entity()
{
	size = 10;
	
	speed = 100; // pif
	
	maxBomb = 3;
	
	isBombing = false;
	
	isInvincible = false;
	
	timeInvincible = 2; //sec
	
	timeHit = clock(); //default
	
	HP = 4;
	
	nbrBombSet = 0;
}

Bomber::Bomber(float x, float y) : Entity(x, y)
{
	size = 10;
	
	speed = 100; // pif
	
	maxBomb = 3;
	
	isBombing = false;
	
	isInvincible = false;
	
	timeInvincible = 2; //sec

	timeHit = clock(); //default
	
	HP = 4;
	
	nbrBombSet = 0;
}


void Bomber::is_bombing(bool val)
{
	isBombing = val;
}

bool Bomber::is_bombing()
{
	return isBombing;
}


int Bomber::get_moving_speed()
{ return speed; }

int Bomber::get_size()
{ return size; }

int Bomber::get_nb_bomb()
{ return bombs.size(); }

Bomb* Bomber::get_bomb(int i)
{ 
	if(0<i && i<(int)bombs.size())
	{ return (bombs[i]); }
	else
	{ return NULL; }	
}

void Bomber::remove_bomb(int i)
{
	if(0<i && i<(int)bombs.size())
	{ 
		free(bombs[i]);
		bombs.erase(bombs.begin() + i); 
	}
	else
	{  }
}


void Bomber::add_bomb(Bomb * B)
{
	if((int)bombs.size() > maxBomb)
	{
		//~ std::cout<<"Max bomb reached !"<<std::endl;
	}
	else
	{  B->set_owner(this); bombs.push_back(B); nbrBombSet++; }	
}

void Bomber::set_invincible(bool b)
{ isInvincible = b; }

bool Bomber::is_invincible()
{ return isInvincible; }

float Bomber::get_invincible_duration()
{ return timeInvincible; }

clock_t Bomber::get_timeHit()
{ return timeHit;}

int Bomber::get_HP()
{ return HP;}

int Bomber::get_nbrBombSet()
{ return nbrBombSet; }


void Bomber::damage()
{
	this->set_invincible(true);
	timeHit = clock();	
	HP--;
}



