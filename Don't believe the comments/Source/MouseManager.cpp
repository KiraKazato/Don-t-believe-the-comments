#include "MouseManager.h"

MouseManager::MouseManager()
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
	DrawFormatString(0, 0, GetColor(0,0,0), "%d : %d", mMousePoint.mouseX, mMousePoint.mouseY);
}

int MouseManager::GetMousePosition(int* X, int* Y)
{
	if (!X || !Y) return -1;
	*X = mMousePoint.mouseX;
	*Y = mMousePoint.mouseY;
}

bool MouseManager::IsLeftClick()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		return true;
	}
	return false;
}
