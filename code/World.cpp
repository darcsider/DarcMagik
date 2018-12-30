/*=====================================================================================
$File: World.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "World.h"

Layer::Layer()
{
	// clear out all data objects for Layer
	m_tileMap.clear();
	m_textureName.clear();
	m_layerType = defaultType;
	m_layerColumns = 0;
	m_layerRows = 0;
	m_currentDrawPosition = Vector2::Zero;
	m_scrollSpeed = Vector2::Zero;
	m_velocity = Vector2::Zero;
	m_sourceRectangle = { 0, 0, 0, 0 };
	m_destinationRectangle = { 0, 0, 0, 0 };
}

Layer::~Layer()
{
	// clear out all data objects for Layer
	m_tileMap.clear();
	m_textureName.clear();
	m_layerType = defaultType;
	m_layerColumns = 0;
	m_layerRows = 0;
	m_currentDrawPosition = Vector2::Zero;
	m_scrollSpeed = Vector2::Zero;
	m_velocity = Vector2::Zero;
	m_sourceRectangle = { 0, 0, 0, 0 };
	m_destinationRectangle = { 0, 0, 0, 0 };
}

void Layer::BuildLayer(string textFile)
{
	string		tempString;				// temp string used in file reading
	ifstream	inFile(textFile);		// file reader variable

										// if the file is successfully opened then start reading data
	if (inFile)
	{
		// read in a line from text file and place into textureName for Layer
		getline(inFile, tempString);
		m_textureName = tempString;

		// read in a line from text file and based on value set layerType
		getline(inFile, tempString);
		if (tempString == "graphicLayer")
		{
			m_layerType = graphicLayer;
		}

		if (tempString == "collissionMap")
		{
			m_layerType = collissionMap;
		}

		if (tempString == "enemyMap")
		{
			m_layerType = enemyMap;
		}

		// read in a line from text file, convert to Integer and place into layerColumns
		getline(inFile, tempString);
		m_layerColumns = atoi(tempString.c_str());

		// read in a line from text file, convert to Integer and place into layerRows
		getline(inFile, tempString);
		m_layerRows = atoi(tempString.c_str());

		// read in a line from text file seperate values and place into tileDimensions
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_tileDimensions.x = (float)(atoi(tempString.c_str()));
			getline(ss, s2, ',');
			m_tileDimensions.y = (float)(atoi(tempString.c_str()));
		}

		// read in a line from text file and place into scrollSpeed
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_scrollSpeed.x = (float)(atoi(tempString.c_str()));
			getline(ss, s2, ',');
			m_scrollSpeed.y = (float)(atoi(tempString.c_str()));
		}

		// loop through the rows of tile map data
		for (int row = 0; row < m_layerRows; row++)
		{
			// grab a single line of data from text file
			getline(inFile, tempString);
			istringstream ss(tempString);

			// loop through each line seperating the individual values
			while (ss)
			{
				string s2;
				if (!getline(ss, s2, ','))
					break;

				// place individual values into tilemap vector
				m_tileMap.push_back(atoi(s2.c_str()));
			}
		}
	}

	// build man dimensions
	m_mapDimensions.x = m_tileDimensions.x * m_layerColumns;
	m_mapDimensions.y = m_tileDimensions.y * m_layerRows;
	// close file when finished reading data
	inFile.close();
}

void Layer::UpdateLayer(float deltaTime)
{
	m_currentDrawPosition.x += (m_scrollSpeed.x * m_velocity.x) * deltaTime;
	m_currentDrawPosition.y += (m_scrollSpeed.y * m_velocity.y) * deltaTime;

	// check to see that the draw position isn't less than 0
	if (m_currentDrawPosition.x < 0)
	{
		m_currentDrawPosition.x = 0;
	}

	// check to see that the draw position isn't greater than the size of the map
	if ( (m_mapDimensions.x >= RenderManager::GetInstance().GetGameWidth()) && 
		m_currentDrawPosition.x > m_mapDimensions.x - RenderManager::GetInstance().GetGameWidth())
	{
		m_currentDrawPosition.x = m_mapDimensions.x - RenderManager::GetInstance().GetGameWidth();
	}

	// check to see that the draw position isn't less than 0
	if (m_currentDrawPosition.y < 0)
	{
		m_currentDrawPosition.y = 0;
	}

	// check to see that the draw position isn't greater than the size of the map
	if ( (m_mapDimensions.y >= RenderManager::GetInstance().GetGameHeight()) &&
		m_currentDrawPosition.y > m_mapDimensions.y - RenderManager::GetInstance().GetGameHeight())
	{
		m_currentDrawPosition.y = m_mapDimensions.y - RenderManager::GetInstance().GetGameHeight();
	}
}

void Layer::RenderLayer()
{
	int screenRows = 0;		// int that represents the number of rows on the screen
	int screenColumns = 0;	// int that represents the number of columns on the screen
	float tileX = 0;		// int that represents x position on screen for a tile
	float tileY = 0;		// int that represents y position on screen for a tile
	int tileValue = 0;		// value of tile in tilemap at the position
	int textureColumns = 0;	// how many columns in the texture

	// only calculate and do rendering if this is a graphical layer
	if (m_layerType == graphicLayer)
	{
		// calculate certain values for rendering
		int gameHeight = RenderManager::GetInstance().GetGameHeight();
		int gameWidth = RenderManager::GetInstance().GetGameWidth();
		screenRows = RenderManager::GetInstance().GetGameHeight() / (int)(m_tileDimensions.y);
		screenColumns = RenderManager::GetInstance().GetGameWidth() / (int)(m_tileDimensions.x);

		tileX = m_currentDrawPosition.x / m_tileDimensions.x;
		tileY = m_currentDrawPosition.y / m_tileDimensions.y;

		textureColumns = (int)(RenderManager::GetInstance().GetTextureDesc(m_textureName).Width / m_tileDimensions.x);

		// traverse through the number of rows
		for (int row = 0; row <= screenRows; row++)
		{
			// traverse through the number of columns
			for (int column = 0; column < screenColumns; column++)
			{
				// grab value from tilemap
				tileValue = m_tileMap[((int(tileY) + row) * m_layerColumns + (int(tileX) + column))];

				// calculate the source rectangle
				m_sourceRectangle.left = (long)((tileValue % textureColumns) * m_tileDimensions.x);
				m_sourceRectangle.top = (long)((tileValue / textureColumns) * m_tileDimensions.y);
				m_sourceRectangle.right = (long)(m_sourceRectangle.left + m_tileDimensions.x);
				m_sourceRectangle.bottom = (long)(m_sourceRectangle.top + m_tileDimensions.y);

				// calculate the destination rectangle
				m_destinationRectangle.left = (long)(column * m_tileDimensions.x);
				m_destinationRectangle.top = (long)(row * m_tileDimensions.y);
				m_destinationRectangle.right = (long)(m_destinationRectangle.left + m_tileDimensions.x);
				m_destinationRectangle.bottom = (long)(m_destinationRectangle.top + m_tileDimensions.y);

				// draw the tile to the screen at the correct position
				RenderManager::GetInstance().RenderObject(m_textureName, m_sourceRectangle, m_destinationRectangle);
			}
		}
	}
}

GameTrigger::GameTrigger()
{
	// clear all data for the GameTrigger
	m_triggerID.clear();
	m_startingLocationID.clear();
	m_startingLocationPosition = Vector2::Zero;
	m_destinationLocationID.clear();
	m_destinationLocationPosition = Vector2::Zero;
}

GameTrigger::~GameTrigger()
{
	// clear all data for the GameTrigger
	m_triggerID.clear();
	m_startingLocationID.clear();
	m_startingLocationPosition = Vector2::Zero;
	m_destinationLocationID.clear();
	m_destinationLocationPosition = Vector2::Zero;
}

void GameTrigger::BuildGameTrigger(string textFile)
{
	string		tempString;				// temp string used in file reading
	ifstream	inFile(textFile);		// file reader variable

										// if the file is successfully opened then start reading data
	if (inFile)
	{
		// read in a line from text file and place into triggerID
		getline(inFile, tempString);
		m_triggerID = tempString;

		// read in a line from text file and place into startingLocationID
		getline(inFile, tempString);
		m_startingLocationID = tempString;

		// read in a line from text file, seperate values and place into startingLocationPosition
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_startingLocationPosition.x = (float)(atoi(tempString.c_str()));
			getline(ss, s2, ',');
			m_startingLocationPosition.y = (float)(atoi(tempString.c_str()));
		}

		// read in a line from text file and place into destinationLocationID
		getline(inFile, tempString);
		m_destinationLocationID = tempString;

		// read in a line from text file, seperate values and place into destinationLocationPosition
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_destinationLocationPosition.x = (float)(atoi(tempString.c_str()));
			getline(ss, s2, ',');
			m_destinationLocationPosition.y = (float)(atoi(tempString.c_str()));
		}
	}
	// close file when finished reading data
	inFile.close();
}

GameMap::GameMap()
{
	// clear all data for the GameMap
	m_gameMapID.clear();
	m_gameMapLayers.clear();
	m_gameMapTriggers.clear();
	m_screenColumns = 0;
	m_screenRows = 0;
}

GameMap::~GameMap()
{
	// clear all data for the GameMap
	m_gameMapID.clear();
	m_gameMapLayers.clear();
	m_gameMapTriggers.clear();
	m_screenColumns = 0;
	m_screenRows = 0;
}

void GameMap::UpdateMapVelocity(string whichOne, float velocity)
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the collection of Layers

	// loop through all layers calling each one's update method
	for (layerIterator = m_gameMapLayers.begin(); layerIterator != m_gameMapLayers.end(); layerIterator++)
	{
		if (whichOne == "X")
		{
			layerIterator->SetLayerXVelocity(velocity);
		}
		else if (whichOne == "Y")
		{
			layerIterator->SetLayerYVelocity(velocity);
		}
		else
		{
			layerIterator->SetLayerXVelocity(velocity);
			layerIterator->SetLayerYVelocity(velocity);
		}
	}
}

void GameMap::BuildMap(string textFile)
{
	string		tempString;				// temp string used in file reading
	ifstream	inFile(textFile);		// file reader variable
	int			numberOfLayers;			// temp Integer used for looping
	int			numberOfTriggers;		// temp Integer used for looping
	Layer		tempLayer;				// temp Layer used for building maps
	GameTrigger tempTrigger;			// temp GameTrigger used for building maps

										// if the file is successfully opened then start reading data
	if (inFile)
	{
		// read in a line from text file and place into mapID
		getline(inFile, tempString);
		m_gameMapID = tempString;

		// read in a line from text file, convert to Integer and place into numberOfLayers
		getline(inFile, tempString);
		numberOfLayers = atoi(tempString.c_str());

		// loop through the number of layers
		for (int layers = 0; layers < numberOfLayers; layers++)
		{
			// read in a line from text file and pass into BuildLayer
			getline(inFile, tempString);
			tempLayer.BuildLayer(tempString);

			// put the built layer into gameMapLayers
			m_gameMapLayers.push_back(tempLayer);
		}

		// read in a line from text file, convert to Integer and place into numberOfTriggers
		getline(inFile, tempString);
		numberOfTriggers = atoi(tempString.c_str());

		// loop through the number of triggers
		for (int trigger = 0; trigger < numberOfTriggers; trigger++)
		{
			// read in a line from text file and pass into BuildGameTrigger
			getline(inFile, tempString);
			tempTrigger.BuildGameTrigger(tempString);

			// put the built layer into gameMapLayers
			m_gameMapTriggers.push_back(tempTrigger);
		}
	}
	// close file when finished reading data
	inFile.close();
}

void GameMap::UpdateMap(float deltaTime)
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the collection of Layers

											// loop through all layers calling each one's update method
	for (layerIterator = m_gameMapLayers.begin(); layerIterator != m_gameMapLayers.end(); layerIterator++)
	{
		layerIterator->UpdateLayer(deltaTime);
	}
}

void GameMap::RenderMap()
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the collection of Layers

											// loop through all layers calling each one's render method
	for (layerIterator = m_gameMapLayers.begin(); layerIterator != m_gameMapLayers.end(); layerIterator++)
	{
		layerIterator->RenderLayer();
	}
}

GameWorld::GameWorld()
{
	// initialize variables to default values
	m_currentGameMap = nullptr;
}

GameWorld::~GameWorld()
{
	// currently left blank intentionally
}

void GameWorld::BuildMaps(string textFile)
{
	int			numberOfMaps = 0;	// temp variable to hold the number of Maps
	string		tempString;			// temp variable to hold line read in from file
	ifstream	inFile(textFile);	// file reader variable

									// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read a line in and convert to Integer and place into numberOfMaps
		getline(inFile, tempString);
		numberOfMaps = atoi(tempString.c_str());
		// Loop through several lines and use data to Build the Worlds
		for (int i = 0; i < numberOfMaps; i++)
		{
			getline(inFile, tempString);
			GameMap* newMap = new GameMap();
			newMap->BuildMap(tempString);
			m_gameMaps.insert(pair<string, GameMap*>(newMap->GetMapID(), newMap));
		}
	}
	inFile.close();
}
GameMap* GameWorld::GetCurrentMap()
{
	// return the current game map it is the responsibility of
	// caller to check to make sure that it is not nullptr
	return m_currentGameMap;
}

void GameWorld::SetCurrentMap(string mapID)
{
	// check to see if the mapID passed in exists in the container
	auto mapIndex = m_gameMaps.find(mapID);

	// if it exists then change the currentWorld equal to the one that matches
	if (mapIndex != m_gameMaps.end())
	{
		m_currentGameMap = mapIndex->second;
	}
	// otherwise set the currentWorld = nullptr
	else
	{
		m_currentGameMap = nullptr;
	}
}

void GameWorld::UpdateCurrentMap(float deltaTime)
{
	// if there is a currently set game map
	if (m_currentGameMap != nullptr)
	{
		// update the current map
		m_currentGameMap->UpdateMap(deltaTime);
	}
}

void GameWorld::RenderCurrentMap()
{
	// if there is a currently set game map
	if (m_currentGameMap != nullptr)
	{
		// render the current map
		m_currentGameMap->RenderMap();
	}
}