/*
=================
cSpaceship.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSpaceship.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cSpaceship::cSpaceship() : cSprite()
{
	this->rocketVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cSpaceship::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = 1;
	direction.Y = 1;

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->rocketVelocity.x = this->rocketVelocity.x + direction.X;
	this->rocketVelocity.y = this->rocketVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->rocketVelocity.x * deltaTime;
	currentSpritePos.y += this->rocketVelocity.y * deltaTime;

	this->rocketVelocity.x *= 0.95;
	this->rocketVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());

	//scale the sprite and move its centre 

	this->scaleSprite();
	this->setSpriteCentre({ 24, 0 });
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cSpaceship::setRocketVelocity(SDL_Point rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cSpaceship::getRocketVelocity()
{
	return rocketVelocity;
}


/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	rocketVelocity += spriteVelocityAdd;
//
//	spritePos2D += rocketVelocity * deltaTime;
//
//	rocketVelocity *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theSpaceshipBullets.begin(); bulletIterartor != theSpaceshipBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
//		{
//			if ((*enemyIterator)->collidedWith((*enemyIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and Enemy to false
//				(*enemyIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theSpaceshipBullets.begin();
//	while (bulletIterartor != theSpaceshipBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theSpaceshipBullets.erase(bulletIterartor);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			(*bulletIterartor)->renderCollisionBox();
//			++bulletIterartor;
//		}
//	}
//}