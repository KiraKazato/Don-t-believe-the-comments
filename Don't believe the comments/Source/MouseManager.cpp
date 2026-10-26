#include "MouseManager.h"

MouseManager::MouseManager() :mMousePoint({ 0,0 }), mnClickFrame(0),mnOldClickFrame(0)
{

}

MouseManager::~MouseManager()
{

}

void MouseManager::Update()
{
	GetMousePoint(&mMousePoint.mouseX, &mMousePoint.mouseY);
	
	ClickFrameCount();
}

void MouseManager::Draw()
{
	//DrawFormatString(0, 0, GetColor(0,0,0), "%d : %d", mMousePoint.mouseX, mMousePoint.mouseY);
	//DrawFormatString(0, 0, GetColor(0,0,0), "前フレーム%d : 現フレーム%d", mnOldClickFrame, mnClickFrame);
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

bool MouseManager::IsClickCancel()
{
	// 前フレームでおしっぱでかつ現フレームは離したか
	return (mnOldClickFrame > 0) && (mnClickFrame < 0);
}

void MouseManager::ClickFrameCount()
{
	// 前フレームのカウント
	mnOldClickFrame = mnClickFrame;

	if (IsLeftClick())
	{
		if (mnClickFrame <= 0)
		{
			mnClickFrame = 0;
		}

		if (mnClickFrame <= (INT_MAX - 1))
		{
			mnClickFrame++;
		}
	}
	else if (mnClickFrame != 0)
	{
		if (mnClickFrame >= 0)
		{
			mnClickFrame = 0;
		}

		if (mnClickFrame >= (INT_MIN + 1))
		{
			mnClickFrame--;
		}
	}

}

void MouseManager::GetClickFrame(int* _nowClickFrame, int* _oldClickFrame)
{
	if(_nowClickFrame)
	{
		*_nowClickFrame = mnClickFrame;
	}

	if(_oldClickFrame)
	{
		*_oldClickFrame = mnOldClickFrame;
	}
}
