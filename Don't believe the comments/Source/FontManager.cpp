#include "FontManager.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}


void FontManager::Initialize()
{
	//ここで使いたいフォントを追加
	LoadInputFont(FONT_TETUBINN, "07鉄瓶ゴシック", "Resource/TetubinGosikku.ttf");
}

void FontManager::LoadInputFont(Font tag, const char* _fontName, LPCSTR _fileName)
{
	AddFontResourceExA(_fileName, FR_PRIVATE, NULL);

	fontList.push_back({ tag, _fontName ,_fileName });
}


void FontManager::LoadSystemFont(Font tag, const char* _fontName)
{
	fontList.push_back({ tag,_fontName,NULL });
}



int FontManager::GetFontHandle(Font tag,int _size)
{
	if (!fontHandleList.empty())
	{
		for (const auto& Handle : fontHandleList)
		{
			if (std::get<0>(Handle) == tag && std::get<1>(Handle) == _size)
			{
				return std::get<2>(Handle);
			}
		}
	}
	for (const auto& font : fontList)
	{
		if (std::get<0>(font) == tag)
		{
			int fontHandle =CreateFontToHandle(std::get<1>(font), _size, 0, DX_FONTTYPE_ANTIALIASING);
			fontHandleList.push_back({ tag,_size,fontHandle });
			return fontHandle;
		}
	}
	return -1;
}

void FontManager::Finalize()
{
	for (const auto& Handle : fontHandleList)
	{
		// フォントハンドルを削除
		DeleteFontToHandle(std::get<2>(Handle));
	}
	for (const auto& tuple : fontList)
	{	
		if (std::get<2>(tuple) != NULL)
		{
			// ウィンドウズに一時的に保持していたフォントデータを削除
			RemoveFontResourceExA(std::get<2>(tuple), FR_PRIVATE, NULL);
		}
	}
	fontList.clear();
}