#include "InputManager.h"
#include "DxLib.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	mnInputHandle = MakeKeyInput(256, TRUE, FALSE, FALSE);
	SetActiveKeyInput(mnInputHandle);
}

void InputManager::Draw()
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

void InputManager::Update()
{
	mnKeyInputState = CheckKeyInput(mnInputHandle);

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

void InputManager::Finalize()
{
	DeleteKeyInput(mnInputHandle);
}