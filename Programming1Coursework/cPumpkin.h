/*
=================
cpumpkin.h
- Header file for class definition - SPECIFICATION
- Header file for the Pumpkin class which is a child of cSprite class
=================
*/
#ifndef _CPUMPKIN_H
#define _CPUMPKIN_H
#include "cSprite.h"

class cPumpkin : public cSprite
{
private:
	SDL_Point pumpkinVelocity;

public:
	cPumpkin();
	void update(double deltaTime);		//  update method
	void setPumpkinVelocity(SDL_Point pumpkinVel);   // Sets the velocity for the pumpkin
	SDL_Point getpumpkinVelocity();				 // Gets the pumpkin velocity
};
#endif