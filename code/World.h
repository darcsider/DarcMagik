#pragma once
/*=====================================================================================
$File: World.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef	WORLD_H
#define WORLD_H
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"

enum LayerType
{
	defaultType = 0,
	graphicLayer = 1,
	collissionMap = 2,
	enemyMap = 3
};

class Layer
{
	private:
		vector<int> m_tileMap;					// vector of int's that define what tiles go where
		string		m_textureName;				// string that holds the texture name that holds tiles
		LayerType	m_layerType;				// LayerType that defines what type of layer this is
		int			m_layerColumns;				// int that defines how many columns total for this layer
		int			m_layerRows;				// int that defines how many rows total for this layer
		Vector2		m_tileDimensions;			// Vector2 that defines the dimensions for a single tile
		Vector2		m_currentDrawPosition;		// Vector2 that defines the current world draw position
		Vector2		m_scrollSpeed;				// Vector2 that defines what speed the layer should scroll in both axes
		Vector2		m_velocity;					// Vector2 that defines the current velocity of the layer in both axes
		Vector2		m_mapDimensions;			// Vector2 that defines the full world map dimensions
		RECT		m_sourceRectangle;			// RECT that defines what tile should be grabbed from the texture
		RECT		m_destinationRectangle;		// RECT that defines where on the screen the current tile should be placed

	public:
		// constructor for a Layer clear out data
		Layer();
		// destructor for a Layer clear out data
		~Layer();
		// Build the layer with data from text file
		void BuildLayer(string textFile);
		// get the texture name for the Layer
		string GetTextureName() { return m_textureName; }
		// set the texture name for the Layer
		void SetTextureName(string newName)
		{
			if (!newName.empty())
			{
				m_textureName = newName;
			}
		}
		// get the layer type for the Layer
		LayerType GetLayerType() { return m_layerType; }
		// set the layer type for the Layer
		void SetLayerType(LayerType newType) { m_layerType = newType; }
		// get the number of columns for the Layer
		int GetLayerColumns() { return m_layerColumns; }
		// set the number of columns for the Layer
		void SetLayerColumns(int columns)
		{
			if (columns > 0)
			{
				m_layerColumns = columns;
			}
		}
		// get the number of rows for the Layer
		int GetLayerRows() { return m_layerRows; }
		// set the number of rows for the Layer
		void SetLayerRows(int rows)
		{
			if (rows > 0)
			{
				m_layerRows = rows;
			}
		}
		// get the current draw position for the Layer
		Vector2 GetCurrentDrawPosition() { return m_currentDrawPosition; }
		// set the current draw position for the Layer
		void SetCurrentDrawPosition(Vector2 newPosition) { m_currentDrawPosition = newPosition; }
		// get the scroll speed for the Layer
		Vector2 GetLayerScrollSpeed() { return m_scrollSpeed; }
		// set the scroll speed for the Layer
		void SetLayerScrollSpeed(Vector2 scroll) { m_scrollSpeed = scroll; }
		// get the velocity for the Layer
		Vector2 GetLayerVelocity() { return m_velocity; }
		// set the X velocity for the Layer
		void SetLayerXVelocity(float xVelocity) { m_velocity.x = xVelocity; }
		// set the Y velocity for the Layer
		void SetLayerYVelocity(float yVelocity) { m_velocity.y = yVelocity; }
		// get the mapDimensions for the Layer
		Vector2 GetLayerWorldDimensions() { return m_mapDimensions; }
		// set the mapDimensions for the Layer
		void SetLayerWorldDimensions(Vector2 dimensions) { m_mapDimensions = dimensions; }
		// get the source rectangle for the Layer
		RECT GetSourceRectangle() { return m_sourceRectangle; }
		// set the source rectangle for the Layer
		void SetSourceRectangle(RECT source) { m_sourceRectangle = source; }
		// get the destination rectangle for the Layer
		RECT GetDestinationRectangle() { return m_destinationRectangle; }
		// set the destination rectangle for the Layer
		void SetDestinationRectangle(RECT destination) { m_destinationRectangle = destination; }
		// update this Layer based on change in time
		void UpdateLayer(float deltaTime);
		// render this Layer in its current state
		void RenderLayer();
};

class GameTrigger
{
	private:
		string		m_triggerID;					// string used to identify the trigger
		string		m_startingLocationID;			// ID of the map that this trigger is located on
		Vector2		m_startingLocationPosition;		// (x,y) position of the trigger in world Coordinates
		string		m_destinationLocationID;		// ID of the map that this trigger is to send the player to
		Vector2		m_destinationLocationPosition;	// (x,y) destination position of the trigger in world Coordinates

	public:
		// Constructor clear out data
		GameTrigger();
		// Destructor clear out data
		~GameTrigger();
		// Build the Game Trigger from textfile data
		void BuildGameTrigger(string textFile);
		// get the ID for the trigger
		string GetTriggerID() { return m_triggerID; }
		// set the ID for the trigger
		void SetTriggerID(string newID)
		{
			if (!newID.empty())
			{
				m_triggerID = newID;
			}
		}
		// get the starting location ID
		string GetStartingLocationID() { return m_startingLocationID; }
		// set the starting location ID
		void SetStartingLocationID(string newID)
		{
			if (!newID.empty())
			{
				m_startingLocationID = newID;
			}
		}
		// get the starting location world position
		Vector2 GetWorldStartingPosition() { return m_startingLocationPosition; }
		// set the starting location world position
		void SetWorldStartingPosition(Vector2 newPosition) { m_startingLocationPosition = newPosition; }
		// get the destination location ID
		string GetDestinationLocationID() { return m_destinationLocationID; }
		// set the destination location ID
		void SetDestinationLocationID(string newID)
		{
			if (!newID.empty())
			{
				m_destinationLocationID = newID;
			}
		}
		// get the destination location world position
		Vector2 GetWorldDestinationPosition() { return m_destinationLocationPosition; }
		// set the destination location world position
		void SetWorldDestinationLocation(Vector2 newPosition) { m_destinationLocationPosition = newPosition; }
};

class GameMap
{
	private:
		string				m_gameMapID;		// string ID used to identify what map this is
		vector<Layer>		m_gameMapLayers;	// vector of Layers some displayed some used for calculations
		vector<GameTrigger> m_gameMapTriggers;	// vector fo GameTriggers used to define map events
		int					m_screenColumns;	// int that defines how many columns are displayed on screen
		int					m_screenRows;		// int that defines how many rows are displayed on screen

	public:
		// Constructor clear out data
		GameMap();
		// Destructor clear out data
		~GameMap();
		// Get Map ID for this instance of a Game Map
		string GetMapID() { return m_gameMapID; }
		// Set Map ID for this instance of a Game Map
		void SetMapID(string newID)
		{
			if (!newID.empty())
			{
				m_gameMapID = newID;
			}
		}
		// Update this instance of a Game Map's Velocity
		void UpdateMapVelocity(string whichOne, float velocity);
		// build this instance of a Game Map
		void BuildMap(string textFile);
		// update the map and all of its layers
		void UpdateMap(float deltaTime);
		// render the map in its current state
		void RenderMap();
};

class GameWorld
{
	private:
		map<string, GameMap*> m_gameMaps;
		GameMap* m_currentGameMap;

	public:
		// Constructor currently does nothing
		GameWorld();
		// Destructor currently does nothing
		~GameWorld();
		// Build all maps for the game world
		void BuildMaps(string textFile);
		// Get the current map for the game world
		GameMap* GetCurrentMap();
		// Set the current map for the game world
		void SetCurrentMap(string mapID);
		// Update the current map
		void UpdateCurrentMap(float deltaTime);
		// Render the current map
		void RenderCurrentMap();
		// here only for testing purposes will NOT BE STAYING
		void CloseGame(bool pressed, GameActions action)
		{
			if (pressed)
				PostQuitMessage(0);
		}
};
#endif