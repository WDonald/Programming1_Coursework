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

	// Set filename
	theFile.setFileName("Data/userScore.dat");

	//set variables
	theAreaClicked = { 0, 0 };
	
	theScore = 0;
	numberOfPumpkins = 10;

	// Store the textures
	textureName = { "Pumpkin1", "Pumpkin2", "Pumpkin3", "Pumpkin4","thePlayer","theBackground"};
	texturesToUse = { "Images\\pumpkin1.png", "Images\\pumpkin2.png", "Images\\pumpkin3.png", "Images\\pumpkin4.png", "Images\\ghostSprite.png", "Images\\ghostMansion.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the button textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn"};
	btnTexturesToUse = { "Images/Buttons/Button16.png","Images/Buttons/Button16.png","Images/Buttons/Button16.png","Images/Buttons/Button16.png","Images/Buttons/Button16.png","Images/Buttons/Button16.png" };
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
	fontList = {"Ghoulish" };
	fontsToUse = { "Fonts/Ghoulish.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	// Create text Textures
	gameTextNames = { "TitleTxt", "GameOverTxt", "PlayTxt","QuitTxt","MenuTxt","SaveTxt","LoadTxt","ScoreTxt","ControlTxt" };
	gameTextList = { "Jacks's challenge", "Game Over","Play","Quit","Menu","Save","Load","Score: ", "Use left and right arrow keys to move" };
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("Ghoulish")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	}
	
	// Load game sounds
	soundList = { "theme", "laugh" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/On the day of our death.wav", "Audio/348142__nikomyth__witch-s-laugh.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//create player
	thePlayer.setSpritePos({ 500, 500 });
	thePlayer.setSpriteRotAngle(90);
	thePlayer.setTexture(theTextureMgr->getTexture("thePlayer"));
	thePlayer.setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());
	thePlayer.setPlayerVelocity({ 0, 0 });

	// Create vector array of textures
	for (int pump = 0; pump < 10; pump++)
	{
		thePumpkins.push_back(new cPumpkin);
		thePumpkins[pump]->setSpritePos({ 150 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) }); 
		thePumpkins[pump]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		
		int randPumpkin = rand() % 4;
		thePumpkins[pump]->setTexture(theTextureMgr->getTexture(textureName[randPumpkin]));
		thePumpkins[pump]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randPumpkin])->getTWidth(), theTextureMgr->getTexture(textureName[randPumpkin])->getTHeight());
		thePumpkins[pump]->setPumpkinVelocity({ 3, 3 });
		thePumpkins[pump]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
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
		
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 390 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		//Render Button text
		tempTextTexture = theTextureMgr->getTexture("PlayTxt");
		pos = { 490, 320, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	
		tempTextTexture = theTextureMgr->getTexture("QuitTxt");
		pos = { 490, 410, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		tempTextTexture = theTextureMgr->getTexture("ControlTxt");
		pos = { 200, 500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	
	case PLAYING:
	{
		
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Render each Pumpkin in the vector array
		
		for (int draw = 0; draw < thePumpkins.size(); draw++)
		{
			thePumpkins[draw]->render(theRenderer, &thePumpkins[draw]->getSpriteDimensions(), &thePumpkins[draw]->getSpritePos(), thePumpkins[draw]->getSpriteRotAngle(), &thePumpkins[draw]->getSpriteCentre(), thePumpkins[draw]->getSpriteScale());
		}
		
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//render score
		theTextureMgr->deleteTexture("ScoreTxt");
		scoreString = "Score : " + std::to_string(theScore);
		
		theTextureMgr->addTexture("ScoreTxt", theFontMgr->getFont("Ghoulish")->createTextTexture(theRenderer, scoreString.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		// render the Player
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());

		//render the buttons
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 710, 680 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		theButtonMgr->getBtn("load_btn")->setSpritePos({ 360, 680 });
		theButtonMgr->getBtn("load_btn")->render(theRenderer, &theButtonMgr->getBtn("load_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("load_btn")->getSpritePos(), theButtonMgr->getBtn("load_btn")->getSpriteScale());

		theButtonMgr->getBtn("save_btn")->setSpritePos({ 10, 680 });
		theButtonMgr->getBtn("save_btn")->render(theRenderer, &theButtonMgr->getBtn("save_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("save_btn")->getSpritePos(), theButtonMgr->getBtn("save_btn")->getSpriteScale());
	
		//Render Button Text
		tempTextTexture = theTextureMgr->getTexture("QuitTxt");
		pos = { 810, 700, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("LoadTxt");
		pos = { 450, 700, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("SaveTxt");
		pos = { 90, 700, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	
	case END:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		//Render Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//render score
		theTextureMgr->deleteTexture("ScoreTxt");
		scoreString = "Score : " + std::to_string(theScore);

		theTextureMgr->addTexture("ScoreTxt", theFontMgr->getFont("Ghoulish")->createTextTexture(theRenderer, scoreString.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//Render Messages
		tempTextTexture = theTextureMgr->getTexture("GameOverTxt");
		pos = { 450, 500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//Render Buttons
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 300 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 390 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	
		//Render Button Text
		tempTextTexture = theTextureMgr->getTexture("QuitTxt");
		pos = { 490, 410, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("MenuTxt");
		pos = { 490, 320, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

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

void cGame::update(double deltaTime)
{
	// Check Button clicked and change state
	switch (theGameState)
	{
	case MENU:
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theAreaClicked = { 0, 0 }; // stops running buttons twice.
	}
	break;

	case PLAYING:
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, END, theAreaClicked);
		theGameState = theButtonMgr->getBtn("load_btn")->update(theGameState, LOADMAP, theAreaClicked);


		//Pumpkin movement
		vector<cPumpkin*>::iterator PumpkinIterator = thePumpkins.begin();
		while (PumpkinIterator != thePumpkins.end())
		{
			if ((*PumpkinIterator)->isActive() == false)
			{
				PumpkinIterator = thePumpkins.erase(PumpkinIterator);
			}
			else
			{
				(*PumpkinIterator)->update(deltaTime);
				++PumpkinIterator;
			}
		}

		//keep player on screen
		if (thePlayer.getSpritePos().x > 900)
		{
			thePlayer.setSpriteRotAngle(-90);
			thePlayer.setSpriteTranslation({ 10,5 });
		}

		if (thePlayer.getSpritePos().x < 10)
		{
			thePlayer.setSpriteRotAngle(90);
			thePlayer.setSpriteTranslation({ 10,5 });
		}


		if (theGameState == LOADMAP)
		{
			theGameState = PLAYING;
			theAreaClicked = { 0, 0 };

			
			cout << "Load" << endl;

			// Check file is available
			if (!theFile.openFile(ios::in)) //open file for input output
			{
				cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				fileAvailable = false;
			}
			else
			{
				cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
				fileAvailable = true;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Load", "Score Loaded", NULL);
				theScore = file.loadScoreFromFile(&theFile);
			}

			
		}


		//save
		theGameState = theButtonMgr->getBtn("save_btn")->update(theGameState, SAVEMAP, theAreaClicked);
		if (theGameState == SAVEMAP)
		{
			theGameState = PLAYING;
			theAreaClicked = { 0, 0 };


			// Check file is available
			if (!theFile.openFile(ios::out)) //open file for output
			{
				cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Save fail", "Save Unsuccessful", NULL);
			}
			else
			{
				//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
				cout << "File '" << theFile.getFileName() << "' opened for output!" << endl;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Saved", "Score Saved", NULL);
				cout << "Save" << endl;

				scoreToSave = std::to_string(theScore);
				cout << "scoreToSave " << scoreToSave << " Saved to " << theFile.getFileName() << endl;
				file.writeScoreDataToFile(&theFile, scoreToSave);
			}

		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/

	
			for (vector<cPumpkin*>::iterator PumpkinIterator = thePumpkins.begin(); PumpkinIterator != thePumpkins.end(); ++PumpkinIterator)
			{
				if ((*PumpkinIterator)->collidedWith(&(*PumpkinIterator)->getBoundingRect(), &thePlayer.getBoundingRect()))

				{
					// if a collision set the Pumpkin to false
					(*PumpkinIterator)->setActive(false);
					numberOfPumpkins -= 1;
					theScore += 1;
					theSoundMgr->getSnd("laugh")->play(0);
				}
		}

		//End game when no pumpkins left
		if (numberOfPumpkins <= 0)
		{
			theGameState = END;
		}
		
	}
	break;
	default:
		break;

	case END:
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		theAreaClicked = { 0, 0 }; // stops running buttons twice.
	}
	break;

	// Update the visibility and position of each asteriod
	vector<cPumpkin*>::iterator PumpkinIterator = thePumpkins.begin();
	while (PumpkinIterator != thePumpkins.end())
	{
		if ((*PumpkinIterator)->isActive() == false)
		{
			PumpkinIterator = thePumpkins.erase(PumpkinIterator);
		}
		else
		{
			(*PumpkinIterator)->update(deltaTime);
			++PumpkinIterator;
		}
	}
	
	}

	// Update the Players position
	thePlayer.update(deltaTime);
	//thePumpkin.update(deltaTime);
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
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			
			case SDL_MOUSEMOTION:
			break;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_RIGHT:
					//rotate sprite to right and move
				{
					thePlayer.setSpriteRotAngle(90);
					thePlayer.setSpriteTranslation({ 10, 5 });
				}
				break;

				case SDLK_LEFT:
					//rotate sprite to right and move
				{
					thePlayer.setSpriteRotAngle(-90);
					thePlayer.setSpriteTranslation({10,5});
				}
				break;
				
				default:
					break;
				}
			default:
				break;
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

