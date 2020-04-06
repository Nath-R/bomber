#include "AI.h"


//** AI CORE **//

AI::AI( int id )
{
	this->id = id;
}

void AI::think(Field * field, float &x, float &y, bool &bomb)
{
	
	this->compute_path(field);
	
	move(field, x, y, bomb);
}

void AI::move( Field * field, float &x, float &y, bool &bomb )
{
	if(path.size() == 0) //rien à faire
	{ return ; }
	
	int squareSize = field->get_square_size();
	
	//Getting the pos
	Bomber bomber = field->get_bomber(id);
	
	float posX, posY, movX, movY;
	
	bomber.get_pos_mov(posX, posY, movX, movY);
	
	//Read next objective	
	int objX = path[0].x * squareSize + squareSize/2;
	int objY = path[0].y * squareSize + squareSize/2;
	
	//Check if we are at the objective
	if( fabs(posX - objX) < squareSize/4 && fabs(posY - objY) < squareSize/4 )
	{
		bomb = path[0].toBomb;
		x = 0;
		y = 0;
		
		//supprime obj actuel
		path.erase (path.begin()+0);		
	}
	//mouvement à faire
	else 
	{
		int sqPosBombX, sqPosBombY;
		field->get_square_pos(posX, posY, sqPosBombX, sqPosBombY);				
		
		//Casr pour arrondir
		x = (int)(objX - posX) / fabs(objX - posX) ;
		y = (int)(objY - posY) / fabs(objY - posY) ;
	}
	
}

void AI::compute_path( Field * field)
{
	//Nada	
}


//** AIs **//

//Dummy
AIDummy::AIDummy(int id) : AI(id)
{}

void AIDummy::compute_path(Field * field)
{
	Bomber bomber = field->get_bomber(id);
	
	//Si on a rien on prend un point autour
	if(path.size() == 0)
	{
		float posX, posY, movX, movY;
		int sqPosBombX, sqPosBombY;
		
		bomber.get_pos_mov(posX, posY, movX, movY);
		field->get_square_pos(posX, posY, sqPosBombX, sqPosBombY);
		
		std::vector<PathPoint> possibleWay;
		
		nature n;
		PathPoint newPP;
	

		//Getting the way we can move
		
		n = (field->get_square(sqPosBombX+1, sqPosBombY)).get_content();
		if( n == EMPTY )
		{
			newPP.x = sqPosBombX+1;
			newPP.y = sqPosBombY;
			newPP.toBomb = false;
			possibleWay.push_back(newPP);
		}
		
		n = (field->get_square(sqPosBombX-1, sqPosBombY)).get_content();
		if( n == EMPTY )
		{
			newPP.x = sqPosBombX-1;
			newPP.y = sqPosBombY;
			newPP.toBomb = false;
			possibleWay.push_back(newPP);
		}
		
		n = (field->get_square(sqPosBombX, sqPosBombY+1)).get_content();
		if( n == EMPTY )
		{
			newPP.x = sqPosBombX;
			newPP.y = sqPosBombY+1;
			newPP.toBomb = false;
			possibleWay.push_back(newPP);
		}
		
		n = (field->get_square(sqPosBombX, sqPosBombY-1)).get_content();
		if( n == EMPTY )
		{
			newPP.x = sqPosBombX;
			newPP.y = sqPosBombY-1;
			newPP.toBomb = false;
			possibleWay.push_back(newPP);
		}
		
		//Curr pos
		newPP.x = sqPosBombX;
		newPP.y = sqPosBombY;
		newPP.toBomb = true;
		possibleWay.push_back(newPP);
		
		//Select the move to do overall
		int alea = rand()%possibleWay.size();	
		
		path.push_back( possibleWay[alea] );
		
	}
}


//Close quarter combat oriented AI

AICqc::AICqc(int id) : AI(id)
{
	
}

void AICqc::compute_path(Field * ptrField)
{
	//Initializations	
	
	field = ptrField;
	
	if(distMap.size() == 0)
	{
		for(int i=0; i < field->get_X_size(); i++)
		{
			distMap.push_back( std::vector<int>() );
			for(int j=0; j< field->get_Y_size(); j++)
			{
				distMap[i].push_back( -1 );
			}
		}
	}
	
	//Dist map init	
	for(int i=0; i < field->get_X_size(); i++)
	{
		for(int j=0; j<field->get_Y_size(); j++)
		{
			distMap[i][j] = -1;
		}
	}
	
	//Starting pos (bomber's pos)
	Bomber bomber = field->get_bomber(id);
	
	float posX, posY, movX, movY;	
	bomber.get_pos_mov(posX, posY, movX, movY);
	field->get_square_pos(posX, posY, startX, startY);
	
	//Objective (closest bomber)
	float prevDist = 1000; //huge walue
	
	for(int i=0; i<field->get_nbr_bomber(); i++)
	{
		if(i != id)
		{
			Bomber target = field->get_bomber(i);
			float posX, posY, movX, movY;
			int sqPosX, sqPosY;	
			target.get_pos_mov(posX, posY, movX, movY);
			field->get_square_pos(posX, posY, sqPosX, sqPosY);
			
			float distCur = sqrt( pow(sqPosX - startX, 2) + pow(sqPosY - startY, 2) );
			if( distCur < prevDist && target.get_HP() > 0)
			{
				prevDist = distCur;
				objX = sqPosX;
				objY = sqPosY;
			}
		}
	}
	
	
	//Target locked, roll out !
	//Generating the distance map
	
	//Fist point
	TestPoint first;
	first.x = startX;
	first.y = startY;
	first.dist = 0;
	
	toTestQueue.push(first);
	
	while(! toTestQueue.empty() )
	{
		fill_map( true );
	}
	

	
	//We have the map, creating the path
	
	if(distMap[objX][objY] != -1) //-1 => unreachable
	{
		path.clear();
		
		//First element
		PathPoint first ;
		first.x = objX;
		first.y = objY;
		first.toBomb = true;
		
		//Inserting at the beginning of the path
		path.insert (path.begin()+0, first);
		
		bool done = false;
			
		while (!done)
		{
			done = generate_path();
		}	
		
		
		if( path.size() > 1)
		{
			//Met l'avant dernier à bomb (-1 last -2 prelast )
			path[path.size() - 2].toBomb = true;	
			
			//supprime obj actuel, car on refait le path à chaque fois ! (sauf si besoin d'un bombe)
			if(! path[0].toBomb)
			path.erase (path.begin()+0);		
		
		}
	
	}
	
	
}

void AICqc::fill_map( bool full = false )
{
	//Error case
	if( toTestQueue.empty() ) return ;
	
	//Getting the next point to test
	TestPoint point = toTestQueue.front();
	toTestQueue.pop();

	
	//Check if the value is in range
	if( !(0 <= point.x && point.x < field->get_X_size()  &&  0 <= point.y && point.y < field->get_Y_size() ) )
	{return;}
	
	//Check if the value is not already set (if not full or with) or with a shorter distance
	if( (!full && distMap[point.x][point.y] != -1) ||  (distMap[point.x][point.y] < point.dist && distMap[point.x][point.y] != -1) )
	{return;}
	
	
	//Getting the content of the case
	nature n = (field->get_square(point.x, point.y)).get_content();
	
	//If current square or not (we can leave it anyway if we are on that square)
	Bomber target = field->get_bomber(id);
	float posX, posY, movX, movY;
	int sqPosX, sqPosY;	
	target.get_pos_mov(posX, posY, movX, movY);
	field->get_square_pos(posX, posY, sqPosX, sqPosY);
	
	bool isFirstSquare = ( point.x == sqPosX && point.y == sqPosY);
	
	//If the block is an obstacle, we cannot go forward (add explosion ?)
	if( (n == INDESTRUCTIBLE || n == BOMB) && !isFirstSquare )
	{ return;}
	
	//We can go to that point, setting dist
	distMap[point.x][point.y] = point.dist;
	
	//if it the block, increase dist (cuz it need a bomb to go foward)
	if(n == BLOCK)
	distMap[point.x][point.y] += BLOCK_COST;
	
	//Adding neighbors to the queue 
	TestPoint left, right, top, bottom;
	
	left.x = point.x - 1;
	left.y = point.y;
	left.dist = distMap[point.x][point.y] + 1;
	
	right.x = point.x + 1;
	right.y = point.y;
	right.dist = distMap[point.x][point.y] + 1;
	
	top.x = point.x;
	top.y = point.y + 1;
	top.dist = distMap[point.x][point.y] + 1;
	
	bottom.x = point.x ;
	bottom.y = point.y - 1;
	bottom.dist = distMap[point.x][point.y] + 1;
		
	toTestQueue.push(left);
	toTestQueue.push(right);
	toTestQueue.push(top);
	toTestQueue.push(bottom);
	
}

bool AICqc::generate_path()
{
	if( path.size() == 0 )
	{
		//Error, should have a value
		return true;
	}
	
	//Start reach
	if( (path[0].x == startX && path[0].y == startY) || distMap[path[0].x][path[0].y] == 0 ) //security
	{
		return true;
	}
	
	//Otherwise, check the surrounding in the map and get the shoret distance
	int dist[4]; //Left right bot top
	for(int i=0; i<4; i++) dist[i] = -1;
	
	if( (0 <= path[0].x-1 && path[0].x-1 < field->get_X_size()  &&  0 <= path[0].y && path[0].y < field->get_Y_size() ) )
	{ dist[0] = distMap[path[0].x-1][path[0].y]; }
	
	if( (0 <= path[0].x+1 && path[0].x+1 < field->get_X_size()  &&  0 <= path[0].y && path[0].y < field->get_Y_size() ) )
	{ dist[1] = distMap[path[0].x+1][path[0].y]; }
	
	if( (0 <= path[0].x && path[0].x < field->get_X_size()  &&  0 <= path[0].y-1 && path[0].y-1 < field->get_Y_size() ) )
	{ dist[2] = distMap[path[0].x][path[0].y-1]; }
	
	if( (0 <= path[0].x && path[0].x < field->get_X_size()  &&  0 <= path[0].y+1 && path[0].y+1 < field->get_Y_size() ) )
	{ dist[3] = distMap[path[0].x][path[0].y+1]; }
	
	int closestPos = 0;
	int prev_dist = 1000;
	
	for(int i=0; i<4; i++)
	{
		if( dist[i] != -1 && dist[i] < prev_dist )
		{
			closestPos = i;
			prev_dist = dist[i];
		}
	}

	//No point available -> error
	if( dist[closestPos] == -1 )
	{
		std::cout<<"Error: corrupted path !"<<std::endl;
		return true;
	}
	
	
	bool bombNeeded = (field->get_square(path[0].x, path[0].y)).get_content() == BLOCK;
	int xToAdd;
	int yToAdd;
	
	if(closestPos == 0)
	{ xToAdd = path[0].x-1; yToAdd = path[0].y; }
	else if(closestPos == 1)
	{ xToAdd = path[0].x+1; yToAdd = path[0].y; }
	else if(closestPos == 2)
	{ xToAdd = path[0].x; yToAdd = path[0].y-1; }
	else //if(closestPos == 3)
	{ xToAdd = path[0].x; yToAdd = path[0].y+1; }
	
	PathPoint newPP ;
	newPP.x = xToAdd;
	newPP.y = yToAdd;
	newPP.toBomb = bombNeeded;
	
	//Inserting at the beginning of the path
	path.insert (path.begin()+0, newPP);
	
	return false;
}

