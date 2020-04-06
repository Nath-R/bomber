#include "Bomb.h"

Bomb::Bomb() :  Entity()
{
	timeStarted = (clock_t)0;
	isExploding = false;
	owner = NULL;
}

Bomb::Bomb(int x, int y, int squareSize) :  Entity()
{
	timeStarted = (clock_t)0;
	isExploding = false;
	owner = NULL;
	
	this->set_pos(x, y, squareSize);
}


bool Bomb::effect(Field * F)
{
	if(!isExploding)
	{
		isExploding = true;
		//Temps de départ
		timeStarted = clock();
	}
	
	//Obtention du temps écoulé depuis le début
	clock_t actTime = clock();
	
	float sec = ((float)actTime - (float)timeStarted)/ (float)CLOCKS_PER_SEC;
	
	//Position centrale
	int x, y;
	get_pos(x, y, F->get_square_size());
	

	
	//Action en conséquence (faire un return dans chaque if)
			
	if(sec <= 0)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3.5)
	{
		//Blast
		F->set_square_content(x, y, EXPLOSION);
		F->set_square_owner(x, y, owner);	
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EXPLOSION); F->set_square_owner(x-1, y, owner); }
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EXPLOSION); F->set_square_owner(x+1, y, owner);}	
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EXPLOSION); F->set_square_owner(x, y-1, owner);}	
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EXPLOSION); F->set_square_owner(x, y+1, owner);}		
		
		return false;
	}
	else
	{
		//fin du blast
		F->set_square_content(x, y, EMPTY);
		F->set_square_owner(x, y, NULL);
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EMPTY); F->set_square_owner(x-1, y, NULL);}
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EMPTY); F->set_square_owner(x+1, y, NULL);}	
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EMPTY); F->set_square_owner(x, y-1, NULL);}	
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EMPTY); F->set_square_owner(x, y+1, NULL);}
	}
	
	
	//End of the effect
	isExploding = false;
	return true;
}

void Bomb::set_pos(int x, int y, int squareSize)
{
	posX = x*squareSize + squareSize/2;
	posY = y*squareSize + squareSize/2;
}

void Bomb::get_pos(int &x, int &y, int squareSize)
{
	x = (int)posX / squareSize;
	y = (int)posY / squareSize;
}

void Bomb::set_owner(Entity * E)
{ owner = E; }



//Xbomb

Xbomb::Xbomb() : Bomb()
{}

Xbomb::Xbomb(int x, int y, int squareSize) :  Bomb(x, y, squareSize)
{
	
}


bool Xbomb::effect(Field * F)
{
	if(!isExploding)
	{
		isExploding = true;
		//Temps de départ
		timeStarted = clock();
	}
	
	//Obtention du temps écoulé depuis le début
	clock_t actTime = clock();
	
	float sec = ((float)actTime - (float)timeStarted)/ (float)CLOCKS_PER_SEC;
	
	//Position centrale
	int x, y;
	get_pos(x, y, F->get_square_size());
	

	
	//Action en conséquence (faire un return dans chaque if)
			
	if(sec <= 0)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3.5)
	{
		//Blast
		F->set_square_content(x, y, EXPLOSION);	
		
		bool ok = true; // pour la propagation
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x-2, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-2, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x-2, y, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x-3, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-3, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x-3, y, EXPLOSION); }
		
		ok = true;
		
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x+2, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+2, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x+2, y, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x+3, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+2, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x+3, y, EXPLOSION); }
		
		ok = true;
		
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x, y-2)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-2)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y-2, EXPLOSION); }
		else {ok = false;}	
		if( (F->get_square(x, y-3)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-3)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y-3, EXPLOSION); }	
		
		ok = true;
		
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x, y+2)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+2)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y+2, EXPLOSION); }
		else {ok = false;}
		if( (F->get_square(x, y+3)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+3)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y+3, EXPLOSION); }	
		
		return false;
	}
	else
	{
		bool ok = true;
		
		//fin du blast
		F->set_square_content(x, y, EMPTY);
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x-2, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-2, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x-2, y, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x-3, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-3, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x-3, y, EMPTY); }		
		
		ok = true;
		
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x+2, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+2, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x+2, y, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x+3, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+3, y)).get_content() != BOMB && ok)	
		{ F->set_square_content(x+3, y, EMPTY); }
		
		ok = true;
		
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EMPTY); }
		else {ok = false;}	
		if( (F->get_square(x, y-2)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-2)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y-2, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x, y-3)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-3)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y-3, EMPTY); }	
		
		ok = true;
		
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x, y+2)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+2)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y+2, EMPTY); }
		else {ok = false;}
		if( (F->get_square(x, y+3)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+3)).get_content() != BOMB && ok)	
		{ F->set_square_content(x, y+3, EMPTY); }
	}
	
	
	//End of the effect
	isExploding = false;
	return true;
}


//Blast bomb


BlastBomb::BlastBomb() : Bomb()
{}

BlastBomb::BlastBomb(int x, int y, int squareSize) :  Bomb(x, y, squareSize)
{
	
}


bool BlastBomb::effect(Field * F)
{
	if(!isExploding)
	{
		isExploding = true;
		//Temps de départ
		timeStarted = clock();
	}
	
	//Obtention du temps écoulé depuis le début
	clock_t actTime = clock();
	
	float sec = ((float)actTime - (float)timeStarted)/ (float)CLOCKS_PER_SEC;
	
	//Position centrale
	int x, y;
	get_pos(x, y, F->get_square_size());
	

	
	//Action en conséquence (faire un return dans chaque if)
			
	if(sec <= 0)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 3.5)
	{
		//Blast
		F->set_square_content(x, y, EXPLOSION);	
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EXPLOSION); }
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EXPLOSION); }	
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EXPLOSION); }	
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EXPLOSION); }
			
		if( (F->get_square(x-1, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y+1, EXPLOSION); }
		if( (F->get_square(x+1, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y-1, EXPLOSION); }	
		if( (F->get_square(x-1, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y-1, EXPLOSION); }	
		if( (F->get_square(x+1, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y+1, EXPLOSION); }		
		
		return false;
	}
	else
	{
		//fin du blast
		F->set_square_content(x, y, EMPTY);
		
		if( (F->get_square(x-1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y, EMPTY); }
		if( (F->get_square(x+1, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y, EMPTY); }	
		if( (F->get_square(x, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x, y-1, EMPTY); }	
		if( (F->get_square(x, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x, y+1, EMPTY); }
		
		if( (F->get_square(x-1, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y+1, EMPTY); }
		if( (F->get_square(x+1, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y-1, EMPTY); }	
		if( (F->get_square(x-1, y-1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-1, y-1)).get_content() != BOMB)	
		{ F->set_square_content(x-1, y-1, EMPTY); }	
		if( (F->get_square(x+1, y+1)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+1, y+1)).get_content() != BOMB)	
		{ F->set_square_content(x+1, y+1, EMPTY); }	
	}
	
	
	//End of the effect
	isExploding = false;
	return true;
}


//GrowingBomb

GrowingBomb::GrowingBomb() : Bomb()
{
	size = 0;
}

GrowingBomb::GrowingBomb(int x, int y, int squareSize) :  Bomb(x, y, squareSize)
{
	size = 0;
}

void GrowingBomb::set_owner(Entity * E)
{ 
	owner = E; 
	//~ super.set_owner(E);
	
	if(size == 0) //pour assurer qu'on ne modif qu'une fois
	{ 
		size = (((Bomber*)E)->get_nbrBombSet())/4;  
		if(size > 10)//taille max en dur
		{size = 10;}
	}
}

bool GrowingBomb::effect(Field * F)
{
	if(!isExploding)
	{
		isExploding = true;
		//Temps de départ
		timeStarted = clock();
	}
	
	//Obtention du temps écoulé depuis le début
	clock_t actTime = clock();
	
	float sec = ((float)actTime - (float)timeStarted)/ (float)CLOCKS_PER_SEC;
		
	//Position centrale
	int x, y;
	get_pos(x, y, F->get_square_size());
	
	bool endPosX, endNegX, endPosY, endNegY;
	endPosX = false;
	endNegX = false;
	endPosY = false;
	endNegY = false;
	
	
	if(sec <= 0)
	{;
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 2)
	{
		F->set_square_content(x, y, BOMB);
		return false;
	}
	else if(sec < 2.5)
	{
		//case de départ
		F->set_square_content(x, y, EXPLOSION); F->set_square_owner(x, y, owner);
	
		//i+1 pour faire au moins un kaboom
		for(int i=1; i<=(1+size); i++)
		{
			//X neg
			if( !endNegX && (F->get_square(x-i, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-i, y)).get_content() != BOMB)	
			{ F->set_square_content(x-i, y, EXPLOSION); F->set_square_owner(x-i, y, owner); }
			else
			{ endNegX = true; }
			
			//X pos
			if( !endPosX && (F->get_square(x+i, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+i, y)).get_content() != BOMB)	
			{ F->set_square_content(x+i, y, EXPLOSION); F->set_square_owner(x+i, y, owner); }
			else
			{ endPosX = true; }
			
			//Y neg
			if( !endNegY && (F->get_square(x, y-i)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-i)).get_content() != BOMB)	
			{ F->set_square_content(x, y-i, EXPLOSION); F->set_square_owner(x, y-i, owner); }
			else
			{ endNegY = true; }
			
			//Y pos
			if( !endPosY && (F->get_square(x, y+i)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+i)).get_content() != BOMB)	
			{ F->set_square_content(x, y+i, EXPLOSION); F->set_square_owner(x, y+i, owner); }
			else
			{ endPosY = true; }			
		}
		return false;
	}
	else
	{
		F->set_square_content(x, y, EMPTY); F->set_square_owner(x, y, NULL);
		
		//i+1 pour faire au moins un kaboom
		for(int i=1; i<=(1+size) ; i++)
		{
			//X neg
			if( !endNegX && (F->get_square(x-i, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x-i, y)).get_content() != BOMB)	
			{ F->set_square_content(x-i, y, EMPTY); F->set_square_owner(x-i, y, NULL); }
			else
			{ endNegX = true; }
			
			//X pos
			if( !endPosX && (F->get_square(x+i, y)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x+i, y)).get_content() != BOMB)	
			{ F->set_square_content(x+i, y, EMPTY); F->set_square_owner(x+i, y, NULL); }
			else
			{ endPosX = true; }
			
			//Y neg
			if( !endNegY && (F->get_square(x, y-i)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y-i)).get_content() != BOMB)	
			{ F->set_square_content(x, y-i, EMPTY); F->set_square_owner(x, y-i, NULL); }
			else
			{ endNegY = true; }
			
			//Y pos
			if( !endPosY && (F->get_square(x, y+i)).get_content() != INDESTRUCTIBLE &&  (F->get_square(x, y+i)).get_content() != BOMB)	
			{ F->set_square_content(x, y+i, EMPTY); F->set_square_owner(x, y+i, NULL); }
			else
			{ endPosY = true; }			
		}
	}
	
	//End of the effect
	isExploding = false;
	return true;
}



//TrackingBomb
