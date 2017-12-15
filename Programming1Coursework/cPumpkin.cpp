/*
=================
cpumpkin.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPumpkin.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPumpkin::cPumpkin() : cSprite()
{
	this->pumpkinVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPumpkin::update(double deltaTime)
{

	this->setSpriteRotAngle(this->getSpriteRotAngle() +(5.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);	
	}
	
	SDL_Rect currentSpritePos = this->getSpritePos();
	if(currentSpritePos.y > 800)
	{
		currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
		currentSpritePos.y = -10;
	}
	else
	{
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y + 0.1;
	}

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
	
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cPumpkin::setPumpkinVelocity(SDL_Point AsteroidVel)
{
	pumpkinVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cPumpkin::getpumpkinVelocity()
{
	return pumpkinVelocity;
}
