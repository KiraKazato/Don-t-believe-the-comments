#include "Commenter.h"

Commenter::Commenter(std::string _name, int _state) :msCommenterName(_name), mnCommenterState(_state)
{
	Master::mpCommenterManager->SetCommenter(this);
}

Commenter::Commenter(Commenter* _parent, std::string _name, int _state, std::string _comment, int _number, bool _isSuperChat) :mParentCommenter(_parent), msCommenterName(_name), mnCommenterState(_state), msComment(_comment), mnCommentNumber(_number), mbIsSuperChat(_isSuperChat)
{
	Master::mpCommenterManager->SetDisplayCommenter(this);
	mnNameFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 21);
	mnCommentFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 24);
	mnSuperChatCommentFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 25);
	mnSuperChatAmountFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 23);

	msCommenterName = CheckLineBreak(msCommenterName, mnNameFontHandle);
	if (!mbIsSuperChat)
	{	
		msComment = CheckLineBreak(msComment, mnCommentFontHandle);
	}
	else
	{
		msComment = CheckLineBreak(msComment, mnSuperChatCommentFontHandle);
	}
	mfCommenterReputation = mParentCommenter->GetReputation();
	mGoodButton.handle = Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/GoodButton.png");
	mGoodButton = {
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/GoodButton.png"),
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/PressedGoodButton.png"),
		{112.0f,11.8f + mnNameHeight,113.5f,17.8f},
		{112.0f,11.8f + mnNameHeight,113.5f,17.8f},
		false,
		false
	};
	mBadButton = {
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/BadButton.png"),
		Master::mpResourceManager->LoadGraphics("Resource/NeedCommenter/PressedBadButton.png"),
		{115.0f,12.2f + mnNameHeight,116.5f,18.2f },
		{115.0f,12.2f + mnNameHeight,116.5f,18.2f },
		false,
		false
	};
}

Commenter::~Commenter()
{

}

void Commenter::Draw()
{
	int currentGridPositionX{};
	int currentGridPositionY{};
	Master::mpCommenterManager->GetCurrentPosition(&currentGridPositionX, &currentGridPositionY);
	int drawX = Master::gridWidth * currentGridPositionX;
	int nameDrawY = Master::gridHeight * (currentGridPositionY + (mnCurrentCommentHeight));
	int commentDrawY = Master::gridHeight * (currentGridPositionY + mnNameHeight + (mnCurrentCommentHeight));
	unsigned int color = GetColor(0, 0, 0);
	
	int commentFont = mnCommentFontHandle;
	if (mbIsSuperChat)
	{
		drawX += Master::gridWidth / 2;
		nameDrawY += Master::gridHeight * 2 - Master::gridHeight / 3;
		commentDrawY += Master::gridHeight * 2 - Master::gridHeight / 3;
		commentFont = mnSuperChatCommentFontHandle;

		color = GetColor(255, 255, 255);
	}
	
	SuperChatFrameDraw();

	DrawFormatStringToHandle(drawX, nameDrawY, color, mnNameFontHandle, "%s", msCommenterName.c_str());
	DrawFormatStringToHandle(drawX, commentDrawY, color, commentFont, "%s", msComment.c_str());
	DrawReputatoionGraph();
}

void Commenter::SuperChatFrameDraw()
{
	if (!mbIsSuperChat)
	{
		return;
	}

	int currentGridPositionX{};
	int currentGridPositionY{};
	Master::mpCommenterManager->GetCurrentPosition(&currentGridPositionX, &currentGridPositionY);
	int drawX1 = Master::gridWidth * currentGridPositionX - Master::gridWidth / 2;
	int drawY1 = Master::gridHeight * (currentGridPositionY + (mnCurrentCommentHeight)) + Master::gridHeight - (Master::gridHeight / 2);
	int drawX2 = Master::gridWidth * 117;
	int drawY2 = drawY1 + (Master::gridHeight * (totalHeightSpace - 4)) + (Master::gridHeight / 2);

	DrawBox(drawX1, drawY1, drawX2, drawY2 + Master::gridHeight, GetColor(178, 70, 70), TRUE);
	DrawBox(drawX1, drawY1, drawX2, drawY2, GetColor(255, 100, 100), TRUE);
	DrawLine(drawX2, drawY2, drawX2, drawY2 + Master::gridHeight, GetColor(191, 75, 75));

	std::string amountStr = AmountSeparate(10000);

	int width, height;
	GetDrawFormatStringSizeToHandle(&width, &height, NULL, mnSuperChatAmountFontHandle, "%s", amountStr.c_str());

	DrawFormatStringToHandle(drawX2 - width - Master::gridWidth, drawY2 - height, GetColor(255, 255, 255), mnSuperChatAmountFontHandle, "%s", amountStr.c_str());
}

void Commenter::SetReputation(float  _reputation)
{
	mfCommenterReputation = _reputation;
}

std::string Commenter::CheckLineBreak(std::string _text, int _fontHandle)
{
	std::string resultText{};
	std::string currentText{};
	int startCommentPotision{};
	Master::mpCommenterManager->GetBasePosition(&startCommentPotision, nullptr);
	startCommentPotision = Master::gridWidth * startCommentPotision;
	int endCommentPosition{};
	if (mbIsSuperChat)
	{
		endCommentPosition = Master::gridWidth * 117;
	}
	else
	{
		endCommentPosition = Master::gridWidth * Master::mpCommenterManager->GetMaxWidthPosition() - Master::gridWidth / 2;
	}

	for (int number = 0; number < _text.size();)
	{
		int charBytes = 1;
		unsigned char charText = (unsigned char)_text[number];
		if ((charText >= 0x81 && charText <= 0x9F) || (charText >= 0xE0 && charText <= 0xFC))
		{
			charBytes = 2;
		}
		std::string nextText = _text.substr(number, charBytes);
		int nextWidth;
		GetDrawStringSizeToHandle(&nextWidth, NULL, NULL, (currentText + nextText).c_str(), static_cast<int>((currentText + nextText).length()), _fontHandle);
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

	if (mbIsSuperChat)
	{
		totalHeightSpace += 3;
	}

	resultText += currentText;
	return resultText;
}

void Commenter::DrawReputatoionGraph()
{
	if (mbIsSuperChat)
	{
		return;
	}

	DrawExtendGraphF(Master::gridWidth * mGoodButton.currentPosition.currentX1, Master::gridHeight * mGoodButton.currentPosition.currentY1, Master::gridWidth * mGoodButton.currentPosition.currentX2, Master::gridHeight * mGoodButton.currentPosition.currentY2, mGoodButton.pressed ? mGoodButton.pressedHandle : mGoodButton.handle, TRUE);
	DrawExtendGraphF(Master::gridWidth * mBadButton.currentPosition.currentX1, Master::gridHeight * mBadButton.currentPosition.currentY1, Master::gridWidth * mBadButton.currentPosition.currentX2, Master::gridHeight * mBadButton.currentPosition.currentY2, mBadButton.pressed ? mBadButton.pressedHandle : mBadButton.handle, TRUE);
}

void Commenter::updateReputation(int _currentHeight)
{
	mnCurrentCommentHeight = _currentHeight;

	ChangeReputationButtonPosition();
	int mouseX, mouseY;
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
	bool goodInside, badInside, goodClicked, badClicked;
	goodInside = Master::mpUIManager->InsideUI((int)(Master::gridWidth * mGoodButton.currentPosition.currentX1), (int)(Master::gridHeight * mGoodButton.currentPosition.currentY1), (int)(Master::gridWidth * mGoodButton.currentPosition.currentX2), (int)(Master::gridHeight * mGoodButton.currentPosition.currentY2), mouseX, mouseY);
	badInside = Master::mpUIManager->InsideUI((int)(Master::gridWidth * mBadButton.currentPosition.currentX1), (int)(Master::gridHeight * mBadButton.currentPosition.currentY1), (int)(Master::gridWidth * mBadButton.currentPosition.currentX2), (int)(Master::gridHeight * mBadButton.currentPosition.currentY2), mouseX, mouseY);
	goodClicked = Master::mpUIManager->ClickedUI(&mGoodButton.pressing, goodInside);
	badClicked = Master::mpUIManager->ClickedUI(&mBadButton.pressing, badInside);
	if (goodInside)
	{
		int a = 0;
	}

	float reputation = mParentCommenter->GetReputation();
	if(goodClicked)
	{
		if (mBadButton.pressed)
		{
			mBadButton.pressed = false;
			reputation += 0.1f;
		}
		if (!mGoodButton.pressed)
		{
			mGoodButton.pressed = true;
			if (reputation <= mfMaxReputation)
			{
				reputation += 0.1f;
			}
			if (reputation >= mfMaxReputation)
			{
				reputation = mfMaxReputation;
			}
		}
		else
		{
			mGoodButton.pressed = false;
			
			if (reputation >= mfMinReputation && reputation < mfMaxReputation)
			{
				reputation -= 0.1f;
			}
			if (reputation <= mfMinReputation)
			{
				reputation = mfMinReputation;
			}
		}
		
	}
	else if (badClicked)
	{
		if (mGoodButton.pressed)
		{
			mGoodButton.pressed = false;
			reputation -= 0.1f;
		}
		if (!mBadButton.pressed)
		{
			mBadButton.pressed = true;
			if (reputation >= mfMinReputation)
			{
				reputation -= 0.1f;
			}
			if (reputation <= mfMinReputation)
			{
				reputation = mfMinReputation;
			}
		} 
		else
		{
			mBadButton.pressed = false;
			if (reputation <= mfMaxReputation && reputation > mfMinReputation)
			{
				reputation += 0.1f;
			}
			if (reputation >= mfMaxReputation)
			{
				reputation = mfMaxReputation;
			}
		}
		
	}
	mParentCommenter->SetReputation(reputation);
	SetReputation(mParentCommenter->GetReputation());
}

void Commenter::ChangeReputationButtonPosition()
{
	int baseGridPositionY{};
	int currentGridPositionY{};
	Master::mpCommenterManager->GetBasePosition(0, &baseGridPositionY);
	Master::mpCommenterManager->GetCurrentPosition(0, &currentGridPositionY);
	mGoodButton.currentPosition.currentY1 = currentGridPositionY - (baseGridPositionY - mGoodButton.basePosition.baseY1)+mnCurrentCommentHeight;
	mGoodButton.currentPosition.currentY2 = currentGridPositionY - (baseGridPositionY - mGoodButton.basePosition.baseY2)+mnCurrentCommentHeight;
	mBadButton.currentPosition.currentY1 = currentGridPositionY - (baseGridPositionY - mBadButton.basePosition.baseY1)+mnCurrentCommentHeight;
	mBadButton.currentPosition.currentY2 = currentGridPositionY - (baseGridPositionY - mBadButton.basePosition.baseY2)+mnCurrentCommentHeight;

}

std::string Commenter::AmountSeparate(int _amount)
{
	std::string ret = "";
	std::string amountStr = std::to_string(_amount);

	ret = amountStr;
	for (int i = static_cast<int>(amountStr.size()) - 3; i > 0; i -= 3) 
	{
		ret.insert(static_cast<size_t>(i), ",");
	}

	return ret;
}
