#include "TextInputManager.h"
#include "Typing.h"
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

	//SetKeyInputStringFont(_fontHandle);

	//mnInputHandle = MakeKeyInput(sizeof(mInputString) - 1, TRUE, TRUE, FALSE);

	//SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR_BACK, KEYINPUT_BACK_COLOR);			// 不確定文字列の背景色変更
	//SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR, KEYINPUT_BACK_COLOR);		// 変換中文字列の背景色変更

	//SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, KEYINPUT_STRING_COLOR);	// 入力文字の色変更
	//SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR, KEYINPUT_STRING_COLOR);		// 入力中文字列の色変更

	//// 変換中文字列も文字数制限の中に入れる
	//SetInputStringMaxLengthIMESync(TRUE);

	//// 入力モードにする
	//SetActiveKeyInput(mnInputHandle);
}

void TextInputManager::Draw()
{
	if (mbIsFinal)
	{
		return;
	}

	if (mnKeyInputState == 0)
	{
		// サイズ計測
		int width = 0, height = 0;
		int cursorLineX = 0;
		GetDrawFormatStringSizeToHandle(&width, &height, NULL, mnFontHandle, "%s", mOutputString.c_str());
		int centerX = (mnKeyInputDrawX - (width / 2));
		cursorLineX = centerX + GetDrawFormatStringWidthToHandle(mnFontHandle, "あ") * mnDrawCursorPosition; // カーソル位置を計算
		
		// サイズに合わせて中央表示
		DrawFormatStringToHandle(centerX, mnKeyInputDrawY, KEYINPUT_STRING_COLOR, mnFontHandle, "%s", mOutputString.c_str());
		DrawLine(cursorLineX, mnKeyInputDrawY, cursorLineX, mnKeyInputDrawY + height, GetColor(128, 128, 128), 2);
	}
}

void TextInputManager::Update()
{
	if (mbIsFinal)
	{
		return;
	}

	// 入力出来たか確認
	//mnKeyInputState = CheckKeyInput(mnInputHandle);
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		mnKeyInputState = 1;
		if (mInputString == "/p" || mInputString == "/ｐ" || mInputString == "・p" || mInputString == "・ｐ")
		{
			mbIsPause = true;
			mnKeyInputState = 2;
		}
	}

	if(mnKeyInputState == 0)
	{
		// 押したボタン（入力した文字）を得る
		char inputChar = GetInputChar(TRUE);

		// 
		if (inputChar != 0)
		{
			ControllInput(inputChar);
		}
	}

	if (!mbIsPause)
	{
		return;
	}

	// ポーズした時の処理
	if (mnKeyInputState == 2)	// キャンセル時
	{
		// 気が向いたら書く
	}
}

void TextInputManager::Finalize()
{
	// ゲームが終了したことを伝える
	mbIsFinal = true;
}

void TextInputManager::SetKeyInputDrawPosition(int _x, int _y)
{
	mnKeyInputDrawX = _x;
	mnKeyInputDrawY = _y;
}

void TextInputManager::SetIsPause(bool _flag)
{
	mbIsPause = _flag;
	//if (!_flag)
	//{
	//	SetActiveKeyInput(mnInputHandle); 	// 再度入力モードにする
	//}
}

int TextInputManager::CheckInput() const
{
	return mnKeyInputState;
}

const std::string& TextInputManager::GetInputString() const
{
	return mOutputString;
}

void TextInputManager::ResetState()
{
	mnKeyInputState = 0;
}

void TextInputManager::ResetInput()
{
	mInputString = "";
	mOutputString = "";
	mTyping.ClearString();
}

void TextInputManager::ControllInput(char _input)
{
	// 全角文字の数
	bool isStringMaxLength = (mOutputString.size() <= static_cast<size_t>(KETINPUT_STRING_MAX_LENGTH_FULL_CHARACTOR * 2 + 1));
	if (_input >= CTRL_CODE_CMP)
	{
		if (!isStringMaxLength)
		{
			return;
		}
		// 入力を加えるただし、スペース、数字、記号は無視する
 		if ((_input != ' ' && (mTyping.IsAlphabet(_input) || mTyping.IsNumber(_input))) || _input == '-')
			mInputString += _input;
		// 入力を変換
		mOutputString = mTyping.KanaDictionary(mInputString);

		mTyping.KanaNormalize(mOutputString);

		if (!mTyping.GetConvertionUnits().empty())
			mnDrawCursorPosition = mTyping.GetConvertionUnits().back().GetKanaCharEnd();
	}
	else
	{
		/*if (_input == CTRL_CODE_LEFT)
		{
			mTyping.ShiftLeftCursor();
		}
		else if (_input == CTRL_CODE_RIGHT)
		{
			mTyping.ShiftRightCursor();
		}*/
		if (_input == CTRL_CODE_BS)
		{
			mTyping.DeleteChar();
		}
	}
	mnDrawCursorPosition = mTyping.GetCurrentDraw();
	mnRomaCursorPosition = mTyping.GetCurrentRomaByte();
	mnKanaCursorPosition = mTyping.GetCurrentKanaByte();

	mInputString = mTyping.GetRoma();
	mOutputString = mTyping.GetKana();
}
