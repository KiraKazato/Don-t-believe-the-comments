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
	
	// 入力内容表示場所の指定
	void SetKeyInputDrawPosition(int _x, int _y) { mnKeyInputDrawX = _x; mnKeyInputDrawY = _y; }
	// ポーズしたかどうか
	void SetIsPause(bool _flag) 
	{
		mbIsPause = _flag;
		if (!_flag) { ReStartKeyInput(mnInputHandle); }	// 再度入力モードにする
	}

	// @return
	// 0:入力完了していない
	// 1:入力完了
	// 2:入力キャンセル(ESCキー)
	int CheckInput() const { return mnKeyInputState; };

	auto GetInputString() const { return mInputString; };

private:
	int mnInputHandle = 0;		// 入力ハンドル
	int mnKeyInputState = 0;	// 入力状態

	int mnKeyInputDrawX = 0;	// 入力表示座標X
	int mnKeyInputDrawY = 0;	// 入力表示座標Y

	int mbIsPause = false;		// ポーズしているかどうか
private:
	char mInputString[256]{};		// 入力文字の取得バッファ（決定）
	char mPauseInputString[256]{};	// 入力文字の取得バッファ（ポーズによる中断）
};