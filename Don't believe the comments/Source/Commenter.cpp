#include "Commenter.h"

Commenter::Commenter(std::string _name, int _state) :msCommenterName(_name), mnCommenterState(_state)
{
	Master::mpCommenterManager->SetCommenter(this);
}

Commenter::Commenter(std::string _name, int _state, std::string _comment, int _number) :msCommenterName(_name), mnCommenterState(_state), msComment(_comment), mnCommentNumber(_number)
{
	Master::mpCommenterManager->SetDisplayCommenter(this);
	mnCommentFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 20);
}

Commenter::~Commenter()
{

}

void Commenter::Draw()
{
	int baseGridPositionX{};
	int baseGridPositionY{};
	Master::mpCommenterManager->GetBasePosition(&baseGridPositionX, &baseGridPositionY);
	DrawFormatStringToHandle(Master::gridWidth * baseGridPositionX, Master::gridHeight * (baseGridPositionY + (5 * mnCommentNumber)), GetColor(0, 0, 0), mnCommentFontHandle, "%s", msCommenterName.c_str());
}