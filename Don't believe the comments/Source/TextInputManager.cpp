#include "TextInputManager.h"
#include "DxLib.h"

TextInputManager::TextInputManager()
{
}

TextInputManager::~TextInputManager()
{
	Finalize();
}

void TextInputManager::Initialize(int _fontHandle)
{
	SetKeyInputStringFont(_fontHandle);

	mnInputHandle = MakeKeyInput(256, TRUE, FALSE, FALSE);

	SetActiveKeyInput(mnInputHandle);
}

void TextInputManager::Draw()
{
	if (mnKeyInputState == 0)
	{
		char tmpStr[256] = "\0";
		auto IMEData = GetIMEInputData();

		try
		{
			GetKeyInputString(tmpStr, mnInputHandle);
		}
		catch (...)
		{
			memset(tmpStr, '\0', sizeof(tmpStr));
		}
		
		int width = GetDrawFormatStringWidth("%s", tmpStr);
		if (IMEData)
			width += GetDrawFormatStringWidth("%s", IMEData->InputString);
		
		DrawKeyInputString(mnKeyInputDrawX - (width / 2), mnKeyInputDrawY, mnInputHandle, FALSE);
	}
}

void TextInputManager::Update()
{
	// 入力出来たか確認
	mnKeyInputState = CheckKeyInput(mnInputHandle);

	if (mnKeyInputState == 1)
	{
		GetKeyInputString(mInputString, mnInputHandle);
		return;
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
	mnInputHandle = -1;
}

void TextInputManager::SetKeyInputDrawPosition(int _x, int _y)
{
	mnKeyInputDrawX = _x;
	mnKeyInputDrawY = _y;
}

void TextInputManager::SetIsPause(bool _flag)
{
	mbIsPause = _flag;
	if (!_flag) 
	{
		SetActiveKeyInput(mnInputHandle); 	// 再度入力モードにする
	}
}

int TextInputManager::CheckInput() const
{
	return mnKeyInputState;
}

const char* TextInputManager::GetInputString() const
{
	return mInputString;
}

void TextInputManager::StateInit()
{
	mnKeyInputState = 0; 
	for (int i = 0; i < 256; i++)
	{
		mInputString[i] = mPauseInputString[i] = '\0';
	}
	SetActiveKeyInput(mnInputHandle);
}
