/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cAsteroid::cAsteroid() : cSprite()
{
	this->asteroidVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cAsteroid::update(double deltaTime)
{

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->scaleSprite();

	if (currentSpritePos.x > 990 && currentSpritePos.y < 1050)
	{ 
		this->setSpriteTranslation({ -getSpriteTranslation().x, getSpriteTranslation().y });
		//this->maxRight = true;
	}
	else if (currentSpritePos.x < 0 && currentSpritePos.x > -50)
	{
		this->setSpriteTranslation({ -getSpriteTranslation().x, getSpriteTranslation().y });
		//this->maxRight = false;
	}

	/*
	if (this->maxRight == false)
	{
		//this->setSpriteTranslation({ 0, 0 });
		this->setSpriteTranslation({ -getSpriteTranslation().x, 60 });
	}
	if (this->maxRight == true)
	{
		//this->setSpriteTranslation({ 0, 0 });
		this->setSpriteTranslation({ -getSpriteTranslation().x, 60 });
	}	*/

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Asteroid position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());

	if (currentSpritePos.y > WINDOW_HEIGHT)
	{
		this->setActive(false);
	}
}

/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(SDL_Point AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}
