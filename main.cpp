


#include <iostream>
#include <fstream>

#include "Field.h"

#include "NetworkModule.h"

#include "Display.h"

#include "AI.h"


#include "MySDL.h"


/**
 * La fonction principale, à déplacer par la suite
 */ 
 void run(Display dm, EventSDL em)
 {
	bool ended = false;	
	 
	//~ Field field (20, 20);
	Field field ("test.map");
	
	
	for(int i=0; i<4; i++)
	{		
		field.add_bomber();
		//players.push_back( field.add_bomber() );
		
		field.get_bomber(i).set_team(i);//free for all
	}
	
	AIDummy ai(0);
	AICqc ai2(2);
		
	while(!ended)
	{
		sleep(0.1);
		
		//Pour davoir combien de joueur sont encore ne vie
		int nbrAlive = 0;
		
		//Event update
		em.update();
		
		for(int i=0; i<4; i++)
		{
			float x = 0;
			float y = 0;
			bool bombing = false;			
			
			//SDL event or AI
			//Joueur
			if(i == 1)
			{
				em.get_arrow(x, y);
				if(em.press_spaceBar())
				{ bombing = true; }
			}
			else if(i == 0)
			{
				//ai.think(&field, x, y, bombing);
			}
			else if(i == 2 && field.get_bomber(2).get_HP() > 0)
			{
				ai2.think(&field, x, y, bombing);
			}
			
			//int x2, y2; //test							
				
			field.get_bomber(i).update_move((float)x, (float)y);
			
			if(bombing)
			{ field.get_bomber(i).is_bombing(true);  }
			
			
			if(field.get_bomber(i).get_HP() > 0)
			{ nbrAlive++; }
			
			
			//~ (players[i])->update_move((float)x, (float)y);
			//~ (players[i])->is_bombing(false);
		}
		
		// Mise à jour, collision destruction etc..		 
		field.update();
		
		//affichage
		dm.display_field(field);
		
		//Check score time...
		if(nbrAlive < 0)
		{ 
			ended = true;
			std::cout<<"Draw"<<std::endl;
		}
		else if (nbrAlive == 1) //basic (no team)
		{
			ended = true;
			for(int i=0; i<4; i++)
			{
				if(field.get_bomber(i).get_HP() > 0)
				{std::cout<<"Team "<< field.get_bomber(i).get_team() <<" won !"<<std::endl;}
			}
		}		
		
		//exit test
		if (em.get_close_request())
        {exit(0);}
	}
	
	//Fin
	
	sleep(1);
 }


/**
 * Serveur
 */
 void runServer(Display dm, EventSDL em)
 {
	bool ended = false;	
	
	///A mettre au dessus ! et à faire passer en paramétre
	Server server;
	
	//~ Field field (20, 20);
	Field field ("test.map");
	//transmission de la map initiale
	server.send_updated_map( field );
	
	for(int i=0; i<4; i++)
	{		
		field.add_bomber();
		//players.push_back( field.add_bomber() );
		
		field.get_bomber(i).set_team(i);//free for all
	}
		
	while(!ended)
	{
		sleep(0.1);
		
		//On regarde si quelqu'un veut se connecter
		server.check_connexion();
		
		//Pour davoir combien de joueur sont encore ne vie
		int nbrAlive = 0;
		
		//Event update
		em.update();
		
		for(int i=0; i<4; i++)
		{
			float x = 0;
			float y = 0;
			bool bombing = false;			

			//Reception des données ou AI
			server.receive_input(i, x, y, bombing);							
			
			field.get_bomber(i).update_move((float)x, (float)y);
			
			if(bombing)
			{ field.get_bomber(i).is_bombing(true);  }
			
			
			if(field.get_bomber(i).get_HP() > 0)
			{ nbrAlive++; }
		}
		
		// Mise à jour, collision destruction etc..		 
		field.update();
		
		//affichage, optionnel (à terme, supprimé)
		dm.display_field(field);		

		
		//Check score time...
		if(nbrAlive < 0)
		{ 
			ended = true;
			std::cout<<"Draw"<<std::endl;
		}
		else if (nbrAlive == 1) //basic (no team)
		{
			ended = true;
			for(int i=0; i<4; i++)
			{
				if(field.get_bomber(i).get_HP() > 0)
				{std::cout<<"Team "<< field.get_bomber(i).get_team() <<" won !"<<std::endl;}
			}
		}		
		
		//Transmission du nouveau terrain avec résultat
		server.send_updated_map( field );	
		
		//server.send_end_data();	
		
		//exit test (à enlever par la suite)
		if (em.get_close_request())
        {exit(0);}
	}
	
	//Fin
	
	sleep(1);
 }

/**
 * La fonction principale, version client
 */ 
 void runClient(Display dm, EventSDL em)
 {
	bool ended = false;	
	 
	//init the field
	Field field (20, 20); 
	
	Client nm;

	//connexion (avant ?)
	nm.connect("127.0.0.1");
	
		
	while(!ended)
	{
		sleep(0.1);
		
		float x = 0;
		float y = 0;
		bool bombing = false;
		
		//Event update
		em.update();
		
		em.get_arrow(x, y);
		if(em.press_spaceBar())
		{ bombing = true; }
		
		//transmission de la modif
		nm.send_input(x, y, bombing);
		
		
		// Obtention sur le réseau de la nouvelle version du terrain
		nm.get_updated_map( field );	 
		//~ field.update();
		//~ ended = ?
		
		//affichage
		dm.display_field(field);
		//Check score time...
		
		
		//exit test
		if (em.get_close_request()) //deconnection
        {exit(0);}
	}
	
	//Fin
	
	sleep(1);
 }


/**
 * Fonction de test et de debug pour le format fichier des maps
 */
void generate_map(std::string fileName)
{
	std::ofstream file;
	file.open(fileName.c_str(), std::ofstream::in);
	
	//size x y en cases
	int X = 20;
	int Y = 20;
	
	file << (int)X ;
	file << " ";
	file << (int)Y ;
	file << "\n";
	
	//Contenu
	int content = 0;
	
	for(int i=0; i<X; i++)
	for(int j=0; j<Y; j++)
	{
		if( i%2 != 0 && j%2 != 0 )
		{ content = 1; }
		else
		{ content = 0; }
		
		file << content;
		file << " ";
	}	
	
	file.close();
	 
}


int main(int argc, char **argv)
{
	EventSDL eventManager;
	
	Display displayManager;
	
	if(argc > 1)
	{
		if( strcmp( argv[1], "-s") == 0 )
		{
			std::cout<<"Server mode"<<std::endl;
			printf("Creating the window\n");
			init_window(400, 400, (char*)"Bomber Server");
			
			runServer(displayManager, eventManager);
		}
		else if ( strcmp( argv[1], "-c") == 0 )
		{
			std::cout<<"client mode"<<std::endl;
			printf("Creating the window\n");
			init_window(400, 400, (char*)"Bomber Client");
			
			runClient(displayManager, eventManager);
		}
	}
	else
	{
		std::cout<<"single player mode"<<std::endl;
		printf("Creating the window\n");
		init_window(400, 400, (char*)"Bomber");
		
		//generate_map("test.map");
		
		run(displayManager, eventManager);
	}
	
	SDL_Quit();
	
	std::cout<<"Program succesfully ended"<<std::endl;
	return 0;
}

