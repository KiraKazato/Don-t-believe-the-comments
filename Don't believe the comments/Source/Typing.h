#pragma once
#include <unordered_map>
#include <string>

struct ConversionUnit
{
	int romaLength = 0;		// ローマ字の長さ（バイト）
	int romaBegin = 0;		// ローマ字の開始位置（バイト）

	int kanaLength = 0;		// かなの長さ（バイト）
	int kanaBegin = 0;		// かなの開始位置（バイト）

	int kanaCharBegin = 0;  // 変換単位の開始位置（表示単位）

	// @return ローマ字（バイト）終端
	int GetRomaEnd() const { return this->romaBegin + this->romaLength; }
	
	// @return かな文字（バイト）終端
	int GetKanaEnd() const { return this->kanaBegin + this->kanaLength; }
	
	// @return かな文字（文字）終端
	int GetKanaCharEnd() const { return this->kanaCharBegin + 1; }

	ConversionUnit(int romaLength, int romaBegin, int kanaLength, int kanaBegin, int kanaCharBegin)
		: romaLength(romaLength), romaBegin(romaBegin), kanaLength(kanaLength), kanaBegin(kanaBegin), kanaCharBegin(kanaCharBegin)
	{
	}ConversionUnit() = default;

	bool operator==(const ConversionUnit& other) const
	{
		return romaLength == other.romaLength
			&& romaBegin == other.romaBegin
			&& kanaLength == other.kanaLength
			&& kanaBegin == other.kanaBegin
			&& kanaCharBegin == other.kanaCharBegin;
	}

	bool operator!=(const ConversionUnit& other) const
	{
		return !(*this == other);
	}
};

class Typing
{
public:
	// 変換辞書
	static const std::unordered_map<std::string_view, std::string_view> KANA_DICTIONARY;
	// 逆引き変換辞書
	static const std::unordered_map<std::string_view, std::string_view> ROMA_DICTIONARY;

public:
	//Typing();
	//~Typing();

	// アルファベット -> かな の変換
	// @return かな に変換した文字列
	const std::string KanaDictionary(const std::string& _roma, bool _isNormalize = false);

	// かな -> アルファベット の変換
	// @return アルファベット に変換した文字列
	// @return 実質的な正規化
	const std::string RomaDictionary(const std::string& _kana);

	// 派生形の入力が多いため、一意にする
	// 拗音は分割し、「ん」は「nn」にし、「っ」はxtuに変換
	// 正規化に近い
	const std::string KanaNormalize(const std::string& _kana);

	// 基本的な文字変換
	bool CharactorConvertion(char _charactor, std::string& _str, std::vector<char>& _roma, bool _isNormalize = false);

	// 「ん」変換
	// @return 「ん」へ変換すべきか
	bool NConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index, bool _isNormalize);

	// 「っ」変換
	// @return 「っ」へ変換すべきか
	bool ConstantConvertion(char _charactor, char _nextCharactor, std::string& _str, std::vector<char>& _roma, int& _index);

	// ローマ字の中に入った全角アルファベットを消す
	void ConvertionFullToHalf(std::string& _roma);

	// 半角アルファベットを全角文字に変換
	std::string AlphabetHalfToFull(char _charactor);

	// 全角アルファベットを半角文字に変換
	char AlphabetFullToHalf(const std::string& _fullCharactor);

	// @return あいうえお（母音）かどうか
	bool IsRomaVowel(char _charactor);

	// @return a-zA-Zかどうか
	bool IsAlphabet(char _charactor);

	// @return ttyaやkkyaなどで「っ」を出すか
	bool IsRomaConsonant(char _charactor, char _nextCharactor);

	// @return 「ん」 を入力したか
	bool IsN(char _charactor, char _nextCharactor);

	// @return それが全角の頭のバイトか
	bool IsCharFullByte(unsigned char _charactorByte);

	// 文字削除
	void DeleteChar();

	// カーソル位置左移動
	void ShiftLeftCursor();

	// カーソル位置右移動
	void ShiftRightCursor();

	// 変換確認とその情報の追加
	void AddConvertion(int _romaLength, int _kanaLength, int _addRomaBegin, int _addKanaBegin, int _addKanaCharLength);

	// 変換単位変換した
	const std::vector<ConversionUnit>& GetConvertionUnits();

	int GetCurrentRomaByte()const;	// ローマ字のバイトカウント取得
	int GetCurrentKanaByte()const;	// かな文字のバイトカウント取得
	int GetCurrentDraw()	const;	// 描画の文字単位カウント取得

	const std::string& GetRoma() const;	// ローマ字取得
	const std::string& GetKana() const;	// かな文字取得

	// 変換した文字列の確認
private:
	std::vector<ConversionUnit> mConvertionUnits{};
	ConversionUnit mInConvertion{};

	// 文字列の情報カウント
private:
	int mnCurrentRomaByte = 0;
	int mnCurrentKanaByte = 0;
	int mnCurrentDraw = 0;

	// 入力文字
private:
	std::string mRoma{};
	std::string mKana{};
};