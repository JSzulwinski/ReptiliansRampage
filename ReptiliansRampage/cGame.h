#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "reptiliansGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	//void spawn(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime, SDL_Renderer* theRenderer);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	static cGame* getInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	bool loop;

	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cSprite spriteMenu;
	cSpaceship theSpaceship;
	cBullet theBullet;
	cSprite Heart1;
	cSprite Heart2;
	cSprite Heart3;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cEnemy*> theEnemies;
	vector<cEnemy*> theBoss;
	vector<cBullet*> theBullets;
	vector<cBullet*> theProjectiles;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextNames;
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Create vector array of button textures
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	// Game objects
	bool up;
	bool down;
	bool right;
	bool left;
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	gameState theGameState;
	btnTypes theBtnType;
	SDL_Rect pos;
	FPoint scale;
	SDL_Rect aRect;
	SDL_Color aColour;
	cTexture* tempTextTexture;
	SDL_Point theAreaClicked;
	
	// Define additional
	int theScore;
	string theStrScore;
	int enemiesDead;
	bool updateScore;
	bool updateLives;
	int lives;
	string theStrLives;
	int enem;
	int boss;
	fstream file;
	string highScore;
	int currentScore;
	string theStrHighScore;
	LPCSTR LPCHigh;
	bool updateEnemies;
	bool gameOver;
	bool projectilePresent;
	int bossHealth;
	bool bossDead;
	bool soundPlayed;
	bool endSoundPlayed;
	bool newHighScore;
};

#endif
