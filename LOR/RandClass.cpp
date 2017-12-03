#include "RandClass.h"



RandClass::RandClass()
{
	default_random_engine temp(rn());
	rnd = temp;
}


RandClass::~RandClass()
{
}

int RandClass::getRandomNumber(int min, int max)
{
	uniform_int_distribution<int> range(min, max);

	//< 3단계. 값 추출
	return range(rnd);
}

float RandClass::getRandomfloat(float min, float max)
{
	uniform_real_distribution<float> range(min, max);

	//< 3단계. 값 추출
	return range(rnd);
}