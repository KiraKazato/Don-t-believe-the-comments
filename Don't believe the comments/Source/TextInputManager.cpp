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
	mnFontHandle = _fontHandle;

	SetKeyInputStringFont(_fontHandle);

	mnInputHandle = MakeKeyInput(sizeof(mInputString) - 1, TRUE, FALSE, FALSE);

	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR_BACK, KEYINPUUT_BACK_COLOR);			// 不確定文字列の背景色変更
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR, KEYINPUUT_BACK_COLOR);		// 変換中文字列の背景色変更
	
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, KEYINPUUT_STRING_COLOR);	// 入力文字の色変更
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR, KEYINPUUT_STRING_COLOR);		// 入力中文字列の色変更

	SetInputStringMaxLengthIMESync(TRUE);

	SetActiveKeyInput(mnInputHandle);
}

void TextInputManager::Draw()
{
	if (mnKeyInputState == 0)
	{
		char tmpStr[sizeof(mInputString)]{};
		const IMEINPUTDATA* IMEData = GetIMEInputData();	// 入力中（変換中）データ取得

		GetKeyInputString(tmpStr, mnInputHandle);	// 入力（変換決定済み）データ取得

		// 1. 確定文字列と変換中文字列を結合して「全体の文字列」を作る
		char displayText[sizeof(mInputString)] = "";
		strcpy_s(displayText, sizeof(displayText), tmpStr);
		if (IMEData != nullptr)
		{
			strcat_s(displayText, sizeof(displayText), IMEData->InputString);
		}

		// 横幅
		int width = GetDrawFormatStringWidthToHandle(mnFontHandle, "%s", displayText);

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
	for (size_t i = 0; i < sizeof(mInputString); i++)
	{
		mInputString[i] = mPauseInputString[i] = '\0';
	}
	SetActiveKeyInput(mnInputHandle);
	SetKeyInputString("", mnInputHandle);
}
