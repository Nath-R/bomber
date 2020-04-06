#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
	protected:
	
	float posX;
	float posY;
	
	/**
	 * Indique le sens du mouvement
	 */
	float movX;
	float movY;
	
	/**
	 * equipe du de l'entité (bomber, bomb, etc...), 0 pas d'équipe, >0 num de l'équipe
	 */
	 int team;
	
	public:
	
	Entity();
	Entity(float X, float Y);
	
	void update_move(int x, int y);
	void update_move(float x, float y);
	
	
	void set_pos(float x, float y);
	
	void get_pos_mov(float &px, float &py, float &mx, float &my);
	
	
	void set_team(int i);	
	
	int get_team();
};


#endif
