#include "MySDL.h"


SDL_Surface *screen = NULL;//on le met ici, si jamais on veut s'en servir ailleurs

int WIDTH, HEIGHT; // variable globale init dans init_wondow, pour être utilisé dans les autres fonctions





/**affichage**/

void init_window(int width, int height, char name[] )//initialise la SDL et la fenetre
{
    WIDTH = width; //on initialise en premier lieu, les variables globales
    HEIGHT = height;

    SDL_Init(SDL_INIT_VIDEO);//init de base

    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);//init de la fenetre

    SDL_WM_SetCaption(name, NULL);//le nom

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));//la couleur

    synchro();//affichage initial

    //SDL_EnableKeyRepeat(10, 10);
}

void clean_screen()//on efface tout: réaffiche couleur de fond initiale
{
     SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));
}

void display_pixel(POSITION pos, int R, int G, int B)//dessine un pixel
{
    SDL_Surface *surface = NULL;

    SDL_Rect P;
    P.x = (int)pos.x;//on place le pixel
    P.y = (int)HEIGHT - pos.y ;//on replace l'ordonné dans un repére où 0,0 est en bas à gauche

    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);//création d'une surface de 1x1 pixel

    SDL_FillRect(surface, NULL, SDL_MapRGB(screen->format, R, G, B));//coloration

    SDL_BlitSurface(surface, NULL, screen, &P);//on colle

    SDL_FreeSurface(surface);//on libére la surface
}

void display_segment(POSITION C, POSITION D, int R, int G, int B)
{
    float a, b;
    POSITION P;

    P.x = C.x;
    P.y = C.y;

    display_pixel(C, R, G, B);//premier pixel, dans le cas où les deux point sont confondu
    display_pixel(D, R, G, B);// dernier point (B)

    if(C.x == D.x)/** segment vertical**/
    {
        while( P.y != D.y)
        {
            display_pixel(P, R, G, B);

            if      (C.y - D.y > 0) {P.y--;}
            else if (C.y - D.y < 0) {P.y++;}
        }
        display_pixel(P, R, G, B);// dernier point (B)
    }

    else /**tout autre cas**/
    {
        //calcul de l'équation de la droite y = ax + b

        a = (C.y - D.y)/(C.x - D.x);
        b = C.y - a * C.x;

        //on trace, selon  x ou y (selon inclinaison)
        /*selon x*/
        if(pow((C.x - D.x),2) >= pow((C.y - D.y),2)) // au carré pour comparer les distance
        {
            while( P.x != D.x )
            {
                display_pixel(P, R, G, B);
                if (C.x - D.x > 0)
                {
                    P.x--;
                    P.y = a*P.x + b;
                }
                else if (C.x - D.x < 0)
                {
                    P.x++;
                    P.y = a*P.x + b;
                }
            }
        }
        /*selon y*/
        else if(pow((C.x - D.x),2) <= pow((C.y - D.y),2))
        {
            while( P.y != D.y )
            {
                display_pixel(P, R, G, B);
                if (C.y - D.y > 0)
                {
                    P.y--;
                    P.x = (P.y - b)/a;
                }
                else if (C.y - D.y < 0)
                {
                    P.y++;
                    P.x = (P.y - b)/a;
                }
            }
        }
    }
}

IMAGE load_image(const char nameBMP[])
{
    IMAGE image; // image à retourner

    image.pict = NULL;
    image.pict = SDL_LoadBMP(nameBMP);
    /*on verifie qu'il n'y a pas eu d'erreur, sinon, on retourne NULL*/
    if(image.pict == NULL)
    {
        //mettre un affichage d'erreur...
        return image;
    }
    //valeur initiale
    image.dir = 0;
    image.pos.x = 0;
    image.pos.y = 0;

    return image;
}

void display_image(IMAGE image, int transp, int R, int G, int B)//affiche une image
//couleur RGB -> couleur transparente, si negatif, pas de transparence
//transp, degré de transparence: 255 = opaque, 0 tranparent, neg rien,
{
    if(image.pict == NULL)
    { return; }// on sort

	//point permettant l'affichage de l'image
    SDL_Rect print;
    SDL_Surface* rotation = NULL;
    
    //transparence de fond  (couleur defini comme transparente)
    if(R>=0 && G>=0 && B>=0)//si neg, pas de transparence
    { SDL_SetColorKey(image.pict, SDL_SRCCOLORKEY, SDL_MapRGB(image.pict->format, R, G, B) );}

    //transparence de l'image elle même
    if(transp >= 0)
    { SDL_SetAlpha(image.pict, SDL_SRCALPHA, transp); }  

    //on crée l'image "tournée"
    rotation = rotozoomSurface (image.pict, image.dir, 1.0, 0);    

    //on place le point pour afficher l'image, en fonction de son centre
    print.x = (int)(image.pos.x - rotation->w/2);
    print.y = (int)(HEIGHT - image.pos.y - rotation->h/2);

    //affichage
    //~ SDL_BlitSurface(rotation, NULL, screen, &print);
    SDL_BlitSurface(image.pict, NULL, screen, &print);

    //supresion de rotation
    SDL_FreeSurface(rotation);
}

void synchro()//màj de l'écran
{ SDL_Flip(screen);}





/**evenements**/

void wait_event()//fonction piqué sur le sdz qui attend un evenement(pour le moment, croix en haut)
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                continuer = 0;
                break;
            case SDL_QUIT:
                continuer = 0;
                break;
        }
    }
}


POSITION get_arrow()
{
    SDL_Event event;
    POSITION back; // valeur de retour
    back.y = 0; back.x = 0;
    static int x = 0, y = 0;

    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        switch (event.key.keysym.sym)
        {
            case SDLK_RIGHT:
                x = 1;
                break;
            case SDLK_LEFT:
                x = -1;
                break;
            case SDLK_UP:
                y = 1;
                break;
            case SDLK_DOWN:
                y = -1;
                break;
            default:;
        }
        else if (event.type == SDL_KEYUP)
        switch (event.key.keysym.sym)
        {
            case SDLK_RIGHT:
                x = 0;
                break;
            case SDLK_LEFT:
                x = 0;
                break;
            case SDLK_UP:
                y = 0;
                break;
            case SDLK_DOWN:
                y = 0;
                break;
            default:;
        }
    }
    back.x = x;
    back.y = y;

    return back;
}

POSITION get_mouse_pos(POSITION P)
{
    SDL_Event event;
    POSITION back;
    back.x = P.x;
    back.y = P.y;

    SDL_PollEvent(&event);

	if(event.type == SDL_MOUSEMOTION)
	{
		back.x = event.motion.x;
		back.y = HEIGHT - event.motion.y;
	}


    return back;
}

int get_close_request()
{
	SDL_Event event;
	
	SDL_PollEvent(&event);
	
	if(event.type == SDL_QUIT)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}


///MySDL Event


EventSDL::EventSDL()
{	
	arrowLeft = false;
	arrowRight = false;
	arrowUp = false;
	arrowDown = false;
	
	spaceBar = false;
	
	exitRequest = false;
	
	mousePos.x = 0.0;
	mousePos.y = 0.0;
}

void EventSDL::update()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
    {
		//ARROWS
        if (event.type == SDL_KEYDOWN)
        switch (event.key.keysym.sym)
        {
            case SDLK_RIGHT:
                arrowRight = true;
                break;
            case SDLK_LEFT:
                arrowLeft = true;
                break;
            case SDLK_UP:
                arrowUp = true;
                break;
            case SDLK_DOWN:
                arrowDown = true;
                break;
            case SDLK_SPACE:
				spaceBar = true;
				break;
            default:;
        }
        else if (event.type == SDL_KEYUP)
        switch (event.key.keysym.sym)
        {
            case SDLK_RIGHT:
                arrowRight = false;
                break;
            case SDLK_LEFT:
                arrowLeft = false;
                break;
            case SDLK_UP:
                arrowUp = false;
                break;
            case SDLK_DOWN:
                arrowDown = false;
                break;
            case SDLK_SPACE:
				spaceBar = false;
				break;
            default:;
        }
        
        //EXIT
		if(event.type == SDL_QUIT)
		{ exitRequest = true; }
		
		//MOUSE
		if(event.type == SDL_MOUSEMOTION)
		{
			mousePos.x = event.motion.x;
			mousePos.y = HEIGHT - event.motion.y; //ALERT: global variable in use here
		}
    }
}

void EventSDL::get_arrow(float& x, float& y)
{
	//x (Left'n right)
	if(arrowRight && arrowLeft)
	{ x = 0; }
	else if(arrowRight)
	{ x = 1; }
	else if(arrowLeft)
	{ x =-1;}
	
	//y (Up'n down)
	if(arrowUp && arrowDown)
	{ y = 0; }
	else if(arrowUp)
	{ y = 1; }
	else if(arrowDown)
	{ y =-1;}
}

bool EventSDL::get_close_request()
{
	return exitRequest;
}

bool EventSDL::press_spaceBar()
{
	return spaceBar;
}

