#include "Field.h"

Field::Field(int x, int y)
{
	squareSize = 20;
	
	fieldSizeX = x;
	fieldSizeY = y;
	
	srand(time(NULL));//Initialisation de l'alea
	
	for(int i=0; i<x; i++)
	{
		squares.push_back( std::vector<Square>() );
		for(int j=0; j<y; j++)
		{
			//~ if(i%3*j%2 == 0) squares[i].push_back( Square(EMPTY) );
			//~ else squares[i].push_back( Square(BLOCK) );		
			
			int alea = rand()%100;
			
			if(alea<60)
			{ squares[i].push_back( Square(BLOCK) ); }
			else if(alea < 85)
			{ squares[i].push_back( Square(EMPTY) ); }
			else
			{ squares[i].push_back( Square(INDESTRUCTIBLE) ); }
		}
	}
}

Field::Field( std::string mapFile)
{
	bool ok = true;
	bool done = false;
	
	squareSize = 20;
	
	std::ifstream map;
	
	map.open(mapFile.c_str(), std::ifstream::out);
	
	
	//Alea init for the player spawn
	srand(time(NULL));
	
	if( !map.good() )
	{ok = false; }
	
	//Getting the field size
	map >> fieldSizeX;
	map >> fieldSizeY;
	
	//going through the file and generating the field
	int content;
	
	for(int i=0; i<fieldSizeX && ok; i++)
	{
		squares.push_back( std::vector<Square>() );
		for(int j=0; j<fieldSizeY && ok; j++)
		{
			map >> content;
			
			if(content == 0)
			{ squares[i].push_back( Square(BLOCK) ); }
			else if (content == 1)
			{ squares[i].push_back( Square(INDESTRUCTIBLE) ); }
			else if (content == 2)
			{ squares[i].push_back( Square(EMPTY) ); }
			else 
			{ squares[i].push_back( Square(EMPTY) ); }
			
			if( !map.good() )
			{ok = false; }
			
			//end
			if( i == fieldSizeX-1 && j == fieldSizeY-1)
			{done = true; }
		}
	}
	//cas d'erreur
	if( !ok && !done )
	{
		std::cout<<"Unable to load the map, reseting and building a random one"<<std::endl;
		squares.clear();
		
		//copie coller du constructeur standard
		fieldSizeX = 20;
		fieldSizeY = 20;
		
		srand(time(NULL));//Initialisation de l'alea
	
		for(int i=0; i<fieldSizeX; i++)
		{
			squares.push_back( std::vector<Square>() );
			for(int j=0; j<fieldSizeY; j++)
			{				
				int alea = rand()%100;
				
				if(alea<60)
				{ squares[i].push_back( Square(BLOCK) ); }
				else if(alea < 85)
				{ squares[i].push_back( Square(EMPTY) ); }
				else
				{ squares[i].push_back( Square(INDESTRUCTIBLE) ); }
			}
		}
		
	}	
	
	map.close();
}



int Field::get_X_size()
{ return fieldSizeX; }

int Field::get_Y_size()
{ return fieldSizeY; }

int Field::get_square_size()
{ return squareSize; }

Square Field::get_square(int x, int y)
{ 
	if( 0<=x && x<fieldSizeX && 0<=y && y<fieldSizeY)
	{   return squares[x][y]; }
	else
	{ std::cout<<"Out of range (squares vector)!"<<std::endl; return Square(INDESTRUCTIBLE); }
}

void Field::set_square_content(int x, int y, nature n)
{
	if( 0<=x && x<fieldSizeX && 0<=y && y<fieldSizeY)
	{   (squares[x][y]).set_content(n); }
	else
	{}
}

void Field::set_square_owner(int x, int y, Entity * E)
{
	if( 0<=x && x<fieldSizeX && 0<=y && y<fieldSizeY)
	{   (squares[x][y]).set_owner(E); }
	else
	{}
}


void Field::get_square_pos(int x, int y, int& px, int& py)
{
	if( 0<=x && x<(fieldSizeX*squareSize)  &&  0<=y && y<(fieldSizeY*squareSize)  )
	{
		px = x/squareSize;
		py = y/squareSize;
	}
	else
	{
		std::cout<<"Out of range (position sup)!"<<std::endl;
		px = -1;
		py = -1;
		
		if ( x>=(fieldSizeX*squareSize) )
		{px = x/squareSize;}
		else if (x < 0)
		{px = -1;}
		else
		{px = x/squareSize;}
		
		if (y>=(fieldSizeY*squareSize))
		{py = y/squareSize;}
		else if (y < 0)
		{py = -1;}
		else
		{py = y/squareSize;}
	}

}

int Field::get_nbr_bomber()
{ return bombers.size(); }

Bomber& Field::get_bomber(int i)
{
	if( i<0 || i>(int)bombers.size() )
	{ std::cout<<"Out of range (bomber vector) !"<<std::endl; return bombers[0]; }
	
	return bombers[i];
	
}


Bomber* Field::add_bomber()
{
	//Position aléatoire pour cette version
	int x = rand()%fieldSizeX;
	int y = rand()%fieldSizeY;
	
	squares[x][y].set_content(EMPTY);
	
	bombers.push_back( Bomber( x*squareSize + squareSize/2, y*squareSize + squareSize/2) );
	return &( bombers.back() );
}

Bomber* Field::add_bomber_default()
{
	bombers.push_back( Bomber(0, 0) );
	return &( bombers.back() );
}

void Field::vector_bombers_size(int s)
{
	bombers.resize(s);
}


void Field::compute_point_on_circle(float x, float y, float radius, float &newX, float &newY)
{
	if(x != 0)
	{
		//équation de droite passant par l'origine: ax = y
		float a = y/x;
		
		//calcul du nouveau x et du nouveau y
		newX = sqrt( pow(radius,2)/(1 + pow(a,2)) );
		newY = sqrt( pow(radius,2)- pow(newX,2));
		
		if(x<0) { newX = -1*newX; }
		if(y<0) { newY = -1*newY; }
	}
	else
	{
		newY=y;
		newX=x;
	}
}


void Field::update()
{
	//Gestion du temps
	static clock_t prevTime= clock();
	clock_t actTime = clock();
	
	float sec = ((float)actTime - (float)prevTime)/ (float)CLOCKS_PER_SEC;
	//std::cout<<"sec:"<<sec<<std::endl;
	
	//Gestion des joueurs
	for(int i=0; i<(int)bombers.size(); i++)
	{	
		if(bombers[i].get_HP() <= 0) { continue; }
		
		
		float Px;
		float Py;
		float newPx;
		float newPy;
		float Mx;
		float My;
		//calcul de vitesse
		float speed = (float)(bombers[i].get_moving_speed()) * sec; //faire intervenir le temps pour avoir la val du mouv		
		(bombers[i]).get_pos_mov(Px, Py, Mx, My);			
				
		compute_point_on_circle(Mx, My, 1, Mx, My);
				
		//Calcul de la nouvelle position
		newPx = Px + (Mx * speed);
		newPy = Py + (My * speed);
		
		
		///Gestion des collisions
		int posSquareX; //la position en case du joueur
		int posSquareY;
		get_square_pos(newPx, newPy, posSquareX, posSquareY);
		
		float cornerX[4]; //0=hg  1=hd  2=bd  4=bg
		float cornerY[4];


		
		for(int j=0; j<4; j++)
		{
			//On recalcule la position des coins 
			
			//get_square_pos(newPx, newPy, posSquareX, posSquareY); // pas sur
			
			cornerX[0] = newPx - (float)bombers[i].get_size()/2.0;
			cornerY[0] = newPy + (float)bombers[i].get_size()/2.0;
					
			cornerX[1] = newPx + (float)bombers[i].get_size()/2.0;
			cornerY[1] = newPy + (float)bombers[i].get_size()/2.0;	
				
			cornerX[2] = newPx + (float)bombers[i].get_size()/2.0;
			cornerY[2] = newPy - (float)bombers[i].get_size()/2.0;
					
			cornerX[3] = newPx - (float)bombers[i].get_size()/2.0;
			cornerY[3] = newPy - (float)bombers[i].get_size()/2.0;		
			
			//début du traitement
			int cornerSquPosX;
			int cornerSquPosY;
			get_square_pos((int)cornerX[j], (int)cornerY[j], cornerSquPosX, cornerSquPosY);			

			//valeur de déplacement pour sortir
			float Xmodif = 0;
			float Ymodif = 0;

			Square S;
			S = get_square(cornerSquPosX, cornerSquPosY);			
			
			//même case que le centre, on fais rien
			if(cornerSquPosX == posSquareX && cornerSquPosY == posSquareY)
			{  }
			//Case accessible, on fait rien 
			//TODO Rajouter éventuellement la possibilité de passe dans ses bombes
			else if(S.get_content() == EMPTY || S.get_content() == EXPLOSION)
			{ }
			else // if(S.get_content() == BLOCK || S.get_content() == INDESTRUCTIBLE || S.get_content() == BOMB)
			{				
				bool done = false;				
				
				//gestion des du déplacement
				//check with prev
				int cornerSquPosXP;
				int cornerSquPosYP;
				int prev = (j-1)%4;
				if(prev < 0) 
				{ prev = 3; }
				
				get_square_pos((int)cornerX[prev], (int)cornerY[prev], cornerSquPosXP, cornerSquPosYP);
				Square Sp;
				Sp = get_square(cornerSquPosXP, cornerSquPosYP);				
						
				//std::cout<<"corner X:"<< cornerSquPosXP<<" Y: "<<cornerSquPosYP<<std::endl;
				
				//Même case que le centre, on ignore 
				if(cornerSquPosXP == posSquareX && cornerSquPosYP == posSquareY)
				{  }
				//aussi dans un mur
				else if( Sp.get_content() == BLOCK || Sp.get_content() == INDESTRUCTIBLE || Sp.get_content() == BOMB )
				{
					if( cornerY[j] == cornerY[(j-1)%4] ) // aligné selon les x
					{ 
						//Par le bas
						if(posSquareY < cornerSquPosY)
						{ Ymodif =  cornerY[j] - (cornerSquPosY * squareSize); } 
						else
						{ Ymodif =  cornerY[j] - ((cornerSquPosY+1) * squareSize); } 
					}
					else //selon les Y
					{ 
						if(posSquareX < cornerSquPosX)
						{ Xmodif = cornerX[j] - (cornerSquPosX * squareSize); }
						else						
						{ Xmodif = cornerX[j] - ((cornerSquPosX+1) * squareSize); } 
					}
					
					done = true;
				}
				
				//avec le point suivant
				int cornerSquPosXN;
				int cornerSquPosYN;
				get_square_pos((int)cornerX[(j+1)%4], (int)cornerY[(j+1)%4], cornerSquPosXN, cornerSquPosYN);
				Square Sn;
				Sn = get_square(cornerSquPosXN, cornerSquPosYN);
				
				//Même case que le centre, on ignore 
				if(cornerSquPosXN == posSquareX && cornerSquPosYN == posSquareY)
				{  }
				//aussi dans un mur
				else if( Sn.get_content() == BLOCK || Sn.get_content() == INDESTRUCTIBLE || Sn.get_content() == BOMB )
				{//std::cout<<"suiv point"<<std::endl;
					if( cornerY[j] == cornerY[(j+1)%4] ) // aligné selon les x
					{ 						
						//Par le bas
						if(posSquareY < cornerSquPosY)
						{ Ymodif = cornerY[j] - (cornerSquPosY * squareSize); } 
						else
						{ Ymodif = cornerY[j] - ((cornerSquPosY+1) * squareSize); } 
					}
					else //selon les Y
					{ 						
						if(posSquareX < cornerSquPosX)
						{ Xmodif = cornerX[j] - (cornerSquPosX * squareSize); }
						else
						{ Xmodif = cornerX[j] - ((cornerSquPosX+1) * squareSize); }  
					}
					
					done = true;
				}
				
				
				//Dans le cas où on à pas encore bougé
				if(!done)
				{					
					//si case aligné sur les x
					float distX = -100; //pas touché
					float distY = -100;
					

					if(posSquareX < cornerSquPosX)
					{ distX = cornerX[j] - (cornerSquPosX * squareSize); }
					//~ else if (posSquareX == cornerSquPosX)
					//~ {}
					else
					{ distX = cornerX[j] - ((cornerSquPosX+1) * squareSize); } 

					if(posSquareY < cornerSquPosY)
					{ distY = cornerY[j] - (cornerSquPosY* squareSize); }
					//~ else if (posSquareY == cornerSquPosY)
					//~ {}
					else
					{ distY = cornerY[j] - ((cornerSquPosY+1) * squareSize); } 
					
										
					if( distX == -100 || fabs(distY)<=fabs(distX) )
					{
						Ymodif = distY;
					}
					else if ( distY == -100 || fabs(distX)<fabs(distY) )
					{
						Xmodif = distX;
					}
				}				
				newPx -= Xmodif;
				newPy -= Ymodif;
			}
			
		}
		
		///End collision				
		
		//Mise en position			
		(bombers[i]).set_pos(newPx, newPy);
		
		///Gestion de dégats: si sur explosion d'équipe différente
		
		//Test durée invincibilité
		clock_t prev = bombers[i].get_timeHit();
		clock_t act = clock();
		float sec = ((float)act - (float)prev)/ (float)CLOCKS_PER_SEC;
		if( sec > bombers[i].get_invincible_duration() )
		{ bombers[i].set_invincible(false); }
		
		//Recalcul des coins
		cornerX[0] = newPx - (float)bombers[i].get_size()/2.0;
		cornerY[0] = newPy + (float)bombers[i].get_size()/2.0;
				
		cornerX[1] = newPx + (float)bombers[i].get_size()/2.0;
		cornerY[1] = newPy + (float)bombers[i].get_size()/2.0;	
			
		cornerX[2] = newPx + (float)bombers[i].get_size()/2.0;
		cornerY[2] = newPy - (float)bombers[i].get_size()/2.0;
				
		cornerX[3] = newPx - (float)bombers[i].get_size()/2.0;
		cornerY[3] = newPy - (float)bombers[i].get_size()/2.0;
		
		for(int j=0; j<4 && !bombers[i].is_invincible(); j++)
		{
			int cornerSquPosX;
			int cornerSquPosY;
			get_square_pos((int)cornerX[j], (int)cornerY[j], cornerSquPosX, cornerSquPosY);	
			Square S;
			S =	get_square(cornerSquPosX, cornerSquPosY);
			
			if(S.get_content() == EXPLOSION && ( S.get_owner()==NULL || (S.get_owner()!=NULL && S.get_owner()->get_team() != bombers[i].get_team()  ) ) )
			{ bombers[i].damage(); }
		}
		

		
	}
	
	//Collision entre joueurs (pas prévu dans un premier temps)
	
	
	//Gestion des bombes
	
	//Nouvelle bombes
	for(int i=0; i<(int)bombers.size(); i++)
	{
		if(bombers[i].get_HP() <= 0) { continue; }
				
		if( bombers[i].is_bombing() )
		{
			float Px, Py;
			float Mx, My;
			(bombers[i]).get_pos_mov(Px, Py, Mx, My);
			Square actS = get_square((int)Px/squareSize, (int)Py/squareSize);
			
			//Creation/selection de la bombe (niveau du joueur ?)
			Bomb * B = new GrowingBomb((int)Px/squareSize, (int)Py/squareSize, squareSize);
			
			//Ajout de la bombe chez le joueur, si on peut (terrain), verif sur nombre Max après
			if( actS.get_content() != BLOCK && actS.get_content() != INDESTRUCTIBLE && actS.get_content() != BOMB )
			{ bombers[i].add_bomb(B); }
			
			//Mouvement achevé
			bombers[i].is_bombing(false);
		}
	}
	
	//On parcourt chaque bomberman, puis chacune de ses bombes
	for(int i=0; i<(int)bombers.size(); i++)
	{		
		int nbrBomb = bombers[i].get_nb_bomb();
		Bomb * B = NULL;
		bool end = false;
		
		for(int j=0; j<nbrBomb; j++)
		{
			B = bombers[i].get_bomb(j);
			
			if(B != NULL) 
			{ end = B->effect(this); }
			
			if(end)
			{ bombers[i].remove_bomb(j); }
		}
	}
	
	
	
	//Reset du prev time
	prevTime = clock();
}
