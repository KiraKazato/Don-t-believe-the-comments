#pragma once
#include "DxLib.h"

struct MousePoint
{
	int mouseX;
	int mouseY;
};


class MouseManager
{
public:
	MouseManager();
	~MouseManager();

	void Update();
	void Draw();

	//マウスの場所
	int GetMousePosition(int* X, int* Y);

	//左クリックしたか
	bool IsLeftClick();
private:

	MousePoint mMousePoint;

};