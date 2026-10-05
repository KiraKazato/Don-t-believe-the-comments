#pragma once
#include "DxLib.h"

class TextInputManager
{
public:
	static const unsigned int KEYINPUUT_BACK_COLOR = 0xFFFFFFFF;	// 背景色	
	static const unsigned int KEYINPUUT_STRING_COLOR = 0x00000000;	// 文字色

public:
	TextInputManager();
	~TextInputManager();

	void Initialize(int _fontHandle = -1);
	void Draw();
	void Update();
	void Finalize();

	// 入力内容表示場所の指定
	void SetKeyInputDrawPosition(int _x, int _y);
	// ポーズしたかどうか
	void SetIsPause(bool _flag);

	// @return
	// 0:入力完了していない
	// 1:入力完了
	// 2:入力キャンセル(ESCキー)
	int CheckInput() const;

	const char* GetInputString() const;

	// 入力状態リセット
	void ResetState();

	// 入力内容リセット
	void ResetInput();

private:
	int mnInputHandle = -1;		// 入力ハンドル
	int mnKeyInputState = 0;	// 入力状態

	int mnKeyInputDrawX = 0;	// 入力表示座標X
	int mnKeyInputDrawY = 0;	// 入力表示座標Y

	int mbIsPause = false;		// ポーズしているかどうか

	int mnFontHandle = -1;
private:
	char mInputString[(15 * 2) + 1]{};		// 入力文字の取得バッファ（決定）
	char mPauseInputString[sizeof(mInputString)]{};	// 入力文字の取得バッファ（ポーズによる中断）
};