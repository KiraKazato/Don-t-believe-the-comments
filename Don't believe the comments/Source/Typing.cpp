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

		{",","、"}, {".","。"}, {"?","？"}, {"!","！"},{" ","　"},{"-","ー"},
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

	{"ー", "-"},{"　"," "},{"、",","},{"。","."},{"？","?"},{"！","!"}
};

const std::string Typing::KanaDictionary(std::string& _roma)
{
	// 変換情報があるのなら余計な情報になるため一度クリア
	if (!mConvertionUnits.empty())
		mConvertionUnits.clear();
	
	if (_roma.empty())
	{
		return _roma;
	}

	std::string ret = "";

	std::vector<char> tmp{};
	

	int romaBegin = 0;
	int kanaBegin = 0;
	int kanaCharBegin = 0;
	for (size_t i = 0; i < _roma.size(); i++)
	{
		// 小文字に変換
		int romaLower = tolower(_roma[i]);

		// 子音 + 母音の変換
		if (CharactorConvertion(romaLower, ret, tmp, kanaBegin, romaBegin, kanaCharBegin))
		{
			continue;
		}

		// 「っ」「ん」 の変換
		if ((i + 1) < _roma.size())
		{
			// 次の文字を小文字に変換
			int nextRomaLower = tolower(_roma[i + 1]);

			int index = static_cast<int>(i);
			if (NConvertion(romaLower, nextRomaLower, ret, tmp, index, romaBegin, kanaBegin, kanaCharBegin)
			 || ConstantConvertion(romaLower, nextRomaLower, ret, tmp, index, romaBegin, kanaBegin, kanaCharBegin))
			{
				i = static_cast<size_t>(index);
				continue;
			}
		}
		tmp.push_back(romaLower);
	}

	// 入力途中で残ったローマ字を追加
	if (!tmp.empty())
	{
		std::string pending(tmp.begin(), tmp.end());

		ret += pending;

		//for (int i = 0; i < pending.size(); i++)
		//{
		//	char alphabet = pending[i];
		//	AddConvertionUnits(
		//		romaBegin + i,
		//		1,
		//		std::string(1, alphabet),
		//		kanaBegin,
		//		kanaCharBegin
		//	);
		//
		//	// 変換できない文字は半角アルファベットを全角に変換して返す
		//	ret += AlphabetHalfToFull(alphabet);
		//}
	}

	_roma = RomaDictionary(_roma);

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
		unsigned char c = static_cast<unsigned char>(_kana[i]);

		// その文字のバイト数
		int charLength = 1;
		// 全角文字の区画ならば
		if (0x81 <= c && c <= 0x9F
			|| 0xE0 <= c && c <= 0xFC)
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

bool Typing::CharactorConvertion(char _charactor, std::string& _str, std::vector<char>& _roma, int& _kanaBegin, int& _romaBegin, int& _kanaCharBegin)
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
		int romaLength = static_cast<int>(_roma.size());		// 変換するローマ字の長さ（バイト）
		int kanaLength = static_cast<int>(itr->second.size());	// 変換後のかなの長さ（バイト）

		// 変換情報を入れる
		AddConvertionUnits(
			_romaBegin,
			romaLength,
			itr->second.data(),
			_kanaBegin,
			_kanaCharBegin
		);

		// 変換文字を入れる
		_str += itr->second;
		
		// 変換した文字の長さ分を足す
		_romaBegin += romaLength;

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

bool Typing::NConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index, int& _romaBegin, int& _kanaBegin, int& _kanaCharBegin)
{
	if (!IsN(_charactor,_nextCharactor))
	{
		return false;
	}

	std::string kana = "ん";

	int romaLength = (_charactor == 'n') ? 2 : 1;

	AddConvertionUnits(
		_index,
		romaLength,          // ローマ字の長さ
		kana,
		_kanaBegin,
		_kanaCharBegin
	);

	_str += kana;

	// AddConvertionUnits が kanaBegin を更新するなら、
	// ここで kanaBegin を再度更新しない
	_romaBegin = _index + romaLength;

	if (_charactor == 'n')
	{
		++(_romaBegin); // 2文字目の n も処理済みにする
	}

	_roma.clear();

	return true;
}

bool Typing::ConstantConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index, int& _romaBegin, int& _kanaBegin, int& _kanaCharBegin)
{
	if (!IsRomaConsonant(_charactor,_nextCharactor))
	{
		return false;
	}

	std::string kana = "っ";

	AddConvertionUnits(
		_index,
		1,          // ローマ字の長さ
		kana,
		_kanaBegin,
		_kanaCharBegin
	);

	_str += kana;
	_romaBegin = _index + 1;

	_roma.clear();

	return true;
}

std::string Typing::AlphabetHalfToFull(char _charactor)
{
	if (!IsAlphabet(_charactor))
	{
		// 半角アルファベット以外はそのまま返す
		return std::string(1,_charactor);
	}

	// 半角アルファベットを全角文字に変換
	// 全角文字のコードポイントは、半角文字のコードポイントに 0xFEE0 を足したものになる

	// 変換後のコードポイントを求める
	int codePoint = static_cast<int>(_charactor) + 0xFEE0;

	unsigned char highByte = (codePoint >> 8) & 0xFF;		// 上位ビットを取得
	unsigned char lowByte = codePoint & 0xFF;	// 下位ビットを取得

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

	// 全角アルファベットのコードポイントを求める
	int codePoint = _fullCharactor[0] | _fullCharactor[1];

	// 全角アルファベットのコードポイントから 0xFEE0 を引くことで、半角アルファベットのコードポイントを求める
	int halfCodePoint = codePoint - 0xFEE0;

	return static_cast<char>(halfCodePoint - 128);	// 128を引くことで、ASCIIコードに変換する
}

bool Typing::IsRomaVowel(char _charactor)
{
	// 母音あいうえお確認
	return ('a' == _charactor)
		|| ('i' == _charactor)
		|| ('u' == _charactor)
		|| ('e' == _charactor)
		|| ('o' == _charactor);
}

bool Typing::IsAlphabet(char _charactor)
{
	return ('a' <= _charactor && _charactor <= 'z') || ('A' <= _charactor && _charactor <= 'Z');
}

bool Typing::IsRomaConsonant(char _charactor, char _nextCharactor)
{
	// 今の文字と次の文字のどちらかが母音なら っ は出さない
	// どちらも n -> ん のになるので っ は出さない
	if (IsRomaVowel(_charactor) || IsRomaVowel(_nextCharactor)
		|| (_charactor == _nextCharactor && _charactor == 'n'))
	{
		return false;
	}

	// 同じ文字ならばそれ っ に変換
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

void Typing::DeleteChar(std::string& _kana, std::string& _roma, int* _kanaCursorPosition, int* _romaCursorPosition, int* _drawCursorPosition)
{
	if (_kana.empty() || _kanaCursorPosition == nullptr || _romaCursorPosition == nullptr || _drawCursorPosition == nullptr)
	{
		return;
	}

	int& romaCursor = *_romaCursorPosition;
	int& kanaCursor = *_kanaCursorPosition;
	int& drawCursor = *_drawCursorPosition;

	//if (drawCursor <)
	{

	}

	//// 入力途中のローマ字が残っている場合
	//if (romaCursor > 0 &&
	//	romaCursor == static_cast<int>(_roma.size()))
	//{
	//	_roma.erase(romaCursor - 1, 1);
	//	romaCursor--;

	//	_kana = KanaDictionary(_roma);
	//	//drawCursor--;
	//	return;
	//}

	//// ローマ字再生成
	//_roma = RomaDictionary(_kana);

	//// 再生成したものをもう一度変換
	//_kana = KanaDictionary(_roma);

	//if (mConvertionUnits.empty() || drawCursor <= 0 || mConvertionUnits.size() <= (static_cast<size_t>(drawCursor) - 1))
	//{
	//	return;
	//}

	//const auto& unit = mConvertionUnits.at(static_cast<size_t>(drawCursor - 1));

	//kanaCursor = unit.GetKanaEnd();
	//romaCursor = unit.GetRomaEnd();

	//if (kanaCursor <= 0 ||
	//	kanaCursor > static_cast<int>(_kana.size()))
	//{
	//	return;
	//}

	//// カーソル直前のバイト
	//int deletePosition = kanaCursor - 1;

	//unsigned char lastByte =
	//	static_cast<unsigned char>(_kana[deletePosition]);

	//int deleteCount = 1;

	//if (deletePosition > 0)
	//{
	//	unsigned char previousByte =
	//		static_cast<unsigned char>(_kana[static_cast<size_t>(deletePosition) - 1]);

	//	// 全角文字の区画ならば
	//	if ((0x81 <= previousByte && previousByte <= 0x9F) ||
	//		(0xE0 <= previousByte && previousByte <= 0xFC))
	//	{
	//		// その中でかなの区画ならば
	//		if ((0x40 <= lastByte && lastByte <= 0xFC) &&
	//			lastByte != 0x7F)
	//		{
	//			deletePosition--;
	//			deleteCount = 2;
	//		}
	//	}
	//}

	//// かな削除
	//_kana.erase(
	//	static_cast<size_t>(unit.kanaBegin),
	//	static_cast<size_t>(unit.kanaLength)
	//);

	//// 削除後のかな側カーソル位置
	//kanaCursor = deletePosition;

	//// 削除後のかなに合わせてローマ字も更新する
	//_roma = RomaDictionary(_kana);

	//// 削除位置のかなをローマ字変換
	//// ローマ字側のカーソル位置を求める
	//std::string prefix = _kana.substr(0, kanaCursor);
	//romaCursor = static_cast<int>(RomaDictionary(prefix).size());

	//if (drawCursor > 0)
	//{
	//	drawCursor--;
	//}
}

void Typing::ShiftLeftCursor(int* _kanaCursorPosition, int* _romaCursorPosition, int* _drawCursorPosition)
{
	if (_kanaCursorPosition == nullptr || _romaCursorPosition == nullptr || _drawCursorPosition == nullptr)
	{
		return;
	}

	int& kanaCursor = *_kanaCursorPosition;
	int& romaCursor = *_romaCursorPosition;
	int& drawCursor = *_drawCursorPosition;

	if (mConvertionUnits.empty() || drawCursor <= 0)
	{
		return;
	}

	const auto& unit = mConvertionUnits.at(static_cast<size_t>(drawCursor) - 1);
	
	romaCursor = unit.romaBegin;
	kanaCursor = unit.kanaBegin;

	drawCursor--;
}

void Typing::ShiftRightCursor(int* _kanaCursorPosition, int* _romaCursorPosition, int* _drawCursorPosition)
{
	if (_kanaCursorPosition == nullptr || _romaCursorPosition == nullptr || _drawCursorPosition == nullptr)
	{
		return;
	}

	int& kanaCursor = *_kanaCursorPosition;
	int& romaCursor = *_romaCursorPosition;
	int& drawCursor = *_drawCursorPosition;

	if (mConvertionUnits.empty() || drawCursor < 0 || mConvertionUnits.size() <= drawCursor)
	{
		return;
	}

	const auto& unit = mConvertionUnits.at(static_cast<size_t>(drawCursor));

	romaCursor = unit.romaBegin;
	kanaCursor = unit.kanaBegin;

	drawCursor++;
}

void Typing::AddConvertionUnits(int _romaBegin, int _romaLength, const std::string& _kana, int& _kanaBegin, int& _kanaCharBegin)
{
	for (size_t i = 0; i < _kana.size();)
	{
		// SJISの全角文字は通常2バイト
		int charLength = 1;

		unsigned char c = static_cast<unsigned char>(_kana[i]);

		// 全角文字の区画ならば
		if (0x81 <= c && c <= 0x9F
			|| 0xE0 <= c && c <= 0xFC)
		{
			charLength = 2;
		}

		// 表示文字1文字分を登録
		mConvertionUnits.emplace_back(
			_romaLength,
			_romaBegin,
			charLength,
			_kanaBegin,
			_kanaCharBegin
		);

		_kanaBegin += charLength;
		_kanaCharBegin++;
		i += charLength;
	}
}