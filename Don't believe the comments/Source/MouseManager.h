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
	void GetMousePosition(int* X, int* Y);

	//左クリックしたか
	bool PressLeftClick();
	//左クリックを離したか
	bool ReleaseLeftClick();

private:

	MousePoint mMousePoint;

	int leftClick;
	int oldLeftClick;
};