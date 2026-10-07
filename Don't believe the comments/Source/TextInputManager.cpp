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

	mnInputHandle = MakeKeyInput(sizeof(mInputString) - 1, TRUE, TRUE, FALSE);

	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR_BACK, KEYINPUUT_BACK_COLOR);			// 不確定文字列の背景色変更
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR, KEYINPUUT_BACK_COLOR);		// 変換中文字列の背景色変更

	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, KEYINPUUT_STRING_COLOR);	// 入力文字の色変更
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR, KEYINPUUT_STRING_COLOR);		// 入力中文字列の色変更

	// 変換中文字列も文字数制限の中に入れる
	SetInputStringMaxLengthIMESync(TRUE);

	// 入力モードにする
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
	/*if (char[] == "ESC") {}*/
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

void TextInputManager::ResetState()
{
	mnKeyInputState = 0;
}

void TextInputManager::ResetInput()
{
	for (size_t i = 0; i < sizeof(mInputString); i++)
	{
		mInputString[i] = mPauseInputString[i] = '\0';
	}
	SetActiveKeyInput(mnInputHandle);
	SetKeyInputString("", mnInputHandle);
}

//const std::string& TextInputManager::TextInputDictionary(const std::string& _roma)
//{
//	const std::unordered_map<std::string, std::string> dictionary =
//	{
//		{"a","あ"},{"i","い"},{"u","う"},{"e","え"},{"o","お"},
//		{"ka","か"},{"ki","き"},{"ku","く"},{"ke","け"},{"ko","こ"},
//		{"sa","さ"},{"si","し"},{"su","す"},{"se","せ"},{"so","そ"},
//		{"ta","た"},{"ti","ち"},{"tu","つ"},{"te","て"},{"to","と"},
//		{"na","な"},{"ni","に"},{"nu","ぬ"},{"ne","ね"},{"no","の"},
//		{"ha","は"},{"hi","ひ"},{"hu","ふ"},{"he","へ"},{"ho","ほ"},
//		{"ma","ま"},{"mi","み"},{"mu","む"},{"me","め"},{"mo","も"},
//		{"ya","や"},{"yi","い"}, {"yu", "ゆ"},{"ye","いぇ"}, { "yo", "よ" },
//		{"ra", "ら"}, {"ri", "り"}, {"ru", "る"}, {"re", "れ"}, {"ro", "ろ"},
//		{"wa", "わ"},{"wi","うぃ"},{"wu","う"},{"we","うぇ"}, {"wo", "を"},
//		{"nn", "ん"},
//		{"ga", "が"}, {"gi", "ぎ"}, {"gu", "ぐ"}, {"ge", "げ"}, {"go", "ご"},
//		{"za", "ざ"}, {"zi", "じ"}, {"zu", "ず"}, {"ze", "ぜ"}, {"zo", "ぞ"},
//		{"da", "だ"}, {"di", "ぢ"}, {"du", "づ"}, {"de", "で"}, {"do", "ど"},
//		{"ba", "ば"}, {"bi", "び"}, {"bu", "ぶ"}, {"be", "べ"}, {"bo", "ぼ"},
//		{"pa", "ぱ"}, {"pi", "ぴ"}, {"pu", "ぷ"}, {"pe", "ぺ"}, {"po", "ぽ"},
//		{"kya", "きゃ"},{"kyi","きぃ"}, {"kyu", "きゅ"},{"kye","きぇ"}, {"kyo", "きょ"},
//		{"sya", "しゃ"},{"syi","しぃ"}, { "syu", "しゅ" },{"sye","しぇ"}, {"syo", "しょ"},
//		{"tya", "ちゃ"},{"tyi","ちぃ"}, {"tyu", "ちゅ"},{"tye","ちぇ"}, {"tyo", "ちょ"},
//		{"nya", "にゃ"},{"nyi","にぃ"}, {"nyu", "にゅ"},{"nye","にぇ"}, {"nyo", "にょ"},
//		{"hya", "ひゃ"},{"hyi","ひぃ"}, { "hyu", "ひゅ" },{"hye","ひぇ"}, {"hyo", "ひょ"},
//		{"mya", "みゃ"},{"myi","みぃ"}, { "myu", "みゅ" },{"mye","みぇ"}, {"myo", "みょ"},
//		{"rya", "りゃ"},{"ryi","りぃ"}, { "ryu", "りゅ" },{"rye","りぇ"}, {"ryo", "りょ"},
//		{"gya", "ぎゃ"},{"gyi","ぎぃ"}, {"gyu", "ぎゅ"},{"gye","ぎぇ"}, {"gyo", "ぎょ"},
//		{"zya", "じゃ"},{"zyi","じぃ"}, {"zyu", "じゅ"},{"zye","じぇ"}, {"zyo", "じょ"},
//		{"bya", "びゃ"},{"byi","びぃ"}, {"byu", "びゅ"},{"bye","びぇ"}, {"byo", "びょ"},
//		{"pya", "ぴゃ"},{"pyi","ぴぃ"}, {"pyu", "ぴゅ"},{"pye","ぴぇ"}, {"pyo", "ぴょ"},
//		{"vya", "ゔゃ"},{"vyi","ゔぃ"}, {"vyu", "ゔゅ"},{"vye","ゔぇ"}, {"vyo", "ゔょ"},
//		{"va", "ゔぁ"}, {"vi", "ゔぃ"},{"vu", "ゔ"}, {"ve", "ゔぇ"}, {"vo", "ゔぉ"},
//		{"sha", "しゃ"},{"shi","し"},{"shu","しゅ"},{"she","しぇ"},{"sho","しょ"},
//		{"ja","じゃ"}, {"ji","じ"}, {"ju","じゅ"}, {"je", "じぇ"}, {"jo","じょ"},
//		{"cha","ちゃ"}, {"chi","ち"}, {"chu","ちゅ"}, {"che", "ちぇ"},{"cho","ちょ"},
//		{"tha", "てゃ"}, {"thi", "てぃ"}, {"thu", "てゅ"}, {"the", "てぇ"}, {"tho", "てょ"},
//		{"dha", "でゃ"}, {"dhi", "でぃ"}, {"dhu", "でゅ"}, {"dhe", "でぇ"}, {"dho", "でょ"},
//		{"twa", "とぁ"}, {"twi", "とぃ"}, {"twu", "とぅ"}, {"twe", "とぇ"}, {"two", "とぉ"},
//		{"dwa", "どぁ"}, {"dwi", "どぃ"}, {"dwu", "どぅ"}, {"dwe", "どぇ"}, {"dwo", "どぉ"},
//		{"fa", "ふぁ"}, {"fi", "ふぃ"}, {"fu", "ふ"}, {"fe", "ふぇ"}, {"fo", "ふぉ"},
//		{"wha", "うぁ"}, {"whi", "うぃ"}, {"wu", "う"}, {"whe", "うぇ"}, {"who", "うぉ"},
//		{"qa","くぁ"}, {"qi","くぃ"}, {"qu","く"}, {"qe","くぇ"}, {"qo","くぉ"},
//		{"gwa","ぐぁ"}, {"gwi","ぐぃ"}, {"gwu","ぐ"}, {"gwe","ぐぇ"}, {"gwo","ぐぉ"},
//		{"swa", "すぁ"}, {"swi", "すぃ"}, {"swu", "す"}, {"swe", "すぇ"}, {"swo", "すぉ"},
//		{"zwa", "ずぁ"}, {"zwi", "ずぃ"}, {"zwu", "ず"}, {"zwe", "ずぇ"}, {"zwo", "ずぉ"},
//		{"tsa", "つぁ"},{"tsi","つぃ"},{"tsu","つ"},{"tse","つぇ"},{"tso","つぉ"},
//		{"qwa","くぁ"},{"qwi","くぃ"},{"qwu","く"},{"qwe","くぇ"},{"qwo","くぉ"},
//		{"xtu", "っ"}, {"-","ー"},
//		{"xa", "ぁ"}, {"xi", "ぃ"}, {"xu", "ぅ"}, {"xe", "ぇ"}, {"xo", "ぉ"},
//		{"la", "ぁ"}, {"li", "ぃ"}, {"lu", "ぅ"}, {"le", "ぇ"}, {"lo", "ぉ"},
//		{"lka", "ゕ"}, {"lke", "ゖ"},
//		{"lya", "ゃ"}, {"lyu", "ゅ"}, {"lyo", "ょ"},
//		{"lwa", "ゎ"},
//		{"xka", "ゕ"}, {"xke", "ゖ"},
//		{"xya", "ゃ"}, {"xyu", "ゅ"}, {"xyo", "ょ"},
//		{"xwa", "ゎ"},
//		{"shi","し"}, {"chi","ち"}, {"tsu","つ"},
//		{"ji","じ"}, {"di","ぢ"}, {"du","づ"},
//		{"fu","ふ"},
//		{"ca","か"},{"ci","し"},{"cu","く"},{"ce","せ"},{"co","こ"},
//		{",","、"}, {".","。"}, {"?","？"}, {"!","！"},{" ","　"},
//		{"wu","う"},
//		{"wi","ゐ"},
//		{"we","ゑ"},
//	};
//
//	if(dictionary.find(_roma) != dictionary.end())
//	{
//		return dictionary.at(_roma.c_str());
//	}
//	else
//	{
//		return _roma.c_str();
//	}
//}
