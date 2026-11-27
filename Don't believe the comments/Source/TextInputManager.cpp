#include "TextInputManager.h"
#include "Typing.h"
#include "DxLib.h"
#include "Master.h"

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

	mbIsFinal = false;

	ResetState();
	ResetInput();

	ClearInputCharBuf();
}

void TextInputManager::Draw()
{
	if (mbIsFinal)
	{
		return;
	}

	if (mnKeyInputState == 0)
	{
		std::string drawString = mbIsAlphabet ? mOutputAlphabetString : mOutputString;

		// サイズ計測
		int width = 0, height = 0;
		GetDrawFormatStringSizeToHandle(&width, &height, NULL, mnFontHandle, "%s", drawString.c_str());
		int centerX = (mnKeyInputDrawX - (width / 2));

		// サイズに合わせて中央表示
		DrawFormatStringToHandle(centerX, mnKeyInputDrawY, KEYINPUT_STRING_COLOR, mnFontHandle, "%s", drawString.c_str());
	}
}

void TextInputManager::Update()
{
	if (mbIsFinal)
	{
		ClearInputCharBuf();
		return;
	}

	// 入力出来たか確認
	//mnKeyInputState = CheckKeyInput(mnInputHandle);
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		mnKeyInputState = 1;
		if (mOutputString.size() >= 2 && mOutputString.substr(mOutputString.size() - 2, 2) == "ｎ")
		{
			mOutputString = mOutputString.erase(mOutputString.size() - 2, 2);
			mOutputString += "ん";
		}
	}

	if (mnKeyInputState == 0)
	{
		// 押したボタン（入力した文字）を得る
		char inputChar = GetInputChar(TRUE);

		// 無入力（0）ではないなら
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

const std::string& TextInputManager::GetInputStringAlphabet() const
{
	return mOutputAlphabetString;
}

void TextInputManager::ResetState()
{
	mnKeyInputState = 0;
}

void TextInputManager::ResetInput()
{
	mInputString.clear();
	mOutputString.clear();
	mOutputAlphabetString.clear();
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
		// 入力を加えるただし、スペース、記号は無視する
		if ((_input != ' ' && (mTyping.IsAlphabet(_input) || mTyping.IsNumber(_input))) || _input == '-')
			mInputString += _input;
		// 入力を変換
		if (mbIsAlphabet)
		{
			// アルファベットモード
			mOutputAlphabetString = mTyping.Alphabet(mInputString);
		}
		else
		{
			// ローマ字モード
			mOutputString = mTyping.KanaDictionary(mInputString);
			mTyping.KanaNormalize(mOutputString);	// 正規化（入力内容を一意にする）
		}

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
	mOutputAlphabetString = mTyping.GetAlphabet();
}

void TextInputManager::SetIsAlphabet(bool _flag)
{
	mbIsAlphabet = _flag;
}

bool TextInputManager::IsAlphabet() const
{
	return mbIsAlphabet;
}

void TextInputManager::SetNoInput()
{
	mnKeyInputState = -1;
}
