#include "GameCycle.h"

GameCycle::GameCycle()
{
}

GameCycle::~GameCycle()
{
}
void GameCycle::Initialize()
{

	mnInputStringHandle = Master::mpFontManager->GetFontHandle(Master::mpFontManager->FONT_NONE, 48);
	mTextInputManager.Initialize(mnInputStringHandle);
	mTextInputManager.SetKeyInputDrawPosition(Master::gridWidth * 45, Master::gridHeight * 82);
}
void GameCycle::Update()
{
	Master::mpCommenterManager->Update();
	if (!mbQuestionSpawned)
	{
		Master::mpCommenterManager->ChangeQuestion();
		Master::mpQuestionManager->SpawnQuestion(stageNumber, mnQuestionNumber);
		mnNowQuestionImageHandle = Master::mpQuestionManager->GetQuestionData().questionGraphHandle;
		mbQuestionSpawned = true;
	}
	if (!mbQuestionNumberDrawn || !mbQuestionDrawn)
	{
		return;
	}
	else
	{
		Master::mpCommenterManager->QuestionStarted(true);
	}

	InputAnswer();

	if (mbInputAnswered && AnswerJudge())
	{
		mbQuestionSpawned = false;
		mbQuestionNumberDrawn = false;
		mbQuestionDrawn = false;
		mnQuestionNumber++;
		Master::mpCommenterManager->QuestionStarted(false);
		if (mnQuestionNumber > mnMaxQuestionNumber)
		{
			GameEnd();
		}
	}
}


void GameCycle::Draw()
{
	CommentDraw();

	QuestionNumberDraw();

	QuestionDraw();

	if (mbQuestionNumberDrawn &&mbQuestionDrawn)
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
	SetDrawArea(Master::gridWidth * 89, Master::gridHeight * 12, Master::gridWidth * 117, Master::gridHeight * 113);
	Master::mpCommenterManager->Draw();
	SetDrawAreaFull();
}

void GameCycle::QuestionNumberDraw()
{
	mbQuestionNumberDrawn = true;
}

void GameCycle::QuestionDraw()
{
	DrawExtendGraph(mnQuestionImageX1, mnQuestionImageY1, mnQuestionImageX2, mnQuestionImageY2, mnNowQuestionImageHandle, TRUE);
	mbQuestionDrawn = true;
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
		
		mbInputAnswered = true;
	}
	else if (nowState == 2)
	{
		
	}
}

bool GameCycle::AnswerJudge()
{
	mbInputAnswered = false;
	
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
		printfDx("\n　デバッグ：%d/%d 問目正解 GameCycle.cpp 122行辺り", mnQuestionNumber, mnMaxQuestionNumber);
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
	Master::mpCommenterManager->QuestionStarted(false);
}