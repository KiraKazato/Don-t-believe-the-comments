#include "MouseManager.h"

MouseManager::MouseManager() :mMousePoint({ 0,0 })
{

}

MouseManager::~MouseManager()
{

}

void MouseManager::Update()
{
	GetMousePoint(&mMousePoint.mouseX, &mMousePoint.mouseY);
}

void MouseManager::Draw()
{
	//DrawFormatString(0, 0, GetColor(0,0,0), "%d : %d", mMousePoint.mouseX, mMousePoint.mouseY);
}

void MouseManager::GetMousePosition(int* _x, int* _y)
{
	if (!_x || !_y)return;
	*_x = mMousePoint.mouseX;
	*_y = mMousePoint.mouseY;
}

bool MouseManager::IsLeftClick()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		return true;
	}
	return false;
}
