#include "BirdEvents.h"


BirdEvents& getBirdEvents()
{
	static BirdEvents* events = new BirdEvents;
	return *events;
}
