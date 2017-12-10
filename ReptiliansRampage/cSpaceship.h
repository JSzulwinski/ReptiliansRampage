/*
=================
cSpaceship.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _cSpaceship_H
#define _cSpaceship_H
#include "cSprite.h"

class cSpaceship : public cSprite
{
private:
	SDL_Point rocketVelocity;

public:
	cSpaceship();
	void update(double deltaTime);		// Rocket update method
	void setRocketVelocity(SDL_Point rocketVel);   // Sets the velocity for the rocket
	SDL_Point getRocketVelocity();				 // Gets the rocket velocity
};
#endif