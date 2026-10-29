#include "Commenter.h"

Commenter::Commenter(std::string _name, int _state) :msCommenterName(_name), mnCommenterState(_state)
{
	Master::mpCommenterManager->SetCommenter(this);
}

Commenter::Commenter(std::string _name, int _state, std::string _comment, int _number) :msCommenterName(_name), mnCommenterState(_state), msComment(_comment), mnCommentNumber(_number)
{
	Master::mpCommenterManager->SetDisplayCommenter(this);
	mnNameFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 21);
	mnCommentFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 24);
	msCommenterName=CheckLineBreak(msCommenterName, mnNameFontHandle);
	msComment = CheckLineBreak(msComment, mnCommentFontHandle);
	mGoodButton.handle = Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/GoodButton.png");
	mGoodButton ={
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/GoodButton.png"),
		{112.0f,12.0f + mnNameHeight,114.0f,18.0f},
		false
	};
	mBadButton = {
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/BadButton.png"),
		{115.0f,12.0f + mnNameHeight,117.0f,18.0f },
		false
	};
}

Commenter::~Commenter()
{

}

void Commenter::Draw(int _currentHeight)
{
	int baseGridPositionX{};
	int baseGridPositionY{};
	Master::mpCommenterManager->GetBasePosition(&baseGridPositionX, &baseGridPositionY);
	DrawFormatStringToHandle(Master::gridWidth * baseGridPositionX, Master::gridHeight * (baseGridPositionY + (_currentHeight)), GetColor(0, 0, 0), mnNameFontHandle, "%s", msCommenterName.c_str());
	DrawFormatStringToHandle(Master::gridWidth * baseGridPositionX, Master::gridHeight * (baseGridPositionY + mnNameHeight + (_currentHeight)), GetColor(0, 0, 0), mnCommentFontHandle, "%s", msComment.c_str());
	Master::mpUIManager->SetUIPosition(mGoodButton.handle, Master::gridWidth * mGoodButton.position.baseX1, Master::gridHeight * (mGoodButton.position.baseY1+_currentHeight), Master::gridWidth * mGoodButton.position.baseX2, Master::gridHeight * (mGoodButton.position.baseY2+_currentHeight));
	Master::mpUIManager->SetUIPosition(mBadButton.handle, Master::gridWidth * mBadButton.position.baseX1, Master::gridHeight * (mBadButton.position.baseY1+_currentHeight), Master::gridWidth * mBadButton.position.baseX2, Master::gridHeight * (mBadButton.position.baseY2+_currentHeight));

}

std::string Commenter::CheckLineBreak(std::string _text, int _fontHandle)
{
	std::string resultText{};
	std::string currentText{};
	int startCommentPotision{};
	Master::mpCommenterManager->GetBasePosition(&startCommentPotision, nullptr);
	startCommentPotision = Master::gridWidth * startCommentPotision;
	int endCommentPosition = Master::gridWidth * Master::mpCommenterManager->GetMaxWidthPosition();

	for (int number = 0; number < _text.size();)
	{
		int charBytes = 1;
		unsigned char charText = (unsigned char)_text[number];
		if ((charText >= 0x81 && charText <= 0x9F) || (charText >= 0xE0 && charText <= 0xFC))
		{
			charBytes = 2;
		}
		std::string nextText = _text.substr(number, charBytes);
		int nextWidth = GetDrawStringWidthToHandle((currentText + nextText).c_str(), static_cast<int>((currentText + nextText).length()), _fontHandle);

		if (startCommentPotision + nextWidth > endCommentPosition)
		{
			resultText += currentText + "\n";
			currentText = nextText;
			totalHeightSpace += 3;
		}
		else
		{
			currentText += nextText;
		}
		number += charBytes;
	}
	resultText += currentText;
	return resultText;
}