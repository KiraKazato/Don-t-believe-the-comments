#include "GameCycle.h"
#include "Master.h"
GameCycle::GameCycle()
{
}

GameCycle::~GameCycle()
{
}
void GameCycle::Initialize()
{
	questionNumberStringHandle = Master::mpFontManager->GetFontHandle(Master::mpFontManager->FONT_TETUBINN, 90);
}
void GameCycle::Update()
{
	CommentDraw();
	if (!QuestionNumberDraw())
	{
		return;
	}

	if (!QuestinDraw())
	{
		return;
	}

	InputAnswer();
}
void GameCycle::Finalize()
{

}
void GameCycle::CommentDraw()
{
}

bool GameCycle::QuestionNumberDraw()
{

	return true;
}

bool GameCycle::QuestinDraw()
{
	return true;
}

void GameCycle::InputAnswer()
{
}

bool GameCycle::AnswerJudge()
{
	return true;
}
