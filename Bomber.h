#ifndef BOMBER_H
#define BOMBER_H

#include "Entity.h"
#include "Bomb.h"
#include <vector>

class Bomb;


/**
 * Si autre que bomberman, héritage ?
 */
class Bomber : public Entity
{
	private:
	
	/**
	 * Taille du bomberMan
	 */
	int size;
	
	/**
	 * Vitesse de déplacement
	 */
	int speed;	
	
	/**
	 * Nombre max de bombe
	 */
	int maxBomb;
	
	/**
	 * Va pour poser une bombe
	 */
	bool isBombing;
	
	/**
	 * Les bombes en elle même
	 */
	std::vector<Bomb*> bombs;
	
	/**
	 * Quand on est touché
	 */
	bool isInvincible;
	
	/**
	 * Le moment où on est touché, pour savoir jusqu'à quand on est invincible
	 */
	clock_t timeHit;
	
	/**
	 * Durée de l'invincibilité
	 */
	float timeInvincible;		
	
	/**
	 * Point de vie
	 */	 
	int HP;
	
	/**
	 * Etat du bomber. (int ?)
	 */
	int status;	
	
	//Stat
	
	/**
	 * Nombre de bombre posé
	 */
	int nbrBombSet;
	
	
	
	
	public:
	
	Bomber();
	Bomber(float x, float y);
	
	/**
	 * Mutateur
	 */
	void is_bombing(bool val);
	
	void add_bomb(Bomb * B);
	
	void remove_bomb(int i);
	
	void set_invincible(bool b);
	
	
	/**
	 * Accesseur 
	 */
	int get_moving_speed();
	
	int get_size();
	
	int get_nb_bomb();
	
	Bomb * get_bomb(int i);
	
	bool is_bombing();
	
	bool is_invincible();
	float get_invincible_duration();
	clock_t get_timeHit();
	
	int get_HP();
	
	int get_nbrBombSet();
	
	/**
	 * Touché, passe invincible, reinit timeHit et  enléve point de vie
	 */
	 void damage();
};

#endif
