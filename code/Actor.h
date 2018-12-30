#pragma once
/*=====================================================================================
$File: Actor.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef ACTOR_H
#define ACTOR_H
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "Sprite.h"
#include "World.h"

// enumeration for what direction the actor is moving
enum MoveDirection
{
	NotMoving,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown
};

// enumeration for the types of actors
enum ActorType
{
	Player,
	NPC,
	Enemy,
	Boss
};

// enumeration for the different types of abilities
enum AbilityType
{
	AbilityNone = 0,
	AbilityAttack = 1,
	AbilityDefense = 2
};

struct ActorAbility
{
	protected:
		string		m_abilityID;		// string that defines what the ability's name is
		AbilityType	m_abilityType;		// what type of ability is this attack or defense
		int			m_effectAmount;		// what is the amount of effect either negating damage or dealing it
		string		m_animationName;	// what is the name of the animation in the sprite for this ability

	public:
		// Constructor
		ActorAbility()
		{
			m_abilityID = "none";
			m_abilityType = AbilityType::AbilityNone;
			m_effectAmount = 0;
			m_animationName = "none";
		}
		// get the ability ID
		string GetAbilityID() { return m_abilityID; }
		// set the ability ID
		void SetAbilityID(string newID)
		{
			if (!newID.empty())
			{
				m_abilityID = newID;
			}
		}
		// get the ability type
		AbilityType GetAbilityType() { return m_abilityType; }
		// set the ability type
		void SetAbilityType(AbilityType type) { m_abilityType = type; }
		// get the amount that the ability will effect a stat either negating or dealing damage
		int GetAbilityEffectAmount() { return m_effectAmount; }
		// set the effect amount
		void SetAbilityEffectAmount(int amount) { m_effectAmount = amount; }
		// get the name of the animation in the sprite
		string GetAbilityAnimationName() { return m_animationName; }
		// set the name of the animation in the sprite
		void SetAbilityAnimationName(string name)
		{
			if (!name.empty())
			{
				m_animationName = name;
			}
		}
};

// forward declaration of component classes
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;

// class that defines a single actor in the game
// whether player, npc, enemy or boss
class Actor
{
	protected:
		// properties set when constructing the Actor
		string					m_actorID;				// Actor ID used for finding in collection easier
		string					m_actorName;			// Name of the Actor
		int						m_attackPower;			// Actor attack power will be 0 if NPC
		int						m_defensePower;			// Actor defense power will be 0 if NPC
		int						m_hitPoints;			// Actor hit points will be 0 if NPC
		float					m_actorSpeed;			// Actor movement speed
		ActorType				m_actorType;			// what type of Actor Player, NPC, Enemy or Boss
		vector<ActorAbility>	m_actorAbilities;		// collection of Abilities will be empty for NPC's

														// properties calculated when constructing the Actor
		Vector2					m_actorVelocity;		// Actor Velocity define which direction moving
		Vector2					m_actorPosition;		// Actor Position define where in the world Actor is currently
		GameMap*				m_currentWorld;			// World that the actor is currently set on
		MoveDirection			m_currentDirection;		// What is the name of the Direction the Actor is moving used in Input

														// Actor Components
		Sprite*					m_spriteComponent;		// Actor sprite that holds all animation information
		InputComponent*			m_inputComponent;		// Input Component for this Actor
		PhysicsComponent*		m_physicsComponent;		// Physics Component for this Actor
		GraphicsComponent*		m_graphicsComponent;	// Graphics Component for this Actor

	public:
		// Constructor
		Actor();
		// Destructor
		~Actor();

		// get the Actor ID
		string GetActorID() { return m_actorID; }
		// set the Actor ID to a new value
		void SetActorID(string newID)
		{
			if (!newID.empty())
			{
				m_actorID = newID;
			}
		}
		// get the Actor name
		string GetActorName() { return m_actorName; }
		// set the Actor name to a new value
		void SetActorName(string name)
		{
			if (!name.empty())
			{
				m_actorName = name;
			}
		}
		// get the Actor attack power
		int GetActorAttackPower() { return m_attackPower; }
		// set the Actor attack power
		void SetActorAttackPower(int attack) { m_attackPower = attack; }
		// get the Actor defense power
		int GetActorDefensePower() { return m_defensePower; }
		// set the Actor defense power
		void SetActorDefensePower(int defense) { m_defensePower = defense; }
		// get the Actor hit points
		int GetActorHitPoints() { return m_hitPoints; }
		// set the Actor hit points
		void SetActorHitPoints(int hp) { m_hitPoints = hp; }
		// get the Actor movement speed
		float GetActorMovementSpeed() { return m_actorSpeed; }
		// set the Actor movement speed to a new value
		void SetActorMovementSpeed(float speed) { m_actorSpeed = speed; }
		// get the type of Actor this instance is
		ActorType GetActorType() { return m_actorType; }
		// set the type of Actor this instance is
		void SetActorType(ActorType type) { m_actorType = type; }

		// add Ability to Actor
		void AddAbilityToActor(ActorAbility ability)
		{
			m_actorAbilities.push_back(ability);
		}
		// get the world that the Actor is attached to currently
		GameMap* GetActorCurrentWorld() { return m_currentWorld; }
		// set the world that the Actor is attached to currently
		void SetActorCurrentWorld(GameMap *world) { m_currentWorld = world; }
		// get the Sprite Component for the Actor
		Sprite* GetActorSprite() { return m_spriteComponent; }
		// set the Sprite Component for the Actor
		void SetActorSprite(Sprite* sprite) { m_spriteComponent = sprite; }
		// get the direction the Actor is currently moving (animation purposes only)
		MoveDirection GetActorDirection() { return m_currentDirection; }
		// set the direction the Actor is currently moving (animation purposes only)
		void SetActorDirection(MoveDirection direction) { m_currentDirection = direction; }
		// get the positon the Actor is currently at
		Vector2 GetActorPosition() { return m_actorPosition; }
		// set the position the Actor is currently at
		void SetActorPosition(Vector2 position) { m_actorPosition = position; }
		// get the Actor velocity
		Vector2 GetActorVelocity() { return m_actorVelocity; }
		// set the Actor velocity to a new value
		void SetActorVelocity(Vector2 velocity) { m_actorVelocity = velocity; }

		// Build Actor with raw data
		void BuildActor(int hp, int attack, int defense, float speed);
		// Build Actor from text File
		void BuildActor(string fileName, GameMap* world, Sprite* sprite);
		// update the Actor and its various components
		void Update(float deltaTime);
		// draw actor at the present location
		void DrawActor();
		// input method for receiving commands to move from keyboard
		// only used on player Actor
		void MoveActor(bool pressed, GameActions action);

		// get the Graphics Component currently set for the Actor
		GraphicsComponent* GetGraphicsComponent() { return m_graphicsComponent; }
		// set the Graphics Component currently set for the Actor
		void SetGraphicsComponent(GraphicsComponent *graphics) { m_graphicsComponent = graphics; }
		// get the Physics Component currenlty set for the Actor
		PhysicsComponent* GetPhysicsComponent() { return m_physicsComponent; }
		// set the Physics Component currently set for the Actor
		void SetPhysicsComponent(PhysicsComponent *physics) { m_physicsComponent = physics; }
		// get the Input Component currenlty set for the Actor
		InputComponent* GetInputComponent() { return m_inputComponent; }
		// set the Input Component currently set for the Actor
		void SetInputComponent(InputComponent *input) { m_inputComponent = input; }
};

// basic Abstract Virtual Graphics Component
class GraphicsComponent
{
	public:
		virtual void update(Actor &actor) = 0;
};

// Player version of the Graphics Component
class PlayerGraphicsComponent : public GraphicsComponent
{
	public:
		// draw the Player Actor at the specific position
		virtual void update(Actor &actor)
		{
			RenderManager::GetInstance().RenderObject(actor.GetActorSprite()->GetSpriteTexture(), actor.GetActorSprite()->GetSpriteRectangle(), actor.GetActorPosition());
		}
};

// basic Abstract Virtual Physics Component
class PhysicsComponent
{
	public:
		virtual void update(Actor &actor, float timeDelta) = 0;
};

// Player version of Physics Component
class PlayerPhysicsCompoonent : public PhysicsComponent
{
	public:
		// calculate the new position for the actor based on direction, velocity and its current position
		// then check with the current world and verify if that is an ok position
		virtual void update(Actor &actor, float timeDelta)
		{
			Vector2 velocity = actor.GetActorVelocity();
			Vector2 oldPosition = actor.GetActorPosition();
			Vector2 newPosition = oldPosition;
			float actorSpeed = actor.GetActorMovementSpeed();
			newPosition.x += (velocity.x * actorSpeed) * timeDelta;
			newPosition.y += (velocity.y * actorSpeed) * timeDelta;

			//if (actor.GetActorCurrentWorld()->CheckPointForCollission(newPosition))
			//{
			//actor.SetActorPosition(newPosition);
			//actor.GetActorCurrentWorld()->ChangeVelocity((Vector2(-(velocity.x), -(velocity.y))));
			//}
			//else
			//{
			//actor.SetActorPosition(oldPosition);
			//}
		}
};

// basic Abstract Virtual Input Component
class InputComponent
{
	public:
		virtual void update(Actor &actor) = 0;
};

// Player version of the Input Component
class PlayerInputComponent : public InputComponent
{
	public:
		// convert the current Actor direction into a velocity value
		virtual void update(Actor &actor)
		{
			if (actor.GetActorDirection() == MoveLeft)
				actor.SetActorVelocity(Vector2(-1, actor.GetActorVelocity().y));

			if (actor.GetActorDirection() == MoveRight)
				actor.SetActorVelocity(Vector2(1, actor.GetActorVelocity().y));

			if (actor.GetActorDirection() == MoveUp)
				actor.SetActorVelocity(Vector2(actor.GetActorVelocity().x, -1));

			if (actor.GetActorDirection() == MoveDown)
				actor.SetActorVelocity(Vector2(actor.GetActorVelocity().x, 1));

			if (actor.GetActorDirection() == NotMoving)
				actor.SetActorVelocity(Vector2(0.0, 0.0));
		}
};
#endif // !ACTOR_H