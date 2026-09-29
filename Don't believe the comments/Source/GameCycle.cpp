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
	mTextInputManager.SetKeyInputDrawPosition(Master::Width / 2, Master::Height / 2);
}
void GameCycle::Update()
{
	if (!QuestionSpawned)
	{
		Master::mpQuestionManager->SpawnQuestion(stageNumber, questionNumber);
		QuestionSpawned = true;
	}
	if (!isQuestionNumberDraw || !isQuestionDraw)
	{
		return;
	}

	InputAnswer();

	if (isInputAnswer && AnswerJudge())
	{
		QuestionSpawned = false;
		questionNumber++;
		if (questionNumber > maxQuestionNumber)
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
	mTextInputManager.Draw();
}


void GameCycle::Finalize()
{
	mTextInputManager.Finalize();
}
void GameCycle::SetStage(StageNumber _stageNumber)
{
	stageNumber = _stageNumber;
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
		mTextInputManager.StateInit();
		answer = "";
		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	if (data.answer == answer)
	{
		mTextInputManager.StateInit();
		answer = "";
		return true;
	}
	else
	{
		mTextInputManager.StateInit();
		answer = "";
		return false;
	}
}

void GameCycle::GameEnd()
{
}
