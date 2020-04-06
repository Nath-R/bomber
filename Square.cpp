#include "Square.h"


Square::Square()
{
	content = EMPTY;
	owner = NULL;
	health = 0;
}

Square::Square(nature n)
{
	content = n;
	owner = NULL;
	health = 0;
}


nature Square::get_content()
{
	return content;
}

void Square::set_content(nature n)
{ content = n; }


void Square::set_owner(Entity * E)
{ owner = E; }

Entity * Square::get_owner()
{ return owner; }
