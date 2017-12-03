#pragma once

#include <random>

using namespace std;

class RandClass
{
private:
	random_device rn;
	default_random_engine rnd;
public:
	RandClass();
	~RandClass();

	int		getRandomNumber(int min, int max);
	float	getRandomfloat(float min, float max);
};

