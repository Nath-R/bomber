
/**
 * La fonction principale, à déplacer par la suite
 */
 
 run(Window W)
 {
	bool ended = false;
	 
	Field field (20, 20);
	
	(Bomber *) players[4]; // set in vector 
	
	for(int i=0; i<4; i++)
	{ players[i] = field.addBomber; }
	
	while(!ended)
	{
		for(int i=0; i<4; i++)
		{
			//SDL event or AI
			players[i].update_speed(x, y);
			players[i].isBombing(true);
		}
		
		// Mise à jour, collision destruction etc..
		 
		field.update();
		
		//affichage
		
		//Check score time...
	}
 }
