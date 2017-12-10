/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;

/*
=================================================================================
Define Buttons
=================================================================================
*/
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Change values from int to const string, so they can be displayed as text

	theScore = 0;
	theStrScore = "SCORE:" + to_string(theScore);
	LPCSTR LPCScore = theStrScore.c_str();

	lives = 3;
	theStrLives = "LIVES:" + to_string(lives);
	LPCSTR LPCLives = theStrLives.c_str();

	//Define initial conditions

	updateEnemies = true;
	boss = 0;
	bossHealth = 5;

	// Store the textures
	textureName = { "enemy", "bullet", "projectile", "spaceship", "theBackground", "menuBackground", "Title", "boss", "heart" };
	texturesToUse = { "Images\\enemy.png", "Images\\missile.png", "Images\\bullet.png", "Images\\spaceship.png", "Images\\bcg2.jpg", "Images\\bcg2.png", "Images\\TitleImage.png", "Images\\glycon.png", "Images\\heart.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "menu_btn", "play_btn"};
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 500, 500 },{ 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	// Create textures for Game Dialogue (text)
	fontList = { "pixelFont" };
	fontsToUse = { "Fonts/VCR_OSD.ttf"};
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 35);
	}
	// Create text Textures
	gameTextNames = { "LivesCount", "ScoreValue", "HighScore", "GameOver", "Victory", "Instructions", "Instructions2" };
	gameTextList = { LPCLives, LPCScore, LPCHigh, "GAME OVER", "VICTORY", "Use WASD keys to move your spaceship", "Press  SPACE to shoot" };

	theTextureMgr->addTexture("LivesCount", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ScoreValue", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HighScore", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Victory", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions2", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));


	// Load game sounds
	soundList = { "theme", "shot", "explosion", "playLevel", "enemySpawn", "bossSpawn", "Victory", "bestPilot", "firstLifeLost", "secondLifeLost", "gameOver", "click", "missleLaunch", "bossSpit" };
	soundTypes = { MUSIC, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX, SFX };
	soundsToUse = { "Audio/fadex-vivant.wav", "Audio/shot007.wav", "Audio/explosion2.wav", "Audio/rdyForWar.wav", "Audio/enemySpawn.wav", "Audio/bossSpawn.wav", "Audio/Victory.wav", "Audio/highestScoreSound.wav", "Audio/life1Lost.wav", "Audio/life2Lost.wav", "Audio/gameOver.wav", "Audio/click.wav", "Audio/missleLaunch.wav", "Audio/bossSpit.wav"  };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	spriteMenu.setSpritePos({ 0, 0 });
	spriteMenu.setTexture(theTextureMgr->getTexture("menuBackground"));
	spriteMenu.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());

	theSpaceship.setSpritePos({ 500, 350 });
	theSpaceship.setTexture(theTextureMgr->getTexture("spaceship"));
	theSpaceship.setSpriteDimensions(theTextureMgr->getTexture("spaceship")->getTWidth(), theTextureMgr->getTexture("spaceship")->getTHeight());
	theSpaceship.setRocketVelocity({ 0, 0 });
	theSpaceship.setSpriteScale({ 3, 3 });

	//Called to generate different succession of results in the subsequent calls to rand
	srand(time(NULL));
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime, theRenderer);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	
	//Switching between game states
	switch (theGameState)
	{
	case MENU:
	{
		spriteMenu.render(theRenderer, NULL, NULL, spriteMenu.getSpriteScale());

		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("instructions_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("instructions_btn")->render(theRenderer, &theButtonMgr->getBtn("instructions_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("instructions_btn")->getSpritePos(), theButtonMgr->getBtn("instructions_btn")->getSpriteScale());

	}
	break;
	case INSTRUCTIONS:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Putting instructions on screen
		tempTextTexture = theTextureMgr->getTexture("Instructions");
		pos = { 140, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("Instructions2");
		pos = { 265, 250, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());

	}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 740, 675 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		// Render each Enemy in the vector array
		for (int draw = 0; draw < theEnemies.size(); draw++)
		{
			theEnemies[draw]->render(theRenderer, &theEnemies[draw]->getSpriteDimensions(), &theEnemies[draw]->getSpritePos(), theEnemies[draw]->getSpriteRotAngle(), &theEnemies[draw]->getSpriteCentre(), theEnemies[draw]->getSpriteScale());
		}
		//Render boss
		for (int draw = 0; draw < theBoss.size(); draw++)
		{
			theBoss[draw]->render(theRenderer, &theBoss[draw]->getSpriteDimensions(), &theBoss[draw]->getSpritePos(), theBoss[draw]->getSpriteRotAngle(), &theBoss[draw]->getSpriteCentre(), theBoss[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
		// Render each projectile in the vector array
		for (int draw = 0; draw < theProjectiles.size(); draw++)
		{
			theProjectiles[draw]->render(theRenderer, &theProjectiles[draw]->getSpriteDimensions(), &theProjectiles[draw]->getSpritePos(), theProjectiles[draw]->getSpriteRotAngle(), &theProjectiles[draw]->getSpriteCentre(), theProjectiles[draw]->getSpriteScale());
		}
		
		// Render Lives
		tempTextTexture = theTextureMgr->getTexture("LivesCount");
		pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//Render Score
		tempTextTexture = theTextureMgr->getTexture("ScoreValue");
		pos = { 840, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// render the rocket
		if (theSpaceship.isActive())
		{
			theSpaceship.render(theRenderer, &theSpaceship.getSpriteDimensions(), &theSpaceship.getSpritePos(), theSpaceship.getSpriteRotAngle(), &theSpaceship.getSpriteCentre(), theSpaceship.getSpriteScale());
		}
		SDL_RenderPresent(theRenderer);
	}
	break;
	case END:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Render High Score
		tempTextTexture = theTextureMgr->getTexture("HighScore");
		pos = { 380, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render Score
		tempTextTexture = theTextureMgr->getTexture("ScoreValue");
		pos = { 380, 160, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		if (gameOver == true)
		{
			//Render Game Over
			tempTextTexture = theTextureMgr->getTexture("GameOver");
			pos = { 380, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
		else if (gameOver == false)
		{
			//Render Victory 
			tempTextTexture = theTextureMgr->getTexture("Victory");
			pos = { 380, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}

		//Put button sprites on the screen
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);

}

void cGame::update()
{

}

void cGame::update(double deltaTime, SDL_Renderer* theRenderer)
{

	// Check Button clicked and change state
	if (theGameState == MENU)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("instructions_btn")->update(theGameState, INSTRUCTIONS, theAreaClicked);
		theAreaClicked = { 0,0 };
	}
	else if (theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		theAreaClicked = { 0,0 };
	}
	else if (theGameState == PLAYING)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theAreaClicked = { 0,0 };
	}
	else if (theGameState == INSTRUCTIONS)
	{
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		theAreaClicked = { 0,0 };
	}


	if (theGameState == PLAYING)
	{
		//set the conditions for played sounds
		
		endSoundPlayed = false;

		if (soundPlayed == false)
		{
			theSoundMgr->getSnd("playLevel")->play(0);
			soundPlayed = true;
		}
		
		//Spawn enemies at the beginning of the stage
		if (updateEnemies == true)
		{
			while (enem <= 5)
			{
				theEnemies.push_back(new cEnemy);
				//enem = theEnemies.size() - 1;
				//theEnemies[enem]->setSpriteScale({ 0, 0 });
				theEnemies[enem]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theEnemies[enem]->setSpriteTranslation({ ((rand() % 600) - 300), 40 });
				//int randEnemy = rand() % 4;
				theEnemies[enem]->setTexture(theTextureMgr->getTexture("enemy"));
				theEnemies[enem]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
				theEnemies[enem]->setenemyVelocity({ 3, 3 });
				theEnemies[enem]->setSpriteScale({ 3, 3 });
				enem++;
			}
			updateEnemies = false;
		}
		
		//Spawn the boss, when the score reaches 10
		if (theScore >= 10)
		{
			if (boss <= 0)
			{
				theBoss.push_back(new cEnemy);
				boss = theBoss.size() - 1;
				theBoss[boss]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theBoss[boss]->setSpriteTranslation({ ((rand() % 600) - 300), 0 });
				theBoss[boss]->setTexture(theTextureMgr->getTexture("boss"));
				theBoss[boss]->setSpriteDimensions(theTextureMgr->getTexture("boss")->getTWidth(), theTextureMgr->getTexture("boss")->getTHeight());
				theBoss[boss]->setenemyVelocity({ 3, 3 });
				theBoss[boss]->setSpriteScale({ 6, 6 });
				boss++;
				theSoundMgr->getSnd("bossSpawn")->play(0);
			}
		}

		// Update the visibility and position of each enemy
		vector<cEnemy*>::iterator enemyIterator = theEnemies.begin();
		while (enemyIterator != theEnemies.end())
		{
			if ((*enemyIterator)->isActive() == false)
			{
				enemyIterator = theEnemies.erase(enemyIterator);
			}
			else
			{
				(*enemyIterator)->update(deltaTime);
				++enemyIterator;
			}
		}

		// Update the visibility and position of Boss
		vector<cEnemy*>::iterator bossIterator = theBoss.begin();
		while (bossIterator != theBoss.end())
		{
			if ((*bossIterator)->isActive() == false)
			{
				bossIterator = theBoss.erase(bossIterator);
			}
			else
			{
				(*bossIterator)->update(deltaTime);
				++bossIterator;
			}
		}

		// Update the visibility and position of each bullet
		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}
		}
		// Update the visibility and position of each projectile
		vector<cBullet*>::iterator projectileIterartor = theProjectiles.begin();
		while (projectileIterartor != theProjectiles.end())
		{
			if ((*projectileIterartor)->isActive() == false)
			{
				projectileIterartor = theProjectiles.erase(projectileIterartor);
				projectilePresent = false;
			}
			else
			{
				(*projectileIterartor)->update(deltaTime);
				++projectileIterartor;
			}
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->collidedWith(&(*enemyIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and Enemy to false
					(*enemyIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("explosion")->play(0);

					theScore++;
					updateScore = true;
					enemiesDead++;

				}
			}
			
			for (vector<cEnemy*>::iterator bossIterator = theBoss.begin(); bossIterator != theBoss.end(); ++bossIterator)
			{
				if ((*bossIterator)->collidedWith(&(*bossIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and Enemy to false
					(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("explosion")->play(0);

					bossHealth--;
					theScore++;
					updateScore = true;
				}
			}
		}

		//Collision of spaceship with the enemy
		for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->collidedWith(&(*enemyIterator)->getBoundingRect(), &theSpaceship.getBoundingRect()))
			{
				//take live from the player and play an appropriate sound
				lives--;
				if (lives == 2)
				{
					theSoundMgr->getSnd("firstLifeLost")->play(0);
				}
				else if (lives == 1)
				{
					theSoundMgr->getSnd("secondLifeLost")->play(0);
				}
				updateLives = true;
				//destroy an enemy after collision
				(*enemyIterator)->setActive(false);
			}
		}

		//Collision of spaceship with the boss projectile
		for (vector<cBullet*>::iterator projectileIterartor = theProjectiles.begin(); projectileIterartor != theProjectiles.end(); ++projectileIterartor)
		{
			if ((*projectileIterartor)->collidedWith(&(*projectileIterartor)->getBoundingRect(), &theSpaceship.getBoundingRect()))
			{
				//take live from the player and play an appropriate sound
				
				lives--;
				if (lives == 2)
				{
					theSoundMgr->getSnd("firstLifeLost")->play(0);
				}
				else if (lives == 1)
				{
					theSoundMgr->getSnd("secondLifeLost")->play(0);
				}
				updateLives = true;

				//destroy a projectile after collision
				(*projectileIterartor)->setActive(false);
			}
		}

		if (lives <= 0)
		{
			//open file to take a score value out of it
			file.open("Data/scoreboard.txt", ios::in);
			if (file.good() == true)
			{
				getline(file, highScore);
				currentScore = atoi(highScore.c_str());
			}
			file.close();

			//if the present score is higher than the previous high score, replace it in file
			if (theScore > currentScore)
			{
				file.open("Data/scoreboard.txt", ios::out);
				file << theScore;
				file.close();
				newHighScore = true;
			}
			
			theGameState = END;
		}

		if (bossHealth <= 0)
		{
			bossDead = true;

			//open file to take a score value out of it
			file.open("Data/scoreboard.txt", ios::in);
			if (file.good() == true)
			{
				getline(file, highScore);
				currentScore = atoi(highScore.c_str());
			}
			file.close();

			//if the present score is higher than the previous high score, replace it in file
			if (theScore > currentScore)
			{
				file.open("Data/scoreboard.txt", ios::out);
				file << theScore;
				file.close();
				newHighScore = true;
			}
			theGameState = END;
		}
		else
		{
			bossDead = false;
		}

		//Set boundaries for the movement of the spaceship
		if (theSpaceship.isActive())
		{
			SDL_Rect currentPos = theSpaceship.getSpritePos();
			if (currentPos.x < 0)
			{
				theSpaceship.setSpritePos({ 1, currentPos.y });
				theSpaceship.setRocketVelocity({ 0, 0 });
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.x > 960)
			{
				theSpaceship.setSpritePos({ 959, currentPos.y });
				theSpaceship.setRocketVelocity({ 0, 0 });
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.y < 0)
			{
				theSpaceship.setSpritePos({ currentPos.x, 1 });
				theSpaceship.setRocketVelocity({ 0, 0 });
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.y > 700)
			{
				theSpaceship.setSpritePos({ currentPos.x , 699 });
				theSpaceship.setRocketVelocity({ 0, 0 });
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}

			// Update the spaceships position
			theSpaceship.update(deltaTime);
		}

		if (theScore < 10)
		{
			//spawn new enemy after one of the currently present ones dies
			for (enemiesDead; enemiesDead > 0; enemiesDead--)
			{
				theEnemies.push_back(new cEnemy);
				enem = theEnemies.size() - 1;
				theEnemies[enem]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theEnemies[enem]->setSpriteTranslation({ ((rand() % 600) - 300), 40 });
				theEnemies[enem]->setTexture(theTextureMgr->getTexture("enemy"));
				theEnemies[enem]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
				theEnemies[enem]->setenemyVelocity({ 3, 3 });
				theEnemies[enem]->setSpriteScale({ 3, 3 });
				theSoundMgr->getSnd("enemySpawn")->play(0);
			}
		}

		//update the score value and make it possible to display on screen
		if (updateScore)
		{
			theTextureMgr->deleteTexture("ScoreValue");
			theStrScore = "SCORE:" + to_string(theScore);
			LPCSTR LPCScore = theStrScore.c_str();
			gameTextList[1] = LPCScore;
			theTextureMgr->addTexture("ScoreValue", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));

		}

		//update the lives value and make it possible to display on screen
		if (updateLives)
		{
			theTextureMgr->deleteTexture("LivesCount");
			theStrLives = "LIVES:" + to_string(lives);
			LPCSTR LPCLives = theStrLives.c_str();
			gameTextList[0] = LPCLives;
			theTextureMgr->addTexture("LivesCount", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
		}

		//Boss shooting

		if (projectilePresent == false)
		{
			for (vector<cEnemy*>::iterator bossIterator = theBoss.begin(); bossIterator != theBoss.end(); ++bossIterator)
			{
				theProjectiles.push_back(new cBullet);
				int numProjectiles = theProjectiles.size() - 1;
				theProjectiles[numProjectiles]->setSpritePos({ (*bossIterator)->getBoundingRect().x + (*bossIterator)->getSpriteCentre().x, (*bossIterator)->getBoundingRect().y + (*bossIterator)->getSpriteCentre().y });
				theProjectiles[numProjectiles]->setSpriteTranslation({ 30, -30 });
				theProjectiles[numProjectiles]->setTexture(theTextureMgr->getTexture("projectile"));
				theProjectiles[numProjectiles]->setSpriteDimensions(theTextureMgr->getTexture("projectile")->getTWidth(), theTextureMgr->getTexture("projectile")->getTHeight());
				theProjectiles[numProjectiles]->setBulletVelocity({ 2, 2 });
				theProjectiles[numProjectiles]->setSpriteRotAngle((*bossIterator)->getSpriteRotAngle());
				theProjectiles[numProjectiles]->setSpriteScale({ 10, 10 });
				theProjectiles[numProjectiles]->setActive(true);
				cout << "Projectile added to Vector at position - x: " << (*bossIterator)->getBoundingRect().x << " y: " << (*bossIterator)->getBoundingRect().y << endl;
				projectilePresent = true;
				theSoundMgr->getSnd("bossSpit")->play(0);

			}
		}

		//when boss dies, end the game
		if (bossDead == true)
		{
			gameOver = false;
		}
		else
		{
			gameOver = true;
		}
	}

	else if (theGameState == END)
	{
		//when game finishes, open file and take the score out of it
		file.open("Data/scoreboard.txt", ios::in);
		if (file.good() == true)
		{
			getline(file, highScore);

		}
		file.close();

		//compare current score to the highest score to send an appropriate audio message to the player
		if (endSoundPlayed == false)
		{
			if (gameOver == false)
			{
				if (newHighScore == true)
				{
					theSoundMgr->getSnd("bestPilot")->play(0);
				}
				else
				{
					theSoundMgr->getSnd("Victory")->play(0);
				}
			}
			else
			{
				theSoundMgr->getSnd("gameOver")->play(0);
			}
			endSoundPlayed = true;
		}

		//display score
		theTextureMgr->deleteTexture("HighScore");
		theStrHighScore = "HIGH SCORE: " + highScore;
		LPCHigh = theStrHighScore.c_str();
		gameTextList[2] = LPCHigh;
		theTextureMgr->addTexture("HighScore", theFontMgr->getFont("pixelFont")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

		//set all conditions to initial values so the game can be replayed
		lives = 3;
		theScore = 0;
		theSpaceship.setSpritePos({ 500, 350 });
		theSpaceship.setRocketVelocity({ 0, 0 });
		theSpaceship.setSpriteTranslation({ 0, 0 });
		updateEnemies = true;
		enemiesDead = 0;
		theEnemies.clear();
		theBoss.clear();
		theBullets.clear();
		theProjectiles.clear();
		enem = 0;
		boss = 0;
		soundPlayed = false;
		bossHealth = 10;
		newHighScore = false;
	}
}



bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
			}

		case SDL_KEYDOWN:
		{
			SDL_Rect currentPos = theSpaceship.getSpritePos();


			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;


			//in every case, check for the state of other buttons and when they are pressed move diagonally
			case SDLK_d:
			{
				right = true;
				if (up == true)
				{
					theSpaceship.setSpriteTranslation({ 30, 30 });
				}
				else if (down == true)
				{
					theSpaceship.setSpriteTranslation({ 30, -30 });
				}
				else
				{
					theSpaceship.setSpriteTranslation({ 30, 0 });
				}
			}
			break;

			case SDLK_a:
			{
				left = true;
				if (up == true)
				{
					theSpaceship.setSpriteTranslation({ -30, 30 });
				}
				else if (down == true)
				{
					theSpaceship.setSpriteTranslation({ -30, -30 });
				}
				else
				{
					theSpaceship.setSpriteTranslation({ -30, 0 });
				}
			}
			break;

			case SDLK_s:
			{
				up = true;
				if (right == true)
				{
					theSpaceship.setSpriteTranslation({ 30, 30 });
				}
				else if (left == true)
				{
					theSpaceship.setSpriteTranslation({ -30, 30 });
				}
				else
				{
					theSpaceship.setSpriteTranslation({ 0, 30 });
				}
			}
			break;

			case SDLK_w:
			{
				down = true;
				if (right == true)
				{
					theSpaceship.setSpriteTranslation({ 30, -30 });
				}
				else if (left == true)
				{
					theSpaceship.setSpriteTranslation({ -30, -30 });
				}
				else
				{
					theSpaceship.setSpriteTranslation({ 0, -30 });
				}
			}
			break;

			//instantiate a bullet after pressing space
			case SDLK_SPACE:
			{
				theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				theBullets[numBullets]->setSpritePos({ theSpaceship.getBoundingRect().x + theSpaceship.getSpriteCentre().x, theSpaceship.getBoundingRect().y + theSpaceship.getSpriteCentre().y });
				theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theBullets[numBullets]->setBulletVelocity({ 2, 2 });
				theBullets[numBullets]->setSpriteRotAngle(theSpaceship.getSpriteRotAngle());
				theBullets[numBullets]->setSpriteScale({ 3, 3 });
				theBullets[numBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << theSpaceship.getBoundingRect().x << " y: " << theSpaceship.getBoundingRect().y << endl;
			
				theSoundMgr->getSnd("missleLaunch")->play(0);

			}
			default:
				break;

			}
		}
		break;

		//stop the movement of the spaceship after letting the button go
		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_d:
			{
				right = false;
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_a:
			{
				left = false;
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_s:
			{
				up = false;
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_w:
			{
				down = false;
				theSpaceship.setSpriteTranslation({ 0, 0 });
			}
			break;
			}
		}
		}
	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

