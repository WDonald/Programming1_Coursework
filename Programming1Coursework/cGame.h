#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "spookyGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
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
	cPlayer thePlayer;
	cPumpkin thePpumpkin;
	
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cPumpkin*> thePumpkins;
	
	int theScore;
	string scoreString;
	string scoreToSave;

	int numberOfPumpkins;

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
	
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	
	SDL_Point theAreaClicked;
	gameState theGameState;
	cTexture* tempTextTexture;
	btnTypes theBtnType;
	SDL_Rect pos;
	FPoint scale;
	cFileHandler theFile;
	cFileMgr file;
	bool fileAvailable;
};

/*
SDL_Rect aRect;
SDL_Color aColour;
*/


#endif
