/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
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

	theScore = 0;
	theStrScore = "SCORE:" + to_string(theScore);
	LPCSTR LPCScore = theStrScore.c_str();

	lives = 3;
	theStrLives = "LIVES:" + to_string(lives);
	LPCSTR LPCLives = theStrLives.c_str();

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
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 500, 500 },{ 400, 300 },{ 740, 500 },{ 400, 300 } };
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
	fontList = { "digital", "spaceAge", "galaxy" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/VCR_OSD.ttf", "Fonts/bigmacca.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 35);
	}
	// Create text Textures
	gameTextNames = { "LivesCount", "ScoreValue", "HighScore", "GameOver", "Victory", "Instructions", "Instructions2" };
	gameTextList = { LPCLives, LPCScore, LPCHigh, "GAME OVER", "VICTORY", "Use WASD keys to move your spaceship", "Press  SPACE to shoot" };

	theTextureMgr->addTexture("LivesCount", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ScoreValue", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HighScore", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Victory", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));


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

	theRocket.setSpritePos({ 500, 350 });
	theRocket.setTexture(theTextureMgr->getTexture("spaceship"));
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("spaceship")->getTWidth(), theTextureMgr->getTexture("spaceship")->getTHeight());
	theRocket.setRocketVelocity({ 0, 0 });
	theRocket.setSpriteScale({ 3, 3 });

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

		// Render each asteroid in the vector array
		for (int draw = 0; draw < theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
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
		if (theRocket.isActive())
		{
			theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());
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

	// CHeck Button clicked and change state
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
		endSoundPlayed = false;

		if (soundPlayed == false)
		{
			theSoundMgr->getSnd("playLevel")->play(0);
			soundPlayed = true;
		}
		
		if (updateEnemies == true)
		{
			while (astro <= 5)
			{
				theAsteroids.push_back(new cAsteroid);
				//astro = theAsteroids.size() - 1;
				//theAsteroids[astro]->setSpriteScale({ 0, 0 });
				theAsteroids[astro]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theAsteroids[astro]->setSpriteTranslation({ ((rand() % 600) - 300), 40 });
				//int randAsteroid = rand() % 4;
				theAsteroids[astro]->setTexture(theTextureMgr->getTexture("enemy"));
				theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
				theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
				theAsteroids[astro]->setSpriteScale({ 3, 3 });
				astro++;
			}
			updateEnemies = false;
		}
		
		if (theScore >= 10)
		{
			if (boss <= 0)
			{
				theBoss.push_back(new cAsteroid);
				boss = theBoss.size() - 1;
				theBoss[boss]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theBoss[boss]->setSpriteTranslation({ ((rand() % 600) - 300), 0 });
				theBoss[boss]->setTexture(theTextureMgr->getTexture("boss"));
				theBoss[boss]->setSpriteDimensions(theTextureMgr->getTexture("boss")->getTWidth(), theTextureMgr->getTexture("boss")->getTHeight());
				theBoss[boss]->setAsteroidVelocity({ 3, 3 });
				theBoss[boss]->setSpriteScale({ 6, 6 });
				boss++;
				theSoundMgr->getSnd("bossSpawn")->play(0);
			}
		}

		// Update the visibility and position of each asteriod
		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				++asteroidIterator;
			}
		}

		// Update the visibility and position of Boss
		vector<cAsteroid*>::iterator bossIterator = theBoss.begin();
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
			//(*bulletIterartor)->update(deltaTime);
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*asteroidIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("explosion")->play(0);

					theScore++;
					updateScore = true;
					enemiesDead++;

				}
			}
			

			for (vector<cAsteroid*>::iterator bossIterator = theBoss.begin(); bossIterator != theBoss.end(); ++bossIterator)
			{
				if ((*bossIterator)->collidedWith(&(*bossIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("explosion")->play(0);

					bossHealth--;
					theScore++;
					updateScore = true;
				}
			}
		}

		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
			{
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
				(*asteroidIterator)->setActive(false);
			}
		}

		for (vector<cBullet*>::iterator projectileIterartor = theProjectiles.begin(); projectileIterartor != theProjectiles.end(); ++projectileIterartor)
		{
			if ((*projectileIterartor)->collidedWith(&(*projectileIterartor)->getBoundingRect(), &theRocket.getBoundingRect()))
			{
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
				(*projectileIterartor)->setActive(false);
			}
		}

		if (lives <= 0)
		{

			file.open("Data/scoreboard.txt", ios::in);
			if (file.good() == true)
			{
				getline(file, highScore);
				currentScore = atoi(highScore.c_str());
			}
			file.close();

			if (theScore > currentScore)
			{
				file.open("Data/scoreboard.txt", ios::out);
				file << theScore;
				file.close();
				newHighScore = true;
			}

			theGameState = END;

			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				(*asteroidIterator)->setActive(false);
			}
		}

		if (bossHealth <= 0)
		{
			bossDead = true;
			file.open("Data/scoreboard.txt", ios::in);
			if (file.good() == true)
			{
				getline(file, highScore);
				currentScore = atoi(highScore.c_str());
			}
			file.close();

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

		//----------------------------------------------------------------------------------------
		if (theRocket.isActive())
		{
			SDL_Rect currentPos = theRocket.getSpritePos();
			if (currentPos.x < 0)
			{
				theRocket.setSpritePos({ 1, currentPos.y });
				theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.x > 960)
			{
				theRocket.setSpritePos({ 959, currentPos.y });
				theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.y < 0)
			{
				theRocket.setSpritePos({ currentPos.x, 1 });
				theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}

			if (currentPos.y > 700)
			{
				theRocket.setSpritePos({ currentPos.x , 699 });
				theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}

			// Update the Rockets position
			theRocket.update(deltaTime);
		}

		if (theScore < 10)
		{
			for (enemiesDead; enemiesDead > 0; enemiesDead--)
			{
				theAsteroids.push_back(new cAsteroid);
				astro = theAsteroids.size() - 1;
				//theAsteroids[astro]->setSpriteScale({ 0, 0 });
				theAsteroids[astro]->setSpritePos({ (rand() % 1000 + 1), 10 });
				theAsteroids[astro]->setSpriteTranslation({ ((rand() % 600) - 300), 40 });
				//int randAsteroid = rand() % 4;
				theAsteroids[astro]->setTexture(theTextureMgr->getTexture("enemy"));
				theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
				theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
				theAsteroids[astro]->setSpriteScale({ 3, 3 });
				theSoundMgr->getSnd("enemySpawn")->play(0);
			}
		}

		if (updateScore)
		{
			theTextureMgr->deleteTexture("ScoreValue");
			theStrScore = "SCORE:" + to_string(theScore);
			LPCSTR LPCScore = theStrScore.c_str();
			gameTextList[1] = LPCScore;
			theTextureMgr->addTexture("ScoreValue", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));

		}

		if (updateLives)
		{
			theTextureMgr->deleteTexture("LivesCount");
			theStrLives = "LIVES:" + to_string(lives);
			LPCSTR LPCLives = theStrLives.c_str();
			gameTextList[0] = LPCLives;
			theTextureMgr->addTexture("LivesCount", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }));
		}

		//BOSS SHOOTING__________________________________________________________________________________________________

		if (projectilePresent == false)
		{
			for (vector<cAsteroid*>::iterator bossIterator = theBoss.begin(); bossIterator != theBoss.end(); ++bossIterator)
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
		file.open("Data/scoreboard.txt", ios::in);
		if (file.good() == true)
		{
			getline(file, highScore);

		}
		file.close();

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

		theTextureMgr->deleteTexture("HighScore");
		theStrHighScore = "HIGH SCORE: " + highScore;
		LPCHigh = theStrHighScore.c_str();
		gameTextList[2] = LPCHigh;
		theTextureMgr->addTexture("HighScore", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

		lives = 3;
		theScore = 0;
		theRocket.setSpritePos({ 500, 350 });
		theRocket.setRocketVelocity({ 0, 0 });
		theRocket.setSpriteTranslation({ 0, 0 });
		updateEnemies = true;
		enemiesDead = 0;
		theAsteroids.clear();
		theBoss.clear();
		astro = 0;
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
			SDL_Rect currentPos = theRocket.getSpritePos();


			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;

			case SDLK_d:
			{
				right = true;
				if (up == true)
				{
					theRocket.setSpriteTranslation({ 30, 30 });
				}
				else if (down == true)
				{
					theRocket.setSpriteTranslation({ 30, -30 });
				}
				else
				{
					theRocket.setSpriteTranslation({ 30, 0 });
				}
			}
			break;

			case SDLK_a:
			{
				left = true;
				if (up == true)
				{
					theRocket.setSpriteTranslation({ -30, 30 });
				}
				else if (down == true)
				{
					theRocket.setSpriteTranslation({ -30, -30 });
				}
				else
				{
					theRocket.setSpriteTranslation({ -30, 0 });
				}
			}
			break;

			case SDLK_s:
			{
				up = true;
				if (right == true)
				{
					theRocket.setSpriteTranslation({ 30, 30 });
				}
				else if (left == true)
				{
					theRocket.setSpriteTranslation({ -30, 30 });
				}
				else
				{
					theRocket.setSpriteTranslation({ 0, 30 });
				}
			}
			break;

			case SDLK_w:
			{
				down = true;
				if (right == true)
				{
					theRocket.setSpriteTranslation({ 30, -30 });
				}
				else if (left == true)
				{
					theRocket.setSpriteTranslation({ -30, -30 });
				}
				else
				{
					theRocket.setSpriteTranslation({ 0, -30 });
				}
			}
			break;

			case SDLK_SPACE:
			{
				theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				theBullets[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
				theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theBullets[numBullets]->setBulletVelocity({ 2, 2 });
				theBullets[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
				theBullets[numBullets]->setSpriteScale({ 3, 3 });
				theBullets[numBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << theRocket.getBoundingRect().x << " y: " << theRocket.getBoundingRect().y << endl;
			
				theSoundMgr->getSnd("missleLaunch")->play(0);

			}
			default:
				break;

			}
		}
		break;

		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_d:
			{
				right = false;
				//theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_a:
			{
				left = false;
				//theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_s:
			{
				up = false;
				//theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
			}
			break;

			case SDLK_w:
			{
				down = false;
				//theRocket.setRocketVelocity({ 0, 0 });
				theRocket.setSpriteTranslation({ 0, 0 });
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

