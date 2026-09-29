#include "TextInputManager.h"
#include "DxLib.h"

TextInputManager::TextInputManager()
{
}

TextInputManager::~TextInputManager()
{
	Finalize();
}

void TextInputManager::Initialize()
{
	mnInputHandle = MakeKeyInput(256, TRUE, FALSE, FALSE);
	SetActiveKeyInput(mnInputHandle);
}

void TextInputManager::Draw()
{
	if (mnKeyInputState == 0)
	{
		char tmpStr[256] = "\0";
		auto IMEData = GetIMEInputData();

		GetKeyInputString(tmpStr, mnInputHandle);
		int width = GetDrawFormatStringWidth("%s", tmpStr);
		width += GetDrawFormatStringWidth("%s", IMEData->InputString);
		mnKeyInputDrawX = (mnKeyInputDrawX + width) / 2;
		
		DrawKeyInputString(mnKeyInputDrawX, mnKeyInputDrawY, mnInputHandle);
	}
}

void TextInputManager::Update()
{
	// 入力出来たか確認
	mnKeyInputState = CheckKeyInput(mnInputHandle);
	ProcessActKeyInput();

	if (mnKeyInputState == 1)
	{
		GetKeyInputString(mInputString, mnInputHandle);
		Finalize();
	}
	
	if (!mbIsPause)
		return;

	// ポーズした時の処理
	if (mnKeyInputState == 2)	// キャンセル時
	{
		mbIsPause = true;
		
		// 途中までの入力を入れる
		GetKeyInputString(mPauseInputString, mnInputHandle);

		// ポーズした文字を入れる
		SetKeyInputString(mPauseInputString, mnInputHandle);
	}
}

void TextInputManager::Finalize()
{
	DeleteKeyInput(mnInputHandle);
}