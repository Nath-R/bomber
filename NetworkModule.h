#ifndef NETMOD_H
#define NETMOD_H

#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

#include "Field.h"

#define PORT 5476

/**
 * Les clients coté serveur
 */
struct ServerClient
{
	sf::IPAddress IP;
	sf::SocketTCP socket;
	bool isConnected;
};
typedef struct ServerClient ServerClient;

/**
 * Classe serveur, qui recoit les infos et qui retransmet la map ensuite
 */
class Server
{
	private:
	
	/**
	 * Socket d'écoute pour les connections.
	 */
	sf::SocketTCP listener;	
	
	/**
	 * Nombre de joueurs maximum, pas de clients connectés !
	 */
	int nbrPlayerMax;
	
	/**
	 * List of all the connected client.
	 */
	std::vector<ServerClient> clients; 
	
	
	public:
	
	/**
	 * Constructeur
	 */
	Server();
	
	//mutateurs
	void set_nbrPlayerMax(int max);
	
	
	/**
	 * Regarde si quelqu'un veut se connecter (vonnection en attente), si c'est le cas, on le rajoute dans la liste.
	 * TODO Rajouter le cas où l'utilisateur est déjà dans la liste, on remplace alors son socket
	 */
	void check_connexion();
	
	/**
	 * Recoit les inputs de tout les joueurs et modifie leurs donnée,
	 * Si rien n'est reçu, on met des valeurs nulle.
	 */
	void receive_input();
	
	/**
	 * 2éme Version.
	 * Recoit les inputs d'un seul joueurs à la demande, le resultat sera mis en reference.
	 * Si rien n'est reçu, on met des valeurs nulle.
	 */
	void receive_input(int i, float& x, float& y, bool& bombing);
	
	/**
	 * Envoie la carte mise à jour à tout les clients.
	 */
	void send_updated_map( Field F);
	
	/**
	 * Envoie les données de terminaison de la partie
	 * Gagnant, scores, etc...
	 */
	void send_end_data();
};





/**
 * Envoie les inputs et receptionne les données de la map.
 */
class Client
{
	private:
	/**
	 * Socket vers le serveur
	 */
	sf::SocketTCP client;
	
	/**
	 * Indique si on est connecté ou non
	 */
	bool connected;
	
	public:
	
	/**
	 * Constructeur
	 */
	Client();
	
	/**
	 * Etablissement de la connection
	 */
	bool connect( std::string adrIP );
	
	/**
	 * Envoie des inputs
	 */
	void send_input(float x, float y, bool bombing);
	 
	 /**
	  * Recupére le nouveau terrain
	  */
	void get_updated_map( Field & F);
};


#endif
