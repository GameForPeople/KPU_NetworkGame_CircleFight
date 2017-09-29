#pragma once
class Scene
{
public:
	Scene();
	~Scene();

	void Draw();
	void Timer();
	bool KeyProcess();
	bool MouseProcess();
	void Destory();
};

