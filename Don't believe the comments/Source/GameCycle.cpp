#include "GameCycle.h"


GameCycle::GameCycle()
{
}

GameCycle::~GameCycle()
{
}
void GameCycle::Initialize()
{

	inputStringHandle = Master::mpFontManager->GetFontHandle(Master::mpFontManager->FONT_NONE, 48);
	mTextInputManager.Initialize(inputStringHandle);
	mTextInputManager.SetKeyInputDrawPosition(Master::gridWidth * 45, Master::gridHeight * 82);
}
void GameCycle::Update()
{
	if (!QuestionSpawned)
	{
		Master::mpQuestionManager->SpawnQuestion(stageNumber, questionNumber);
		nowQuestionImageHandle = Master::mpQuestionManager->GetQuestionData().questionGraphHandle;
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
		isQuestionNumberDraw = false;
		isQuestionDraw = false;
		questionNumber++;
		if (questionNumber > maxQuestionNumber)
		{
			clsDx();
			printfDx("\n　デバッグ：最後まで行きました GameCycle.cpp 42行辺り");
			GameEnd();
		}
	}
}

void GameCycle::Draw()
{
	CommentDraw();

	QuestionNumberDraw();

	QuestionDraw();

	if (isQuestionNumberDraw &&isQuestionDraw)
	{
		mTextInputManager.Draw();
	}
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
	DrawExtendGraph(QuestionImageX1, QuestionImageY1, QuestionImageX2, QuestionImageY2, nowQuestionImageHandle, TRUE);
	isQuestionDraw = true;
}

void GameCycle::InputAnswer()
{
	//現在の入力状態
	int nowState = mTextInputManager.CheckInput();
	mTextInputManager.ResetState();
	if (nowState == 0)
	{
		mTextInputManager.Update();
	}
	else if (nowState == 1)
	{
		//文字入力
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
		mTextInputManager.ResetInput();
		answer = "";
		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	clsDx();
	if (data.answer == answer)
	{
		printfDx("\n　デバッグ：%d/%d 問目正解 GameCycle.cpp 122行辺り", questionNumber, maxQuestionNumber);
		mTextInputManager.ResetInput();
		answer = "";
		return true;
	}
	else
	{
		printfDx("\n　デバッグ：正解は %s　%s\n", data.answer.c_str(), "GameCycle.cpp 129行辺り");
		mTextInputManager.ResetInput();
		answer = "";
		return false;
	}
}

void GameCycle::GameEnd()
{
	mTextInputManager.Finalize();
}
