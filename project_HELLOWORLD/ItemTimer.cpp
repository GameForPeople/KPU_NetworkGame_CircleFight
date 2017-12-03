#include <Windows.h>
#include "ItemTimer.h"



ItemTimer::ItemTimer(double time, int type, int idx)
{
	Timer = time;
	itemType = type;
	userIdx = idx;
}


ItemTimer::~ItemTimer()
{	
}

int ItemTimer::update(const double time)
{
	Timer -= time;

	if (Timer <= 0)
	{
		return itemType;
	}
	return -1;
}