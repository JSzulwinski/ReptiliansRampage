/*
=================
cEnemy.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy() : cSprite()
{
	this->enemyVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cEnemy::update(double deltaTime)
{

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->scaleSprite();

	if (currentSpritePos.x > 990 && currentSpritePos.y < 1050)
	{ 
		this->setSpriteTranslation({ -getSpriteTranslation().x, getSpriteTranslation().y });
	}
	else if (currentSpritePos.x < 0 && currentSpritePos.x > -50)
	{
		this->setSpriteTranslation({ -getSpriteTranslation().x, getSpriteTranslation().y });
	}

	//set the srpiteSctive to false when enemy reaches the bottom of the screen

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Enemy position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());

	if (currentSpritePos.y > WINDOW_HEIGHT)
	{
		this->setActive(false);
	}
}

/*
=================================================================
  Sets the velocity for the Enemy
=================================================================
*/
void cEnemy::setenemyVelocity(SDL_Point EnemyVel)
{
	enemyVelocity = EnemyVel;
}
/*
=================================================================
  Gets the Enemy velocity
=================================================================
*/
SDL_Point cEnemy::getenemyVelocity()
{
	return enemyVelocity;
}
