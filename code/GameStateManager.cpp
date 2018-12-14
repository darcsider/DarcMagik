/*=====================================================================================
$File: GameStateManager.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "GameStateManager.h"

BannerParadeState::BannerParadeState()
{
	// does nothing currently
	// TODO: Figure out if this method needs anything
}

BannerParadeState::BannerParadeState(GameStateManager *stateManager, string filename)
{
	// set the delay of how long each banner should display before next one
	m_bannerDelay = 2.0f;
	// set the text file used to build the state
	m_textFile = filename;
	// set the pointer to the stateManager
	m_stateManager = stateManager;
	// Build all banners from text file
	BuildState();
}

BannerParadeState::~BannerParadeState()
{
	// does nothing currently
	// TODO: Figure out if this method needs anything
}

void BannerParadeState::BuildState()
{
	string tempString;		// string to hold line from text file
	string filename;		// string to hold filename of texture
	string texture;			// string to hold texture name of banner
	bool skip;				// bool holds if this banner can be skipped
	int numberOfBanners;	// how many banners are there

	ifstream inFile(m_textFile);
	if (inFile)
	{
		// read in a line
		getline(inFile, tempString);
		// convert it to an integer and set it to NumberOfBanners
		numberOfBanners = atoi(tempString.c_str());

		// loop through the banners adding each one to the container
		for (int i = 0; i < numberOfBanners; i++)
		{
			// read in the file name
			getline(inFile, filename);
			// read in the texture name
			getline(inFile, texture);
			// read in the skip option and set variable based on what its value is
			getline(inFile, tempString);
			// set the skip option to true or false depending on the value in textFile
			if (tempString == "TRUE")
			{
				skip = true;
			}
			else
			{
				skip = false;
			}

			// insert the banner in the container and add the texture to the RenderManager container
			m_banners.insert(pair<string, bool>(texture, skip));
			RenderManager::GetInstance().AddTexture(filename, texture);
		}
	}
	inFile.close();

	// get the first banner in map and set currentBanner equal to it
	map<string, bool>::iterator bannerIterator;
	bannerIterator = m_banners.begin();
	m_currentBanner = bannerIterator->first;

}

void BannerParadeState::InputCallBack(bool pressed, GameActions action)
{
	// TODO(Jamie): Add input to banner parade
}

void BannerParadeState::SetupInput()
{
	// clear input container to make sure no other input is currently set
	InputManager::GetInstance().ClearFunctionPointers();
}

void BannerParadeState::Update(float delta)
{
	// update bannerTimer with time change from timer
	m_bannerTimer += delta;

	// if the bannerTimer is greater than delay
	if (m_bannerTimer >= m_bannerDelay)
	{
		// clear bannerTimer
		m_bannerTimer -= m_bannerDelay;

		// check to see if the current banner is last banner or not
		auto bannerIndex = m_banners.find(m_currentBanner);
		// if its not the end then increment and grab next banner
		if (bannerIndex != m_banners.end())
		{
			bannerIndex++;
			if (bannerIndex != m_banners.end())
			{
				m_currentBanner = bannerIndex->first;
			}
			// if its the end of the banners then move the state to next state
			else
			{
				//m_stateChange("MainMenu");
				m_stateManager->ChangeState("MainMenu");
			}
		}
	}
}

void BannerParadeState::Execute()
{
	// draw the currentBanner to the screen
	RECT destRect;
	destRect.left = 0;
	destRect.top = 0;
	destRect.right = RenderManager::GetInstance().GetGameWidth();
	destRect.bottom = RenderManager::GetInstance().GetGameHeight();

	// texture description to get texture dimensions
	D3D11_TEXTURE2D_DESC textureDesc;

	// grab the dimensions for the current banner
	textureDesc = RenderManager::GetInstance().GetTextureDesc(m_currentBanner);

	// fill Rectangle for the dimensions of the texture
	RECT sourceRect;
	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.right = textureDesc.Width;
	sourceRect.bottom = textureDesc.Height;

	// draw the banner to the screen
	RenderManager::GetInstance().RenderObject(m_currentBanner, sourceRect, destRect);
}

void BannerParadeState::OnEnter()
{
	SetupInput();
}

void BannerParadeState::OnExit()
{
	// TODO: Figure out if this method needs anything
}

MainMenuState::MainMenuState()
{
	// default constructor left blank for now
	// TODO: Figure out if this method needs anything
}

MainMenuState::MainMenuState(GameStateManager *stateManager, string filename)
{
	// set the function pointer to change state
	//m_stateChange = funcPoint;
	// set anykey to false
	anyKeyPressed = false;
	// the state is not initialized yet
	initialized = false;
	// set the textFile for the state
	m_textFile = filename;
	// set the pointer to the stateManager
	m_stateManager = stateManager;
	// build the state with the text file passed in
	BuildState();
}

MainMenuState::~MainMenuState()
{
	// destructor left blank for now
	// TODO: Figure out if this method needs anything
}

void MainMenuState::BuildState()
{
	string file;		// filename for texture being read in
	string texture;		// texture name for texture being read in

	ifstream inFile(m_textFile);
	if (inFile)
	{
		// read a line in and set to file
		getline(inFile, file);
		// read a line in and set to texture
		getline(inFile, texture);

		// set the mainmenubackground equal to first texture passed in
		m_mainMenuBackground = texture;
		// add the texture to the container in RenderManager
		RenderManager::GetInstance().AddTexture(file, texture);

		// read in the next texture
		getline(inFile, file);
		getline(inFile, texture);

		// set the current texture equal to pressanykey
		m_pressAnyKey = texture;
		// add the texture to the container in RenderManager
		RenderManager::GetInstance().AddTexture(file, texture);

		// read in remaining textures for the main menu
		for (int i = 0; i < 8; i++)
		{
			getline(inFile, file);
			getline(inFile, texture);

			m_menuTextures.push_back(texture);
			RenderManager::GetInstance().AddTexture(file, texture);
		}
	}
	inFile.close();
}

void MainMenuState::InputCallBack(bool pressed, GameActions action)
{
	// check for input and change the curerntly selected menu option
	if (pressed && action == ActionUp)
	{
		if (anyKeyPressed)
		{
			if (m_currentSelection == 0)
				m_currentSelection = 3;

			else
				m_currentSelection--;
		}
		else
		{
			anyKeyPressed = true;
		}
	}

	else if (pressed && action == ActionDown)
	{
		if (pressed)
		{
			if (anyKeyPressed)
			{
				if (m_currentSelection == 3)
					m_currentSelection = 0;
				else
					m_currentSelection++;
			}
			else
			{
				anyKeyPressed = true;
			}
		}
	}

	if (pressed && action == ActionAccept)
	{
		if (anyKeyPressed)
		{
			if (m_currentSelection == 0)
				m_stateManager->ChangeState("GameStart");
			//m_stateChange("NewGame");

			if (m_currentSelection == 1)
				m_stateManager->ChangeState("GameStart");
			//m_stateChange("LoadGame");

			if (m_currentSelection == 2)
				m_stateManager->ChangeState("GameStart");
			//m_stateChange("Options");

			if (m_currentSelection == 3)
				m_stateManager->ChangeState("GameStart");
			//m_stateChange("OnExit");
		}
		else
		{
			anyKeyPressed = true;
		}
	}
}

void MainMenuState::SetupInput()
{
	// clear the current InputManager input to make sure only this input is in there
	InputManager::GetInstance().ClearFunctionPointers();
	// add input actions to the Input Manager
	function<void(bool, GameActions)> inputFunctionPointer = bind(&MainMenuState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionAccept, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionUp, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionDown, inputFunctionPointer);
}

void MainMenuState::Update(float delta)
{
	// clear the current set of textures to render to screen at beginning of update
	m_renderTextures.clear();

	// depending on what menu option is selected set which textures to render to the screen
	if (m_currentSelection == 0)
	{
		m_renderTextures.push_back(m_menuTextures.at(0));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 1)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(2));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 2)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(4));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 3)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(6));
	}
	initialized = true;
}

void MainMenuState::Execute()
{
	// if the state has been initialized then start rendering to the screen
	if (initialized)
	{
		// texture description to get texture dimensions
		D3D11_TEXTURE2D_DESC textureDesc;

		// set the Rectangle to draw to
		RECT destRect;
		RECT sourceRect;
		destRect.left = 0;
		destRect.top = 0;
		destRect.right = RenderManager::GetInstance().GetGameWidth();
		destRect.bottom = RenderManager::GetInstance().GetGameHeight();

		// calculate the starting x and y to begin rendering to and update accordingly
		vector<string>::iterator textureIterator;
		int width = RenderManager::GetInstance().GetTextureDesc(m_renderTextures.at(0).c_str()).Width;
		int height = RenderManager::GetInstance().GetTextureDesc(m_renderTextures.at(0).c_str()).Height;
		float startX = (float)(RenderManager::GetInstance().GetGameWidth() / 2);
		float startY = (float)(RenderManager::GetInstance().GetGameHeight() / 2);
		float currX = startX - (width / 2);
		float currY = startY;

		// grab the dimensions for the current banner
		textureDesc = RenderManager::GetInstance().GetTextureDesc(m_mainMenuBackground);

		// source Rectangle for the texture being displayed.
		sourceRect.left = 0;
		sourceRect.top = 0;
		sourceRect.right = textureDesc.Width;
		sourceRect.bottom = textureDesc.Height;

		// draw the background to the screen first
		RenderManager::GetInstance().RenderObject(m_mainMenuBackground, sourceRect, destRect);
		// if input has already been received then start looping through the renderTextures and draw to screen
		if (anyKeyPressed)
		{
			for (textureIterator = m_renderTextures.begin(); textureIterator != m_renderTextures.end(); textureIterator++)
			{
				RenderManager::GetInstance().RenderObject((textureIterator)->c_str(), Vector2(currX, currY));
				width = RenderManager::GetInstance().GetTextureDesc((textureIterator)->c_str()).Width;
				currX = startX - (width / 2);
				currY += height;
			}
		}
		// if no key has been pressed yet just display background and press any key
		else
		{
			RenderManager::GetInstance().RenderObject(m_pressAnyKey, Vector2((float)(RenderManager::GetInstance().GetGameWidth() / 3), startY));
		}
	}
}

void MainMenuState::OnEnter()
{
	SetupInput();
}

void MainMenuState::OnExit()
{
	// TODO: Figure out if this method needs anything
}

GameStartState::GameStartState()
{
	// default constructor left blank currently
}

GameStartState::GameStartState(GameStateManager *stateManager, string filename)
{
	// set the filename for the state
	m_textFile = filename;
	// set the pointer to the stateManager
	m_stateManager = stateManager;
	// Build the new Game State
	BuildState();
}

GameStartState::~GameStartState()
{
	// TODO: Figure out if this method needs anything
}

void GameStartState::BuildState()
{
	// NewGame State is not what it will be when final
	ifstream inFile(m_textFile);
	int numberOfTextures;
	string tempString;
	string filename;
	string resourceName;
	string mapFileName;

	if (inFile)
	{
		getline(inFile, tempString);
		numberOfTextures = atoi(tempString.c_str());

		for (int i = 0; i < numberOfTextures; i++)
		{
			getline(inFile, filename);
			getline(inFile, resourceName);
			RenderManager::GetInstance().AddTexture(filename, resourceName);
		}
		getline(inFile, mapFileName);
	}
	inFile.close();

	testWorld = new GameWorld();
	testWorld->BuildMaps(mapFileName);
	testWorld->SetCurrentMap("testMap1");

	Console::GetInstance().Initialize(Vector2(0, 0), RenderManager::GetInstance().GetGameWidth(), 200, XMFLOAT4(0, 0, 0, 0.8f));
	box = new RPGDialogBox();
	//box->BuildDialogBox();
	//box->SetWindow(screen);
	/*vector<string> text;
	text.push_back("test string, how does this display on the screen.");
	text.push_back("Line 2 how does this display on the screen.");
	text.push_back("Line 3 how does this display on the screen.");
	text.push_back("Line 4 how does this display on the screen.");
	text.push_back("test string, how does this display on the screen.");
	box->BuildDialogBox(text);*/
}

void GameStartState::InputCallBack(bool pressed, GameActions action)
{
	if (pressed)
	{
		/*if (action == DirectionMoveLeft)
		{
			testWorld->GetCurrentMap()->UpdateMapVelocity("X", -1.0f);
		}

		if (action == DirectionMoveRight)
		{
			testWorld->GetCurrentMap()->UpdateMapVelocity("X", 1.0f);
		}

		if (action == DirectionMoveUp)
		{
			testWorld->GetCurrentMap()->UpdateMapVelocity("Y", -1.0f);
		}

		if (action == DirectionMoveDown)
		{
			testWorld->GetCurrentMap()->UpdateMapVelocity("Y", 1.0f);
		}*/
		if (action == SystemConsole)
		{
			//box->DisplayToScreen();
		}
	}
	else
	{
		//testWorld->GetCurrentMap()->UpdateMapVelocity("both", 0.0f);
	}
}

void GameStartState::SetupInput()
{
	InputManager::GetInstance().ClearFunctionPointers();

	function<void(bool, GameActions)> closeGame = bind(&GameWorld::CloseGame, testWorld, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemExitEarly, closeGame);

	function<void(bool, GameActions)> moveMap = bind(&GameStartState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(CharacterAttack, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveLeft, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveRight, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveUp, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveDown, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemConsole, moveMap);
}

void GameStartState::Update(float delta)
{
	//testWorld->UpdateCurrentMap(delta);
}

void GameStartState::Execute()
{
	//testWorld->RenderCurrentMap();
	//box->DisplayDialogBox();
	box->BuildDialogBox();
}

void GameStartState::OnEnter()
{
	SetupInput();
}

void GameStartState::OnExit()
{
	// TODO: Figure out if this method needs anything
}

CoreGameState::CoreGameState()
{
	// default constructor left blank currently
}

CoreGameState::CoreGameState(GameStateManager *stateManager, string filename)
{
	// set the filename for the state
	m_textFile = filename;
	// set the pointer to the stateManager
	m_stateManager = stateManager;
	// Build the new Game State
	BuildState();
}

CoreGameState::~CoreGameState()
{
	// TODO: Figure out if this method needs anything
}

void CoreGameState::BuildState()
{
	// NewGame State is not what it will be when final
	ifstream inFile(m_textFile);
	int numberOfTextures;
	string tempString;
	string filename;
	string resourceName;
	string mapFileName;

	if (inFile)
	{
		getline(inFile, tempString);
		numberOfTextures = atoi(tempString.c_str());

		for (int i = 0; i < numberOfTextures; i++)
		{
			getline(inFile, filename);
			getline(inFile, resourceName);
			RenderManager::GetInstance().AddTexture(filename, resourceName);
		}
		getline(inFile, mapFileName);
	}
	inFile.close();

	m_currentWorld = new GameWorld();
	m_currentWorld->BuildMaps(mapFileName);
	m_currentWorld->SetCurrentMap("testMap1");
}

void CoreGameState::InputCallBack(bool pressed, GameActions action)
{
	if (pressed)
	{

	}
	else
	{

	}
}

void CoreGameState::SetupInput()
{
	InputManager::GetInstance().ClearFunctionPointers();

	function<void(bool, GameActions)> closeGame = bind(&GameWorld::CloseGame, m_currentWorld, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemExitEarly, closeGame);

	function<void(bool, GameActions)> moveMap = bind(&CoreGameState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(CharacterAttack, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveLeft, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveRight, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveUp, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveDown, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemConsole, moveMap);
}

void CoreGameState::Update(float delta)
{

}

void CoreGameState::Execute()
{

}

void CoreGameState::OnEnter()
{
	SetupInput();
}

void CoreGameState::OnExit()
{
	// TODO: Figure out if this method needs anything
}

GameStateManager::GameStateManager()
{
	// left blank for now
	// TODO: Figure out if this method needs anything
}

GameStateManager::~GameStateManager()
{
	// left blank for now
	// TODO: Figure out if this method needs anything
}

void GameStateManager::ChangeState(string state)
{
	// make sure the state type trying to change to has a value
	if (!state.empty())
	{
		// search the game states for that state
		auto gameState = m_gameStates.find(state);

		// if there is a state with that name
		if (gameState != m_gameStates.end())
		{
			// then set the currentState to that state
			m_currentState = gameState->second;
			m_currentState->OnEnter();
		}
	}
}

void GameStateManager::AddState(string stateName, GameState* newState)
{
	// make sure both the name and the state has a real value
	if (!stateName.empty() && newState != nullptr)
	{
		// insert into map
		m_gameStates.insert(std::pair<string, GameState*>(stateName, newState));
	}
}

void GameStateManager::Update(float delta)
{
	// update the current state
	m_currentState->Update(delta);
}

void GameStateManager::Execute()
{
	// execute the currentState then check for stateChange happening
	m_currentState->Execute();
}