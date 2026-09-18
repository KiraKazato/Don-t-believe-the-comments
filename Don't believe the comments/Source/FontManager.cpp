#include "FontManager.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}

void FontManager::Initialize()
{
	LoadInputFont(FONT_TETUBINN, "07鉄瓶ゴシック", "Resource/TetubinGosikku.ttf", 80);
}

void FontManager::LoadInputFont(Font tag, const char* _fontName, LPCSTR _fileName, int _size)
{
	AddFontResourceExA(_fileName, FR_PRIVATE, NULL);

	int fontHandle = CreateFontToHandle(_fontName, _size, 0, DX_FONTTYPE_ANTIALIASING);

	fontList.push_back({ tag, fontHandle ,_fileName });
}

void FontManager::LoadSystemFont(Font tag, const char* _fontName, int _size)
{
	int fontHandle = CreateFontToHandle(_fontName, _size, 0, DX_FONTTYPE_ANTIALIASING);

	fontList.push_back({ tag, fontHandle ,NULL });
}



int FontManager::GetFontHandle(Font tag)
{
	for (const auto& tuple : fontList)
	{
		if (std::get<0>(tuple) == tag)
		{
			return std::get<1>(tuple);
		}
	}
	return -1;
}

void FontManager::Finalize()
{
	for (const auto& tuple : fontList)
	{
		// フォントハンドルを削除
		DeleteFontToHandle(std::get<1>(tuple));
		if (std::get<2>(tuple) != NULL)
		{
			// ウィンドウズに一時的に保持していたフォントデータを削除
			RemoveFontResourceExA(std::get<2>(tuple), FR_PRIVATE, NULL);
		}
	}
	fontList.clear();
}