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
	mTextInputManager.Initialize();
}
void GameCycle::Update()
{
	
	if (!isQuestionNumberDraw||!isQuestionDraw)
	{
		return;
	}

	InputAnswer();

	if (isInputAnswer&& AnswerJudge())
	{
		questionNumber++;
		if (questionNumber > 10)
		{
			GameEnd();
		}
	}
}

void GameCycle::Draw()
{
	CommentDraw();
	if (!isQuestionNumberDraw)
	{
		QuestionNumberDraw();
		return;
	}
	if (!isQuestionDraw)
	{
		QuestionDraw();
		return;
	}
}


void GameCycle::Finalize()
{
	mTextInputManager.Finalize();
}
void GameCycle::CommentDraw()
{
}

void GameCycle::QuestionNumberDraw()
{
	isQuestionNumberDraw = true;

}

void GameCycle::QuestionDraw()
{
	isQuestionDraw = true;
	
}

void GameCycle::InputAnswer()
{
	//Œ»Ý‚Ì“ü—Íó‘Ô
	int nowState = mTextInputManager.CheckInput();

	if (nowState == 0)
	{
		mTextInputManager.Update();
	}
	else if (nowState == 1)
	{
		//•¶Žš“ü—Í
		answer = mTextInputManager.GetInputString();
		isInputAnswer = true;
	}
	else if (nowState == 2)
	{
	}
}

bool GameCycle::AnswerJudge()
{
	isInputAnswer = false;

	if (answer == "")
	{
		answer = "";
		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	if (data.answer == answer)
	{
		answer = "";
		return true;
	}
	else
	{
		answer = "";
		return false;
	}
}

void GameCycle::GameEnd()
{
}
