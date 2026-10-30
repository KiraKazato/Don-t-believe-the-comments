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
	mnQuestionNumberHandle = Master::mpFontManager->GetFontHandle(Master::mpFontManager->FONT_TETUBINN, 26);
	mTextInputManager.Initialize(mnInputStringHandle);
	mTextInputManager.SetKeyInputDrawPosition(Master::gridWidth * 45, Master::gridHeight * 82);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_BEFORE);
}
void GameCycle::Update()
{
	Master::mpCommenterManager->Update();

	if (mbGameEnd)
	{
		return;
	}

	if (!mbQuestionSpawned)
	{
		Master::mpCommenterManager->ChangeQuestion();
		Master::mpQuestionManager->SpawnQuestion(mnQuestionNumber);
		mnNowQuestionImageHandle = Master::mpQuestionManager->GetQuestionData().questionGraphHandle;
		mbQuestionSpawned = true;
		switch (mnQuestionNumber)
		{
		case 1:
			Master::mpCommenterManager->SetCommentSpawnKind(STATE_BEGINNING);
			break;
		case 2:
			Master::mpCommenterManager->SetCommentSpawnKind(STATE_MID);
			break;
		case 7:
			Master::mpCommenterManager->SetCommentSpawnKind(STATE_FINAL);
			break;
		default:
			break;
		}
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
		mfQuestionDrawEasing = 0.0f;
		Master::mpCommenterManager->QuestionStarted(false);
		if (mnQuestionNumber >= mnMaxQuestionNumber)
		{
			GameEnd();
		}
		else
		{
			mnQuestionNumber++;
		}
	}
}


void GameCycle::Draw()
{
	CommentDraw();

	QuestionNumberDraw();

	if (mbGameEnd)
	{
		return;
	}

	QuestionDraw();

	if (mbQuestionNumberDrawn && mbQuestionDrawn)
	{
		mTextInputManager.Draw();
	}

}


void GameCycle::Finalize()
{
	mTextInputManager.Finalize();
}

void GameCycle::CommentDraw()
{
	SetDrawArea(Master::gridWidth * 89, Master::gridHeight * 12, Master::gridWidth * 117, Master::gridHeight * 113);
	Master::mpCommenterManager->Draw();
	SetDrawAreaFull();
}

void GameCycle::QuestionNumberDraw()
{
	// ç≈èâÇÕfalseÇ…Ç∑ÇÈ
	int numberWidth,numberHeight;
	GetDrawFormatStringSizeToHandle(&numberWidth, &numberHeight, NULL, mnQuestionNumberHandle, "Q: %d / %d", mnQuestionNumber, mnMaxQuestionNumber);

	DrawFormatStringToHandle(Master::gridWidth * 45 - numberWidth / 2,Master::gridHeight * 15 - numberHeight / 2, GetColor(0, 0, 0), mnQuestionNumberHandle, "Q: %d / %d", mnQuestionNumber, mnMaxQuestionNumber);

	mbQuestionNumberDrawn = true;
}

void GameCycle::QuestionDraw()
{
	auto easing = [](float t) {return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f; };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * easing(mfQuestionDrawEasing)));
	DrawExtendGraph(mnQuestionImageX1, mnQuestionImageY1, mnQuestionImageX2, mnQuestionImageY2, mnNowQuestionImageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 2ïbÇ©ÇØÇƒï\é¶
	constexpr float easingCount = 1.0f / (60.0f * 2.0f);
	mfQuestionDrawEasing += easingCount;	

	// èIÇÌÇ¡ÇΩ
	mbQuestionDrawn = true;
}

void GameCycle::InputAnswer()
{
	//åªç›ÇÃì¸óÕèÛë‘
	int nowState = mTextInputManager.CheckInput();
	mTextInputManager.ResetState();
	if (nowState == 0)
	{
		mTextInputManager.Update();
	}
	else if (nowState == 1)
	{
		//ï∂éöì¸óÕ
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
	if (data.answer == answer)
	{
		mTextInputManager.ResetInput();
		answer = "";
		return true;
	}
	else
	{
		mTextInputManager.ResetInput();
		answer = "";
		return false;
	}
}

void GameCycle::GameEnd()
{
	mbGameEnd = true;
	mTextInputManager.Finalize();
	Master::mpCommenterManager->QuestionStarted(false);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_AFTER);
}