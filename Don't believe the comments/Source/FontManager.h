#pragma once
#include "DxLib.h"
#include <string>
#include <vector>

class FontManager
{
public:
	//フォントの識別タグ
	enum Font
	{
		FONT_TETUBINN = 0
	};
public:
	//コンストラクタ
	FontManager();
	//デストラクタ
	~FontManager();
	void Initialize();//イニシャライズ
	void LoadInputFont(Font tag, const char* _fontName,LPCSTR _fileName, int _size);//外部ファイルからフォント読み込み
	void LoadSystemFont(Font tag, const char* _fontName,int _size);//Windowsにもともとあるフォント読み込み
	int GetFontHandle(Font tag);//指定フォントのハンドル取得
	void Finalize();//ファイナライズ
private:
	//このクラスで必要なフォント情報のリスト
	std::vector<std::tuple<Font,int,LPCSTR>>fontList;
};