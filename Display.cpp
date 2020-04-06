#include "Display.h"

Display::Display()
{
	squareEmp = load_image("resource/empty.bmp");
	squareBlo = load_image("resource/block.bmp");
	squareInd = load_image("resource/indes.bmp");
	squareExp = load_image("resource/explo.bmp");
	squareBom = load_image("resource/bomb.bmp");
	
	//Face
	playerFace[0] = load_image("resource/bomberFace.bmp");
	playerFace[1] = load_image("resource/bomberFace1.bmp");
	playerFace[2] = load_image("resource/bomberFace2.bmp");
	
	//Back
	playerBack[0] = load_image("resource/bomberBack.bmp");
	playerBack[1] = load_image("resource/bomberBack1.bmp");
	playerBack[2] = load_image("resource/bomberBack2.bmp");
	
	//Lateral left
	playerLatL[0] = load_image("resource/bomberLatL.bmp");
	playerLatL[1] = load_image("resource/bomberLatL1.bmp");
	playerLatL[2] = load_image("resource/bomberLatL2.bmp");
	
	//lateral right
	playerLatR[0] = load_image("resource/bomberLatR.bmp");
	playerLatR[1] = load_image("resource/bomberLatR1.bmp");
	playerLatR[2] = load_image("resource/bomberLatR2.bmp");
}


Display::~Display()
{
	//SDL_Quit();
}


void Display::display_field(Field F)
{
	//~ std::cout<<"Starting display"<<std::endl;
	
	int maxX = F.get_X_size();
	int maxY = F.get_Y_size();
	int squareSize = F.get_square_size();
	int imageSquareSize = 20;
	
	int ratio = imageSquareSize/squareSize; // surtout utile pour les joueurs
	
	int offset = (imageSquareSize)/2;
	
	//On efface tout
	clean_screen();
	
	//Printing the field
	//~ std::cout<<"Displaying the field"<<std::endl;
	for(int i=0; i<maxX; i++)
	{
	for(int j=0; j<maxY; j++)
	{
		Square S;
		S = F.get_square(i, j);
				
		switch ( S.get_content() )
		{
			case BLOCK:
				//~ printf("Block\n");
				squareBlo.pos.x = i*imageSquareSize + offset;
				squareBlo.pos.y = j*imageSquareSize + offset;
				squareBlo.dir = 0;
				display_image(squareBlo, -250, -255, 255, 255);
				break;
				
			case EMPTY:
				//~ printf("Empty\n");
				squareEmp.pos.x = i*imageSquareSize + offset;
				squareEmp.pos.y = j*imageSquareSize + offset;
				squareEmp.dir = 0;
				display_image(squareEmp, -250, -255, 255, 255);
				break;
			
			case INDESTRUCTIBLE:
				//~ printf("Indestructible\n");
				squareInd.pos.x = i*imageSquareSize + offset;
				squareInd.pos.y = j*imageSquareSize + offset;
				squareInd.dir = 0;
				display_image(squareInd, -250, -255, 255, 255);
				break;
				
			case EXPLOSION:
				//~ printf("Explosion\n");
				squareExp.pos.x = i*imageSquareSize + offset;
				squareExp.pos.y = j*imageSquareSize + offset;
				squareExp.dir = 0;
				display_image(squareExp, -250, -255, 255, 255);
				break;
				
			case BOMB:
				//~ printf("Bomb\n");
				squareBom.pos.x = i*imageSquareSize + offset;
				squareBom.pos.y = j*imageSquareSize + offset;
				squareBom.dir = 0;
				display_image(squareBom, -250, -255, 255, 255);
				break;
		}
	}
	}
	
	//Affichage des bombers
	int nbrBomb = F.get_nbr_bomber();
	Bomber man; //JDMP
	IMAGE player;
	//pos du bomber
	float x;
	float y;
	float mx;
	float my;
	
	for(int i=0; i<nbrBomb; i++)
	{
		man = F.get_bomber(i);
		man.get_pos_mov(x, y, mx, my);
		
		player = select_bomber_image(i, x, y, mx ,my);
		
		//Positionnement de l'image
		player.pos.x = (int)x * ratio;
		player.pos.y = (int)y * ratio;
		player.dir = 0;
		
		if(man.get_HP() <= 0)
		{}
		else if(!man.is_invincible())
		{ 
			display_image(player, 250, 255, 255, 255); 			
		}
		else
		{
			clock_t act = clock();
			clock_t prev = man.get_timeHit();
			float sec = ((float)act - (float)prev)/ (float)CLOCKS_PER_SEC;
			int s = (int)(sec * 100)%20; // pour pouvoir faire un modulo 0.2 en fait
			
			if(s >= 10)
			{display_image(player, 250, 255, 255, 255);}
			else
			{}
		}
	}

	
			
     
	synchro();
	//~ std::cout<<"Display ended"<<std::endl;
	
}

IMAGE Display::select_bomber_image(int i, float x, float y, float mx, float my)
{
	if(i >= (int)bomberData.size() - 1) //on vérifie qu'il existe bien dans le vector, sinon on le crée
	{ 
		AnimData newAD;
		newAD.ang = 0;
		newAD.cmpt = 0; 
		newAD.stepAnimStart = clock();
		bomberData.push_back(newAD);
	}
	
	//If not moving
	if( mx == 0.0f && my == 0.0f )
	{
		bomberData[i].cmpt = 0;
		
		if(bomberData[i].ang == (float)M_PI/2 ) //gauche par rapport au bomber, droite en fait
		{ return playerLatR[0]; }
		else if(bomberData[i].ang == (float)M_PI || bomberData[i].ang == -(float)M_PI) //vers le haut
		{ return playerBack[0]; }
		else if (bomberData[i].ang == -(float)M_PI/2 )  //droite du bomber donc gauche...
		{ return playerLatL[0]; }
		else  //de face
		{ return playerFace[0]; }		
	}
	else
	{
		//On détermine le nouvel angle
		if( my < 0.0f ) //vers le bas, face
		{ bomberData[i].ang = 0; }
		else if (my > 0.0f)//vers le haut
		{ bomberData[i].ang = M_PI;}
		else if (mx < 0.0f) // left
		{ bomberData[i].ang = -M_PI/2;}
		else if (mx > 0.0f) // right
		{ bomberData[i].ang = M_PI/2;}
		else
		{ bomberData[i].ang = 0; }

		
		//Selection de l'étape de l'anim
		clock_t cur = clock();
		float sec = ((float)cur - (float)bomberData[i].stepAnimStart)/ (float)CLOCKS_PER_SEC;
		float step = 0.17f;
		int pos = 0; //image à afficher (moment de l'animation)
		
		if(sec > 4.0f*step)
		{
			bomberData[i].stepAnimStart = clock();
			sec = 0;
		}
		
		if(sec < step)
		{ pos = 0; }
		else if(sec < 2*step)
		{ pos = 1; }
		else if(sec < 3*step)
		{ pos = 0; }
		else if(sec < 4*step)
		{ pos = 2; }
		else
		{ pos = 0; }
		
		
		//Renvoi
		if(bomberData[i].ang == (float)M_PI/2 ) //gauche par rapport au bomber, droite en fait
		{ return playerLatR[ pos ];  }
		else if(bomberData[i].ang == (float)M_PI || bomberData[i].ang == -(float)M_PI) //vers le haut
		{ return playerBack[ pos ]; }
		else if (bomberData[i].ang == -(float)M_PI/2 )  //droite du bomber donc gauche...
		{ return playerLatL[ pos ]; }
		else  //de face
		{ return playerFace[ pos ]; }	
	}
	
}
