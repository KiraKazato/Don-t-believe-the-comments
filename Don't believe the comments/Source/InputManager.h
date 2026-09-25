#pragma once
#include "DxLib.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Initialize();
	void Draw();
	void Update();
	void Finalize();

	void SetKeyInputDrawPosition(int _x, int _y) { mnKeyInputDrawX = _x; mnKeyInputDrawY = _y; }
	void SetIsPause(bool _flag) { mbIsPause = _flag; if (!_flag) { ReStartKeyInput(mnInputHandle); }; }

	// @return
	// 0:入力完了していない
	// 1:入力完了
	// 2:入力キャンセル(ESCキー)
	int CheckInput() const { return mnKeyInputState; };

	auto GetInputString() const { return mInputString; };

private:
	int mnInputHandle = 0;
	int mnKeyInputState = 0;

	int mnKeyInputDrawX = 0;
	int mnKeyInputDrawY = 0;

	int mbIsPause = false;
private:
	char mInputString[256]{};
	char mPauseInputString[256]{};
};