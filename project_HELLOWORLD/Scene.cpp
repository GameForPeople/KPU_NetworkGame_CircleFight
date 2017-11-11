#include "Scene.h"



Scene::Scene()
{

}

Scene::Scene(HWND hwnd)
{
	m_hWnd = hwnd;
	m_isDestory = false;
}


Scene::~Scene()
{

}

bool Scene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	return 0;
}

bool Scene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	return 0;
}

void Scene::Destory() {
}


