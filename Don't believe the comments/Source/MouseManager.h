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
	
	//クリックフレーム（nullptrなら渡さない）
	//～-1 離しているフレーム数
	//+1～ 押しているフレーム数
	//±0 起動時から一度も押していない
	void GetClickFrame(int* _nowClickFrame, int* _oldClickFrame);

	//左クリックしたか
	bool IsLeftClick();

	//クリックしたけどキャンセルするか
	bool IsClickCancel();

	//クリックしているフレーム数を計測
	void ClickFrameCount();

	

private:

	MousePoint mMousePoint{};

	int mnClickFrame = 0;
	int mnOldClickFrame = 0;
};