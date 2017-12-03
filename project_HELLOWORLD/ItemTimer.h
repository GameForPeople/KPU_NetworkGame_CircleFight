#pragma once
class ItemTimer
{
private:
	double	Timer;
	int		itemType;
	int		userIdx;
public:
	ItemTimer(double time, int itemType, int idx);
	~ItemTimer();

	int update(const double time);
	int getUserIdx() 
	{
		return userIdx;
	}
};

