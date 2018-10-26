/*=====================================================================================
$File: Actor.cpp
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#include "Actor.h"

Actor::Actor()
{
	// TODO Fill This In
}

Actor::~Actor()
{
	// TODO Fill This In

}

void Actor::BuildActor(int hp, int attack, int defense, float speed)
{
	m_hitPoints = hp;
	m_attackPower = attack;
	m_defensePower = defense;
	m_actorSpeed = speed;
}

void Actor::BuildActor(string fileName, GameMap* world, Sprite* sprite)
{
	string			tempString;			// string to hold line from file
	int				numberOfAbilities;	// Integer to hold number of abilities
	ActorAbility	tempAbility;		// temp Ability for building abilities
	ifstream		inFile(fileName);	// file reader variable

	m_currentWorld = world;
	m_spriteComponent = sprite;

	if (inFile)
	{
		// Read a line in and set Actor ID
		getline(inFile, tempString);
		SetActorID(tempString);
		// Read a line in and set Actor Name
		getline(inFile, tempString);
		SetActorName(tempString);
		// Read a line in convert it to Integer and set Attack Power
		getline(inFile, tempString);
		SetActorAttackPower(atoi(tempString.c_str()));
		// Read a line in convert it to Integer and set Defense Power
		getline(inFile, tempString);
		SetActorDefensePower(atoi(tempString.c_str()));
		// Read a line in convert it to Integer and set Hit Points
		getline(inFile, tempString);
		SetActorHitPoints(atoi(tempString.c_str()));
		// Read a line in convert to float and set movement speed
		getline(inFile, tempString);
		SetActorMovementSpeed((float)(atof(tempString.c_str())));
		// Read a line and set the Actor type based on value
		getline(inFile, tempString);
		if (tempString == "Player")
		{
			SetActorType(Player);
		}
		if (tempString == "NPC")
		{
			SetActorType(NPC);
		}
		if (tempString == "Enemy")
		{
			SetActorType(Enemy);
		}
		if (tempString == "Boss")
		{
			SetActorType(Boss);
		}
		// Read a line convert to Integer and set number of Abilities
		getline(inFile, tempString);
		numberOfAbilities = atoi(tempString.c_str());
		// loop through the number of Abilities and build each Ability
		for (int ability = 0; ability < numberOfAbilities; ability++)
		{
			// Read a line and set the Ability ID
			getline(inFile, tempString);
			tempAbility.SetAbilityID(tempString);
			// Read a line and set the Ability Type based on value
			getline(inFile, tempString);
			if (tempString == "AbilityAttack")
			{
				tempAbility.SetAbilityType(AbilityAttack);
			}
			if (tempString == "AbilityDefense")
			{
				tempAbility.SetAbilityType(AbilityDefense);
			}
			// Read a line convert to Integer and set the Ability Effect Amount
			getline(inFile, tempString);
			tempAbility.SetAbilityEffectAmount(atoi(tempString.c_str()));
			// Read a line and set Ability Animation Name
			getline(inFile, tempString);
			tempAbility.SetAbilityAnimationName(tempString);
			// Add the built Ability to the Actor
			AddAbilityToActor(tempAbility);
		}
	}

	// set starting velocity of 0
	SetActorVelocity(Vector2(0.0, 0.0));
	// set starting direction of not moving
	SetActorDirection(NotMoving);
	// set starting position to what ever the current world's Player start point
	//SetActorPosition(GetActorCurrentWorld()->GetPlayerStartSpawnPoint());

	// finished close the file
	inFile.close();
}

void Actor::Update(float deltaTime)
{
	// update the Input Component for the Actor
	m_inputComponent->update(*this);
	// update the Physics Component for the Actor
	m_physicsComponent->update(*this, deltaTime);
	// update the Sprite Component for the Actor
	m_spriteComponent->UpdateSprite(deltaTime);
}

void Actor::DrawActor()
{
	// update the Graphics Component for the Actor
	m_graphicsComponent->update(*this);
}

void Actor::MoveActor(bool pressed, GameActions action)
{
	// if the Player Actor has received Input of Some kind
	if (pressed)
	{
		// depending on the Action set the Actor Direction
		switch (action)
		{
			case ActionUp:
				break;
			case ActionDown:
				break;
			case ActionLeft:
				break;
			case ActionRight:
				break;
			case ActionAccept:
				break;
			case ActionCancel:
				break;
			case CharacterAttack:
				break;
			case CharacterDefense:
				break;
			case DirectionMoveLeft:
				SetActorDirection(MoveLeft);
				break;
			case DirectionMoveRight:
				SetActorDirection(MoveRight);
				break;
			case DirectionMoveUp:
				SetActorDirection(MoveUp);
				break;
			case DirectionMoveDown:
				SetActorDirection(MoveDown);
				break;
			case SystemTest:
				break;
			case SystemExitEarly:
				break;
			case SystemConsole:
				break;
			default:
				SetActorDirection(NotMoving);
				break;
		}
	}
	else
	{
		SetActorDirection(NotMoving);
	}

	// update the Input Component again after changing Actor Direction
	m_inputComponent->update(*this);
}

/*void Actor::SetActorDirection(MoveDirection direction)
{
	// based on the passed in MoveDirection set the Sprite Component Animation Action
	switch (direction)
	{
		case NotMoving:
			m_spriteComponent->SetCurrentAction("notMoving");
			break;
		case MoveLeft:
			m_spriteComponent->SetCurrentAction("moveLeft");
			break;
		case MoveRight:
			m_spriteComponent->SetCurrentAction("moveRight");
			break;
		case MoveUp:
			m_spriteComponent->SetCurrentAction("moveUp");
			break;
		case MoveDown:
			m_spriteComponent->SetCurrentAction("moveDown");
			break;
		default:
			m_spriteComponent->SetCurrentAction("notMoving");
			break;
	}
	m_currentDirection = direction;
}*/