
#include "NetworkModule.h"


///SERVER PART

Server::Server()
{
	listener.SetBlocking(false);
	
	if( !listener.Listen(PORT) )
	{
		std::cout<<"Unable to listen !"<<std::endl;
	}
	
	nbrPlayerMax = 4; //default
}

//Mutateurs

void Server::set_nbrPlayerMax(int max)
{
	nbrPlayerMax = max;
}

//Méthodes standard

void Server::check_connexion()
{
	sf::IPAddress newCliAdr;
	sf::SocketTCP newCliSoc;
	
	
	if( listener.Accept(newCliSoc, &newCliAdr) != sf::Socket::NotReady) //une connection en attente
	{
		ServerClient newCli;
		newCli.IP = newCliAdr;
		newCli.socket = newCliSoc;
		newCli.isConnected = true;
		
		clients.push_back(newCli);
	}
	else //Pas de connection
	{}
}


void Server::receive_input()
{
	//~ for(int i=0; i<nbrPlayerMax; i++)
	//~ { receive_input(i); }
}

void Server::receive_input(int i, float& x, float& y, bool& bombing)
{
	//i dans un intervale possible: i positif, en dessous du nombre de clients et dans les client joueurs
	if(0 <= i && i < nbrPlayerMax && i < (int)clients.size() /*&& clients[i].isConnected */)
	{
		sf::Packet received;
		
		if ( clients[i].socket.Receive( received ) != sf::Socket::Disconnected )
		{
			received >> x >> y >> bombing;
		}
		else
		{
			clients[i].isConnected = false;
		}
	}
}


void Server::send_updated_map( Field F)
{
	sf::Packet toSend;
	toSend<<true; //game not over
	
	int mapX = F.get_X_size();
	int mapY = F.get_Y_size();
	int nbrPlayer = F.get_nbr_bomber();
	
	//send taille de la carte et des zones
	toSend<<mapX<<mapY<<F.get_square_size();
	
	//send map
	for(int i=0; i<mapX; i++)
	for(int j=0; j<mapY; j++)
	{
		toSend<<(int)(F.get_square(i, j).get_content());
	}
	
	//send number of player
	toSend<<nbrPlayer;
	
	//send player (pos+ speed + hp + hit + status? )
	for(int i=0; i<nbrPlayer; i++)
	{
		Bomber  B = F.get_bomber(i);
		float x, y, mx, my;
		B.get_pos_mov(x, y, mx, my);
		
		toSend<< x << y << mx << my << B.get_HP() << B.is_invincible();
	}
	
	//sending
	for(unsigned int i=0; i<clients.size(); i++)
	{
		clients[i].socket.Send(toSend);
		//ajouter une vérif sur la fin
	}
}




///CLIENT PART

Client::Client()
{
	connected = false;
}


bool Client::connect( std::string adrIP  )
{
	if( !client.Connect(PORT, adrIP) )
	{
		std::cout<<"Unable to connect to "<<adrIP<<std::endl;
		return false;
	}
	std::cout<<"connection to "<<adrIP<<" suceeded"<<std::endl;
	return true;
}


void Client::send_input(float x, float y, bool bombing)
{
	sf::Packet toSend;
	toSend << x << y << bombing;
	
	if( client.Send(toSend) == sf::Socket::Disconnected )
	{ connected = false; }
}


void Client::get_updated_map( Field & F )
{
		sf::Packet received;
		
		if ( client.Receive( received ) != sf::Socket::Disconnected )
		{
			bool running;
			received >> running;
			
			if (!running)
			{
				//fin de partie
				std::cout<<"Game ended"<<std::endl;
			}
			
			int mapX, mapY, squareSize;
			
			received >> mapX >> mapY >> squareSize;
			//faire une vérif sur la taille, si différente, on recrée le terrain ? fonction initiale différente ?
			
			//Récuperation de la carte
			nature newCont = BLOCK;
			int newContInt;
			
			for(int i=0; i<mapX; i++)
			for(int j=0; j<mapY; j++)
			{
				received >> newContInt;
				newCont = (nature)newContInt;
				F.set_square_content(i,j,newCont);
			}
			
			
			//Récuperation des joueurs
			int nbrPlayer;
			received >> nbrPlayer;
			//Ajout des bombers manquant
			if(nbrPlayer > F.get_nbr_bomber() )
			{
				F.vector_bombers_size(nbrPlayer);
				for(int i=0; i<(nbrPlayer - F.get_nbr_bomber()); i++ )
				{ F.add_bomber_default(); }
			}
			
			//Mise à jour des positions
			float x, y, mx, my;
			int HP;
			bool invicible;
			
			for(int i=0; i<nbrPlayer; i++)
			{
				received >> x >> y >> mx >> my >> HP >> invicible;
				F.get_bomber(i).set_pos(x, y);
				F.get_bomber(i).update_move(mx, my);
				//HP ?
				F.get_bomber(i).set_invincible(invicible);
			}
			
			
		}
		else
		{
			//game over il faudra passer ended à true
		}
}
