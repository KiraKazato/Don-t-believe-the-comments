#include "Commenter.h"

Commenter::Commenter(std::string _name, int _state) :msCommenterName(_name), mnCommenterState(_state)
{
	Master::mpCommenterManager->SetCommenter(this);
}

Commenter::Commenter(Commenter* _parent,std::string _name, int _state, std::string _comment, int _number) :mParentCommenter(_parent), msCommenterName(_name), mnCommenterState(_state), msComment(_comment), mnCommentNumber(_number)
{
	Master::mpCommenterManager->SetDisplayCommenter(this);
	mnNameFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 21);
	mnCommentFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 24);
	msCommenterName=CheckLineBreak(msCommenterName, mnNameFontHandle);
	msComment = CheckLineBreak(msComment, mnCommentFontHandle);
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
	DrawFormatStringToHandle(Master::gridWidth * currentGridPositionX, Master::gridHeight * (currentGridPositionY + (mnCurrentCommentHeight)), GetColor(0, 0, 0), mnNameFontHandle, "%s", msCommenterName.c_str());
	DrawFormatStringToHandle(Master::gridWidth * currentGridPositionX, Master::gridHeight * (currentGridPositionY + mnNameHeight + (mnCurrentCommentHeight)), GetColor(0, 0, 0), mnCommentFontHandle, "%s", msComment.c_str());
	DrawReputatoionGraph();
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
		int nextWidth; //GetDrawStringWidthToHandle((currentText + nextText).c_str(), static_cast<int>((currentText + nextText).length()), _fontHandle);
		int nextHeight;
		GetDrawStringSize(&nextWidth, &nextHeight, 0, (currentText + nextText).c_str(), static_cast<int>((currentText + nextText).length()));
		if (startCommentPotision + nextWidth > endCommentPosition)
		{
			resultText += currentText + "\n";
			currentText = nextText;
			totalHeightSpace += nextHeight + 3;
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

void Commenter::DrawReputatoionGraph()
{
	DrawExtendGraphF(Master::gridWidth * mGoodButton.currentPosition.currentX1, Master::gridHeight * mGoodButton.currentPosition.currentY1, Master::gridWidth * mGoodButton.currentPosition.currentX2, Master::gridHeight * mGoodButton.currentPosition.currentY2, mGoodButton.pressed ? mGoodButton.pressedHandle : mGoodButton.handle, TRUE);
	DrawExtendGraphF(Master::gridWidth * mBadButton.currentPosition.currentX1, Master::gridHeight * mBadButton.currentPosition.currentY1, Master::gridWidth * mBadButton.currentPosition.currentX2, Master::gridHeight * mBadButton.currentPosition.currentY2, mBadButton.pressed ? mBadButton.pressedHandle : mBadButton.handle, TRUE);
	DrawFormatStringF(Master::gridWidth * mGoodButton.currentPosition.currentX1, Master::gridHeight*mGoodButton.currentPosition.currentY2, GetColor(0, 255, 0), "%f", mfCommenterReputation);
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
