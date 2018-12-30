#pragma once
/*=====================================================================================
$File: EventManager.h
$Date: March 29, 2017
$Creator: Jamie Cooper
=====================================================================================*/
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "Includes.h"

class Event
{
	protected:
		virtual ~Event();
};

class Subject
{
	protected:

	public:

};

class Observer
{
	protected:

	public:

};

template<class T>
bool TryHandleEvent(const Event* event)
{
	if (const T* event = dynamic_cast<const T*>(event))
	{
		return HandleEvent(event);
	}
	return false;
}

void OnEvent(const Event* event)
{

}
#endif // !EVENTMANAGER_H