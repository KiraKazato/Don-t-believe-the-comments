#pragma once
#include "DxLib.h"
#include <string>
#include "Typing.h"

class TextInputManager
{
public:
	static const unsigned int KEYINPUT_BACK_COLOR = 0xFFFFFFFF;	// 背景色	
	static const unsigned int KEYINPUT_STRING_COLOR = 0x00000000;	// 文字色

	static const int KETINPUT_STRING_MAX_LENGTH_FULL_CHARACTOR = 15;	// 全角文字最大

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
	// 2:入力キャンセル
	int CheckInput() const;

	const std::string& GetInputString() const;

	// 入力状態リセット
	void ResetState();

	// 入力内容リセット
	void ResetInput();

	// 削除やカーソル移動等
	void ControllInput(char _input);

private:
	int mnFontHandle = -1;		// フォントハンドル
	
	int mnKeyInputState = 0;	// 入力状態

	int mnKeyInputDrawX = 0;	// 入力表示座標X
	int mnKeyInputDrawY = 0;	// 入力表示座標Y

	bool mbIsPause = false;		// ポーズしているかどうか

private:
	std::string mInputString{};		// ローマ字入力
	std::string mOutputString{};	// かな文字入力

	int mnKanaCursorPosition = 0;	// （かな文字）カーソル位置
	int mnRomaCursorPosition = 0;	// （ローマ字）カーソル位置
	int mnDrawCursorPosition = 0;	// （描画文字）カーソル位置

	Typing mTyping{};
};