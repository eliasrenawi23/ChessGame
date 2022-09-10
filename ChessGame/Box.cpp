#include "Box.h"
#include"Window.h"


Box::Box() {
	

}

Box::Box(int x, int y, int size, SDL_Color boxColor) 
	:x(x), y(y), size(size), boxColor(boxColor){
	
}

void Box::resize(int x, int y, int size)
{
	this->x = x;
	this->y = y;
	this->size = size;
}
Box::~Box() {


}