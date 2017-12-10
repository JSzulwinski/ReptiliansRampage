/*
=================
cEnemy.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _cEnemy_H
#define _cEnemy_H
#include "cSprite.h"

class cEnemy : public cSprite
{
private:
	SDL_Point enemyVelocity;
	bool maxRight = false;

public:
	cEnemy();
	void update(double deltaTime);		// Rocket update method
	void setenemyVelocity(SDL_Point EnemyVel);   // Sets the velocity for the Enemy
	SDL_Point getenemyVelocity();				 // Gets the Enemy velocity
};
#endif