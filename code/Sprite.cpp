/*=====================================================================================
$File: Sprite.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Sprite.h"

Sprite::Sprite()
{
	// initialize variables
	m_currentAction = AnimatedAction();
}

Sprite::~Sprite()
{
	// left blank for now
}

void Sprite::UpdateSprite(float timeDelta)
{
	// make sure there are still frames in the current animation
	if (m_currentAction.endFrame - m_currentAction.startFrame > 0)
	{
		// update the animation timer with the time change since last frame
		m_currentAction.animationTimer += timeDelta;

		// if the animation timer has exceeded the animation frame delay
		if (m_currentAction.animationTimer > m_currentAction.frameDelay)
		{
			// update the timer and increase what frame is the current frame
			m_currentAction.animationTimer -= m_currentAction.frameDelay;
			m_currentAction.currentFrame++;
			
			// if the current frame is less than the start frame or past the last frame then set it to the first frame
			if (m_currentAction.currentFrame < m_currentAction.startFrame || m_currentAction.currentFrame > m_currentAction.endFrame)
				m_currentAction.currentFrame = m_currentAction.startFrame;

			// update the source rectangle
			SetSourceRectangle();
		}
	}
}

void Sprite::AddAction(int start, int end, double frameD, string name)
{
	// create an animated action and add it to the collection of actions
	AnimatedAction tempAction{ start, end, 0, frameD };
	m_actionAnimations.insert(pair<string, AnimatedAction>(name, tempAction));
}

void Sprite::BuildSprite(int w, int h, int cols, int rows, float scale, string name)
{
	// build the sprite from information passed in
	// calculate the width and height of each individual sprite frame
	m_spriteInfo.width = w / cols;
	m_spriteInfo.height = h / rows;
	// not currently used but may be incorporated into engine later
	m_spriteInfo.scale = scale;
	m_spriteInfo.textureName = name;
	m_spriteInfo.columns = cols;
	m_spriteInfo.rows = rows;

	// set the current action to not moving
	// this may be changed in the future to add a standing animation
	// so that when the player isn't moving the character will still move
	// around a little bit
	SetCurrentAction("notMoving");
	// update the source rectangle for the sprite
	SetSourceRectangle();
}

void Sprite::SetCurrentAction(string name)
{
	// find the action that was passed in and set the current action equal to that action
	auto actionIndex = m_actionAnimations.find(name);

	if (actionIndex != m_actionAnimations.end())
		m_currentAction = actionIndex->second;

	m_currentAction.currentFrame = m_currentAction.startFrame;
}

void Sprite::SetCurrentFrame(int frame)
{
	// update the frame to whatever frame is passed in as long as frame is > 0
	if (frame > 0)
	{
		m_currentAction.currentFrame = frame;
		// update the source rectangle accordingly
		SetSourceRectangle();
	}
}

void Sprite::SetSourceRectangle()
{
	// build the source rectangle based on the sprite information
	m_spriteInfo.sourceRect.left = (m_currentAction.currentFrame % m_spriteInfo.columns) * m_spriteInfo.width;
	m_spriteInfo.sourceRect.right = m_spriteInfo.sourceRect.left + m_spriteInfo.width;
	m_spriteInfo.sourceRect.top = (m_currentAction.currentFrame / m_spriteInfo.columns) * m_spriteInfo.height;
	m_spriteInfo.sourceRect.bottom = m_spriteInfo.sourceRect.top + m_spriteInfo.height;
}