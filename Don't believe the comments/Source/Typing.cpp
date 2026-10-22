#include "Typing.h"
#include <memory>
#include "DxLib.h"

// 変換辞書
// ん は挙動が特殊で別の変換にするためリストには入れていない
const std::unordered_map<std::string_view, std::string_view> Typing::KANA_DICTIONARY = 
{
		{"a","あ"},{"i","い"},{"u","う"},{"e","え"},{"o","お"},

		{"ka","か"},{"ki","き"},{"ku","く"},{"ke","け"},{"ko","こ"},

		{"sa","さ"},{"si","し"},{"su","す"},{"se","せ"},{"so","そ"},

		{"ta","た"},{"ti","ち"},{"tu","つ"},{"te","て"},{"to","と"},

		{"na","な"},{"ni","に"},{"nu","ぬ"},{"ne","ね"},{"no","の"},

		{"ha","は"},{"hi","ひ"},{"hu","ふ"},{"he","へ"},{"ho","ほ"},

		{"ma","ま"},{"mi","み"},{"mu","む"},{"me","め"},{"mo","も"},

		{"ya","や"},{"yi","い"}, {"yu", "ゆ"},{"ye","いぇ"}, { "yo", "よ" },

		{"ra", "ら"}, {"ri", "り"}, {"ru", "る"}, {"re", "れ"}, {"ro", "ろ"},

		{"wa", "わ"},{"wi","うぃ"},{"wu","う"},{"we","うぇ"}, {"wo", "を"},

		{"ga", "が"}, {"gi", "ぎ"}, {"gu", "ぐ"}, {"ge", "げ"}, {"go", "ご"},

		{"za", "ざ"}, {"zi", "じ"}, {"zu", "ず"}, {"ze", "ぜ"}, {"zo", "ぞ"},

		{"da", "だ"}, {"di", "ぢ"}, {"du", "づ"}, {"de", "で"}, {"do", "ど"},

		{"ba", "ば"}, {"bi", "び"}, {"bu", "ぶ"}, {"be", "べ"}, {"bo", "ぼ"},

		{"pa", "ぱ"}, {"pi", "ぴ"}, {"pu", "ぷ"}, {"pe", "ぺ"}, {"po", "ぽ"},

		{"kya", "きゃ"},{"kyi","きぃ"}, {"kyu", "きゅ"},{"kye","きぇ"}, {"kyo", "きょ"},

		{"sya", "しゃ"},{"syi","しぃ"}, { "syu", "しゅ" },{"sye","しぇ"}, {"syo", "しょ"},

		{"tya", "ちゃ"},{"tyi","ちぃ"}, {"tyu", "ちゅ"},{"tye","ちぇ"}, {"tyo", "ちょ"},

		{"nya", "にゃ"},{"nyi","にぃ"}, {"nyu", "にゅ"},{"nye","にぇ"}, {"nyo", "にょ"},

		{"hya", "ひゃ"},{"hyi","ひぃ"}, { "hyu", "ひゅ" },{"hye","ひぇ"}, {"hyo", "ひょ"},

		{"mya", "みゃ"},{"myi","みぃ"}, { "myu", "みゅ" },{"mye","みぇ"}, {"myo", "みょ"},

		{"rya", "りゃ"},{"ryi","りぃ"}, { "ryu", "りゅ" },{"rye","りぇ"}, {"ryo", "りょ"},

		{"gya", "ぎゃ"},{"gyi","ぎぃ"}, {"gyu", "ぎゅ"},{"gye","ぎぇ"}, {"gyo", "ぎょ"},

		{"zya", "じゃ"},{"zyi","じぃ"}, {"zyu", "じゅ"},{"zye","じぇ"}, {"zyo", "じょ"},

		{"bya", "びゃ"},{"byi","びぃ"}, {"byu", "びゅ"},{"bye","びぇ"}, {"byo", "びょ"},

		{"pya", "ぴゃ"},{"pyi","ぴぃ"}, {"pyu", "ぴゅ"},{"pye","ぴぇ"}, {"pyo", "ぴょ"},

		{"vya", "ヴゃ"},{"vyi","ヴぃ"}, {"vyu", "ヴゅ"},{"vye","ヴぇ"}, {"vyo", "ヴょ"},

		{"va", "ヴぁ"}, {"vi", "ヴぃ"},{"vu", "ヴ"}, {"ve", "ヴぇ"}, {"vo", "ヴぉ"},

		{"sha", "しゃ"},{"shi","し"},{"shu","しゅ"},{"she","しぇ"},{"sho","しょ"},

		{"ja","じゃ"}, {"ji","じ"}, {"ju","じゅ"}, {"je", "じぇ"}, {"jo","じょ"},

		{"cha","ちゃ"}, {"chi","ち"}, {"chu","ちゅ"}, {"che", "ちぇ"},{"cho","ちょ"},

		{"tha", "てゃ"}, {"thi", "てぃ"}, {"thu", "てゅ"}, {"the", "てぇ"}, {"tho", "てょ"},

		{"dha", "でゃ"}, {"dhi", "でぃ"}, {"dhu", "でゅ"}, {"dhe", "でぇ"}, {"dho", "でょ"},

		{"twa", "とぁ"}, {"twi", "とぃ"}, {"twu", "とぅ"}, {"twe", "とぇ"}, {"two", "とぉ"},

		{"dwa", "どぁ"}, {"dwi", "どぃ"}, {"dwu", "どぅ"}, {"dwe", "どぇ"}, {"dwo", "どぉ"},

		{"fa", "ふぁ"}, {"fi", "ふぃ"}, {"fu", "ふ"}, {"fe", "ふぇ"}, {"fo", "ふぉ"},

		{"wha", "うぁ"}, {"whi", "ゐ"}, {"whu", "う"}, {"whe", "ゑ"}, {"who", "うぉ"},

		{"qa","くぁ"}, {"qi","くぃ"}, {"qu","く"}, {"qe","くぇ"}, {"qo","くぉ"},

		{"gwa","ぐぁ"}, {"gwi","ぐぃ"}, {"gwu","ぐ"}, {"gwe","ぐぇ"}, {"gwo","ぐぉ"},

		{"swa", "すぁ"}, {"swi", "すぃ"}, {"swu", "す"}, {"swe", "すぇ"}, {"swo", "すぉ"},

		{"zwa", "ずぁ"}, {"zwi", "ずぃ"}, {"zwu", "ず"}, {"zwe", "ずぇ"}, {"zwo", "ずぉ"},

		{"tsa", "つぁ"},{"tsi","つぃ"},{"tsu","つ"},{"tse","つぇ"},{"tso","つぉ"},

		{"qwa","くぁ"},{"qwi","くぃ"},{"qwu","く"},{"qwe","くぇ"},{"qwo","くぉ"},

		{"xa", "ぁ"}, {"xi", "ぃ"}, {"xu", "ぅ"}, {"xe", "ぇ"}, {"xo", "ぉ"},

		{"la", "ぁ"}, {"li", "ぃ"}, {"lu", "ぅ"}, {"le", "ぇ"}, {"lo", "ぉ"},

		//{"lka", "ゕ"}, {"lke", "ゖ"},{"xka", "ゕ"}, {"xke", "ゖ"},

		{"lya", "ゃ"}, {"lyu", "ゅ"}, {"lyo", "ょ"},{"xya", "ゃ"}, {"xyu", "ゅ"}, {"xyo", "ょ"},

		//{"lwa", "ゎ"},{"xwa", "ゎ"},

		{"xtu", "っ"}, {"ltu","っ"},

		{"ca","か"},{"ci","し"},{"cu","く"},{"ce","せ"},{"co","こ"},
	};

// 逆引き変換辞書。
// shi や tsu などは書かず一意にする
// 小文字と大文字は分割する
// ん -> nn とする
const std::unordered_map<std::string_view, std::string_view> Typing::ROMA_DICTIONARY =
{
	{"あ","a"},{"い","i"},{"う","u"},{"え","e"},{"お","o"},

	{"か","ka"},{"き","ki"},{"く","ku"},{"け","ke"},{"こ","ko"},

	{"さ","sa"},{"し","si"},{"す","su"},{"せ","se"},{"そ","so"},

	{"た","ta"},{"ち","ti"},{"つ","tu"},{"て","te"},{"と","to"},

	{"な","na"},{"に","ni"},{"ぬ","nu"},{"ね","ne"},{"の","no"},

	{"は","ha"},{"ひ","hi"},{"ふ","hu"},{"へ","he"},{"ほ","ho"},

	{"ま","ma"},{"み","mi"},{"む","mu"},{"め","me"},{"も","mo"},

	{"や", "ya"}, {"ゆ", "yu"}, {"よ", "yo"},

	{"ら", "ra"}, {"り", "ri"}, {"る", "ru"}, {"れ", "re"}, {"ろ", "ro"},

	{"わ", "wa"}, {"を", "wo"},{"ん","nn"},

	{"が", "ga"}, {"ぎ", "gi"}, {"ぐ", "gu"}, {"げ", "ge"}, {"ご", "go"},

	{"ざ", "za"}, {"じ", "zi"}, {"ず", "zu"}, {"ぜ", "ze"}, {"ぞ", "zo"},

	{"だ", "da"}, {"ぢ", "di"}, {"づ", "du"}, {"で", "de"}, {"ど", "do"},

	{"ば", "ba"}, {"び", "bi"}, {"ぶ", "bu"}, {"べ", "be"}, {"ぼ", "bo"},

	{"ぱ", "pa"}, {"ぴ", "pi"}, {"ぷ", "pu"}, {"ぺ", "pe"}, {"ぽ", "po"},

	{"っ", "xtu"},{"ゃ", "xya"},{"ゅ", "xyu"},{"ょ", "xyo"},

	{"ぁ", "xa"},{"ぃ", "xi"},{"ぅ", "xu"},{"ぇ", "xe"},{"ぉ", "xo"},

	{"ー", "-"}
};

const std::string Typing::KanaDictionary(const std::string& _roma, bool _isNormalize)
{
	// 変換情報があるのなら余計な情報になるため一度クリア
	if (!mConvertionUnits.empty() || mInConvertion != ConversionUnit())
	{
		mInConvertion = ConversionUnit();
		mConvertionUnits.clear();
		mnCurrentDraw = 0;
		mnCurrentKanaByte = 0;
		mnCurrentRomaByte = 0;
	}



	if (_roma.empty())
	{
		return _roma;
	}

	mKana.clear();

	std::string ret = "";

	std::vector<char> dictionaryTmp{};

	for (size_t i = 0; i < _roma.size(); i++)
	{
		// 小文字に変換
		int romaLower = tolower(_roma[i]);
		if (_roma[i] == '-')
		{
			ret += "ー";
			// アルファベット（全角の変換）
			if (_isNormalize)
			{
				AddConvertion(1, 2, 1, 2, 1);
				mnCurrentKanaByte += 2;
				mnCurrentRomaByte += 2;
				mnCurrentDraw++;
			}
			continue;
		}

		/*if (!IsAlphabet(romaLower))
		{
			continue;
		}*/

		// 子音 + 母音の変換
		if (CharactorConvertion(romaLower, ret, dictionaryTmp, _isNormalize))
		{
			mKana = ret;
			continue;
		}

		// 「っ」「ん」 の変換
		if ((i + 1) < _roma.size())
		{
			// 次の文字を小文字に変換
			int nextRomaLower = tolower(_roma[i + 1]);

			int index = static_cast<int>(i);
			if (NConvertion(romaLower, nextRomaLower, ret, dictionaryTmp, index, _isNormalize)
				|| ConstantConvertion(romaLower, nextRomaLower, ret, dictionaryTmp, index))
			{
				mKana = ret;
				i = static_cast<size_t>(index);
				continue;
			}
		}
		dictionaryTmp.push_back(romaLower);
	}

	// 入力途中で残ったローマ字を追加
	if (!dictionaryTmp.empty())
	{
		std::string pending(dictionaryTmp.begin(), dictionaryTmp.end());
		for (int i = 0; i < pending.size(); i++)
		{
			char modify = pending[i];
	
			// 変換できない文字は半角を全角に変換して返す
			if(IsAlphabet(modify))	
			{
				ret += AlphabetHalfToFull(modify);
			}
			else if(IsNumber(modify))
			{
				ret += NumberHalfToFull(modify);
			}
	
			// アルファベット（全角の変換）
			if (_isNormalize)
			{
				AddConvertion(1, 2, 1, 2, 1);
				mnCurrentKanaByte += 2;
				mnCurrentRomaByte += 2;
				mnCurrentDraw++;
			}
		}
	}
	mRoma = _roma;

	// 変換できた場合は、変換後の文字列を返す
	if (!ret.empty())
	{
		return ret;
	}
	else
	{
		return _roma;
	}
}

const std::string Typing::RomaDictionary(const std::string& _kana)
{
	std::string ret = "";

	for (size_t i = 0; i < _kana.size();)
	{
		unsigned char charByte = static_cast<unsigned char>(_kana[i]);

		// その文字のバイト数
		int charLength = 1;
		// 全角文字の区画ならば
		if (IsCharFullByte(charByte))
		{
			charLength = 2;
		}

		// 文字を取り出す
		std::string kana = _kana.substr(i, charLength);

		auto itr = ROMA_DICTIONARY.find(kana);
		if (itr != ROMA_DICTIONARY.end())
		{
			ret += itr->second;
		}
		else
		{
			ret += kana;
		}
		// 探索した文字分進める
		i += kana.size();
	}

	return ret;
}

const std::string Typing::KanaNormalize(const std::string& _kana)
{
	mRoma = RomaDictionary(_kana);
	ConvertionFullToHalf(mRoma);
	mKana = KanaDictionary(mRoma, true);

	return mKana;
}

bool Typing::CharactorConvertion(char _charactor, std::string& _str, std::vector<char>& _roma, bool _isNormalize)
{
	// 母音じゃない -> 変換ができていない
	// ん は除外する
	if (!IsRomaVowel(_charactor))
	{
		return false;
	}

	// 最後の文字を入れる
	_roma.push_back(_charactor);

	// 検索用にヌル文字追加
	_roma.push_back('\0');

	// 検索して変換
	auto itr = KANA_DICTIONARY.find(_roma.data());
	if (itr != KANA_DICTIONARY.end())
	{
		_roma.pop_back();	// 末尾のヌル文字を除外

		if (_isNormalize)
		{
			AddConvertion(
				static_cast<int>(_roma.size()),
				static_cast<int>(itr->second.size()),
				static_cast<int>(_roma.size()),
				static_cast<int>(itr->second.size()),
				1);
			mnCurrentKanaByte = mInConvertion.GetKanaEnd();
			mnCurrentRomaByte = mInConvertion.GetRomaEnd();
			mnCurrentDraw++;
		}

		// 変換文字を入れる
		_str += itr->second;
		
		// 次の変換時に余計な文字が入らないようにクリア
		_roma.clear();
	}
	else
	{
		// もしも検索出来なかった場合、検索用ヌル文字を削除する
		_roma.pop_back();
	}

	return true;
}

bool Typing::NConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index, bool _isNormalize)
{
	if (!IsN(_charactor,_nextCharactor))
	{
		return false;
	}

	std::string kana = "ん";

	if (_charactor == _nextCharactor && _charactor == 'n')
	{
		_index += 1;

		if(_isNormalize)
		{
			AddConvertion(2, 2, 2, 2, 1);

			mnCurrentDraw++;
			mnCurrentKanaByte += 2;
			mnCurrentRomaByte += 2;
		}
	}

	_str += kana;

	_roma.clear();

	return true;
}

bool Typing::ConstantConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index)
{
	if (!IsRomaConsonant(_charactor,_nextCharactor))
	{
		return false;
	}

	std::string kana = "っ";

	_str += kana;

	// 探索1つ進める
	_index++;

	_roma.clear();

	// 次文字を入れる
	_roma.push_back(_nextCharactor);

	return true;
}

void Typing::ConvertionFullToHalf(std::string& _roma)
{
	for (int i = 0; i < _roma.size();)
	{
		unsigned char charByte = static_cast<unsigned char>(_roma[static_cast<size_t>(i)]);

		// 全角の区間じゃないならば変換の必要はない
		if (!IsCharFullByte(charByte))
		{
			i++;
			continue;
		}

		// 変換用文字列
		// 全角のバイトを得る
		std::string tmp = _roma.substr(static_cast<size_t>(i), 2);

		char modify = '\0';	// 全角から半角に変換する文字
		if (IsFullAlphabet(tmp))
		{
			// 全角アルファベットを半角アルファベットへ
			modify = AlphabetFullToHalf(tmp);
		}
		else if (IsFullNumber(tmp))
		{
			modify = NumberFullToHalf(tmp);
		}

		// アルファベットの全角削除
		_roma.erase(static_cast<size_t>(i), 2);
		// 変換したアルファベットを入れる
		_roma += modify;
		i = 0;
	}
}

std::string Typing::AlphabetHalfToFull(char _charactor)
{
	if (!IsAlphabet(_charactor))
	{
		// 半角アルファベット以外はそのまま返す
		return std::string(1,_charactor);
	}

	// アルファベットの 半角 -> 全角 
	// 変換用の数
	constexpr int convertionCharactorAlphabet = 0x8220;

	// 半角アルファベットを全角文字に変換
	// 変換後のコードポイントを求める
	int codePoint = static_cast<int>(_charactor) + convertionCharactorAlphabet;

	// 16進数単位の上位バイトと下位バイト取得
	unsigned char highByte = (codePoint >> 8) & 0xFF;	// 上位バイトを取得
	unsigned char lowByte = codePoint & 0xFF;			// 下位バイトを取得
	
	// 変換後の文字列を返す
	return std::string({ static_cast<char>(highByte), static_cast<char>(lowByte) });
}

char Typing::AlphabetFullToHalf(const std::string& _fullCharactor)
{
	// 全角アルファベットの文字列が2バイトであることを確認
	// 2バイト未満の場合は、全角アルファベットではないため、そのまま返す
	if (_fullCharactor.size() < 2)
	{
		// 全角アルファベットは2バイトなので、2バイト以外はそのまま返す
		return _fullCharactor[0];
	}

	constexpr int convertionCharactorAlphabet = 0x8220;

	std::pair<unsigned char, unsigned char> code = { static_cast<unsigned char>(_fullCharactor[0]),static_cast<unsigned char>(_fullCharactor[1]) };

	// 全角アルファベットのコードポイントを求める
	int codePoint = (code.first << 8) | code.second;

	// 全角アルファベットのコードポイントから 0x8220 を引くことで、半角アルファベットのコードポイントを求める
	int halfCodePoint = codePoint - convertionCharactorAlphabet;

	return static_cast<char>(halfCodePoint);
}

std::string Typing::NumberHalfToFull(char _charactor)
{
	// それが数字でないのならば
	if (!IsNumber(_charactor))
	{
		return std::string(_charactor, 1);
	}

	// 数字の 半角 -> 全角
	// 変換用の数
	constexpr int convertionNumberCharactor = 0x821F;

	int codePoint = static_cast<int>(_charactor) + convertionNumberCharactor;

	// 16進数単位の上位バイトと下位バイト取得
	unsigned char highByte = (codePoint >> 8) & 0xFF;	// 上位ビットを取得
	unsigned char lowByte = codePoint & 0xFF;			// 下位ビットを取得

	return std::string({ static_cast<char>(highByte),static_cast<char>(lowByte) });
}

char Typing::NumberFullToHalf(const std::string& _fullCharactor)
{
	// 全角数字の文字列が2バイトであることを確認
	// 2バイト未満の場合は、全角数字ではないため、そのまま返す
	if (_fullCharactor.size() < 2)
	{
		// 全角数字は2バイトなので、2バイト以外はそのまま返す
		return _fullCharactor[0];
	}

	// 数字の 全角 -> 半角
	// 変換用の数
	constexpr int convertionNumberCharactor = 0x821F;

	std::pair<unsigned char, unsigned char> code = { static_cast<unsigned char>(_fullCharactor[0]),static_cast<unsigned char>(_fullCharactor[1]) };

	// 全角数字のコードポイントを求める
	int codePoint = (code.first << 8) | code.second;

	// 全角文字のコードポイントから 0x821F を引くことで、半角文字のコードポイントを求める
	int halfCodePoint = codePoint - convertionNumberCharactor;

	return static_cast<char>(halfCodePoint);
}

bool Typing::IsRomaVowel(char _charactor)
{
	// 母音あいうえお確認
	return (_charactor == 'a')
		|| (_charactor == 'i')
		|| (_charactor == 'u')
		|| (_charactor == 'e')
		|| (_charactor == 'o');
}

bool Typing::IsAlphabet(char _charactor)
{
	constexpr char minAlphabetSmallCharactor = 'a';	// a
	constexpr char maxAlphabetSmallCharactor = 'z';	// z
	constexpr char minAlphabetLargeCharactor = 'A';	// A
	constexpr char maxAlphabetLargeCharactor = 'Z';	// Z

	return (_charactor >= minAlphabetSmallCharactor && _charactor <= maxAlphabetSmallCharactor)
		|| (_charactor >= minAlphabetLargeCharactor && _charactor <= maxAlphabetLargeCharactor);
}

bool Typing::IsNumber(char _charactor)
{
	constexpr char minNumberCharactor = '0';
	constexpr char maxNumberCharactor = '9';

	return _charactor >= minNumberCharactor && _charactor <= maxNumberCharactor;
}

bool Typing::IsRomaConsonant(char _charactor, char _nextCharactor)
{
	// 今の文字と次の文字のどちらかが母音なら
	bool isVowel = IsRomaVowel(_charactor) || IsRomaVowel(_nextCharactor);
	// どちらかがアルファベットで無いか 
	bool isNotAlphabet = !IsAlphabet(_charactor) || !IsAlphabet(_nextCharactor);
	// どっちも「n」であるか（nn -> ん）
	bool isN = (_charactor == _nextCharactor) && (_charactor == 'n');

	// 「っ」への変換判定をしていいのか
	if (isVowel || isNotAlphabet || isN)
	{
		return false;
	}

	// 同じ文字ならば「っ」に変換
	return _charactor == _nextCharactor;
}

bool Typing::IsN(char _charactor, char _nextCharactor)
{
	// 最初の文字が n ですらないのなら無し
	if (_charactor != 'n')
	{
		return false;
	}

	// 次の文字が母音なら ん ではない
	return _nextCharactor == 'n' || !IsRomaVowel(_nextCharactor);
}

bool Typing::IsCharFullByte(unsigned char _charactorByte)
{
	//!(0x81 <= charByte && charByte <= 0x9F
	//	|| 0xE0 <= charByte && charByte <= 0xFC)

	constexpr unsigned char minFullCharactorHighByte[] = { unsigned char(0x81u),unsigned char(0xE0u) };	// 全角区間
	constexpr unsigned char maxFullCharactorHighByte[] = { unsigned char(0x9Fu),unsigned char(0xFCu) };	// 全角区間
	
	return (_charactorByte >= minFullCharactorHighByte[0] && _charactorByte <= maxFullCharactorHighByte[0])
		|| (_charactorByte >= minFullCharactorHighByte[1] && _charactorByte <= maxFullCharactorHighByte[1]);
}

bool Typing::IsFullAlphabet(const std::string& _fullCharactor)
{
	constexpr int minFullLargeCharactor = 0x8260;	// Ａ
	constexpr int maxFullLargeCharactor = 0x8279;	// Ｚ
	constexpr int minFullSmallCharactor = 0x8281;	// ａ
	constexpr int maxFullSmallCharactor = 0x829A;	// ｚ

	int codePoint = (static_cast<unsigned char>(_fullCharactor[0]) << 8) | static_cast<unsigned char>(_fullCharactor[1]);

	return ((codePoint >= minFullLargeCharactor) && (codePoint <= maxFullSmallCharactor))
		|| ((codePoint >= minFullSmallCharactor) && (codePoint <= maxFullLargeCharactor));
}

bool Typing::IsFullNumber(const std::string& _fullCharactor)
{
	constexpr int minFullNumberCharactor = 0x824F;	// ０
	constexpr int maxFullNumberCharactor = 0x8258;	// ９

	int codePoint = (static_cast<unsigned char>(_fullCharactor[0]) << 8) | static_cast<unsigned char>(_fullCharactor[1]);

	return (codePoint >= minFullNumberCharactor)
		&& (codePoint <= maxFullNumberCharactor);
}

void Typing::DeleteChar()
{
	// 変換などが何もない時
	if (mConvertionUnits.empty())
	{
		return;
	}

	if ((mnCurrentDraw <= 0) || (mConvertionUnits.size() < (static_cast<size_t>(mnCurrentDraw))))
	{
		return;
	}

	// 消す位置を求める
	int deletePosition = mnCurrentDraw - 1;

	// そのカーソルの場所の情報を得る
	decltype(auto) unit = mConvertionUnits.at(static_cast<size_t>(deletePosition));

	// 削除
	mRoma.erase(unit.romaBegin, unit.romaLength);
	mKana.erase(unit.kanaBegin, unit.kanaLength);

	// 消した情報を得る
	decltype(auto) deleteUnit = unit;

	mConvertionUnits.erase(mConvertionUnits.begin() + deletePosition);

	clsDx();
	// 消した分の情報を修正
	// 消した文字の右側にある数は全て修正
	for (size_t i = deletePosition; i < mConvertionUnits.size(); i++)
	{
		decltype(auto) modify = mConvertionUnits[i];
		
		modify.romaBegin -= deleteUnit.romaLength;
		modify.kanaBegin -= deleteUnit.kanaLength;
		modify.kanaCharBegin--;

		printfDx("romaBegin:%d romaLength:%d kanaLength:%d kanaBegin:%d %d文字目\n", modify.romaBegin, modify.romaLength, modify.kanaBegin, modify.kanaLength, modify.kanaCharBegin);
	}

	// 各変数を変更
	mnCurrentRomaByte -= deleteUnit.romaLength;
	mnCurrentKanaByte -= deleteUnit.kanaLength;
	mnCurrentDraw--;
}

//void Typing::ShiftLeftCursor()
//{
//	// 変換などが何もない時
//	if (mConvertionUnits.empty())
//	{
//		return;
//	}
//
//	if (mnCurrentDraw <= 0)
//	{
//		return;
//	}
//
//	// 前位置を求める
//	int position = --mnCurrentDraw;
//
//	// そのカーソル位置の情報を得る
//	const auto& unit = mConvertionUnits.at(static_cast<size_t>(position));
//
//	mnCurrentRomaByte -= unit.romaLength;
//	mnCurrentKanaByte -= unit.kanaLength;
//}
//
//void Typing::ShiftRightCursor()
//{
//	// 変換などが何もない時
//	if (mConvertionUnits.empty())
//	{
//		return;
//	}
//
//	if (mConvertionUnits.size() <= (static_cast<size_t>(mnCurrentDraw)))
//	{
//		return;
//	}
//
//	// 次位置を求める
//	int position = mnCurrentDraw++;
//
//	// そのカーソル位置の情報を得る
//	const auto& unit = mConvertionUnits.at(static_cast<size_t>(position));
//	
//	mnCurrentRomaByte += unit.romaLength;
//	mnCurrentKanaByte += unit.kanaLength;
//}

void Typing::AddConvertion(int _romaLength, int _kanaLength, int _addRomaBegin, int _addKanaBegin, int _addKanaCharLength)
{
	mInConvertion.romaLength = _romaLength;
	mInConvertion.kanaLength = _kanaLength;

	mConvertionUnits.emplace_back(mInConvertion);

	mInConvertion.romaBegin += _addRomaBegin;
	mInConvertion.kanaBegin += _addKanaBegin;
	mInConvertion.kanaCharBegin += _addKanaCharLength;
}

const std::vector<ConversionUnit>& Typing::GetConvertionUnits()
{
	return mConvertionUnits;
}

int Typing::GetCurrentRomaByte() const
{
	return mnCurrentRomaByte;
}

int Typing::GetCurrentKanaByte() const
{
	return mnCurrentKanaByte;
}

int Typing::GetCurrentDraw() const
{
	return mnCurrentDraw;
}

const std::string& Typing::GetRoma() const
{
	return mRoma;
}

const std::string& Typing::GetKana() const
{
	return mKana;
}

void Typing::ClearString()
{
	mRoma.clear();
	mKana.clear();
	mConvertionUnits.clear();
	mnCurrentDraw = 0;
	mnCurrentKanaByte = 0;
	mnCurrentRomaByte = 0;
	mInConvertion = ConversionUnit();
\
}
