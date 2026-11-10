#include "GameCycle.h"

GameCycle::GameCycle()
	:mnCorrectAnswer(0)
	,mnFallingCrossMiss(0)
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

	mnCorrectAnswer = new EffekseerEffect("Resource/Effect/CorrectAnswer.efk",300.0f);
	mnFallingCrossMiss = new EffekseerEffect("Resource/Effect/FallingCrossMiss.efk", 95.0f);

}
void GameCycle::Update()
{
	Master::mpCommenterManager->Update();

	//画面シェイクの更新処理
	if (mnShakeTime > 0)
	{
		mnShakeTime--; // 残り時間を1減らす

		// 揺れの強さ
		int shakePower = 10;

		//ランダムなズレにする
		mnShakeX = GetRand(shakePower * 2) - shakePower;
		mnShakeY = GetRand(shakePower * 2) - shakePower;
	}
	else
	{
		// 揺れ終わったらズレを 0に戻す
		mnShakeX = 0;
		mnShakeY = 0;
	}

	mTextInputManager.SetKeyInputDrawPosition(Master::gridWidth * 45 + mnShakeX,Master::gridHeight * 82 + mnShakeY);


	if (mbGameEnd)
	{
		return;
	}

	if (mnCorrectAnswer != nullptr)
	{
		mnCorrectAnswer->Update();
	}
	if (mnFallingCrossMiss != nullptr)
	{
		mnFallingCrossMiss->Update();
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
	Master::mpViewers->Update();
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

	Master::mpViewers->Draw();
	if (mbQuestionNumberDrawn &&mbQuestionDrawn)
	{
		mTextInputManager.Draw();
	}



}


void GameCycle::Finalize()
{
	mTextInputManager.Finalize();
	Master::mpCommenterManager->GameEnd();

}

void GameCycle::CommentDraw()
{
	SetDrawArea(Master::gridWidth * 89, Master::gridHeight * 12, Master::gridWidth * 117, Master::gridHeight * 113);
	Master::mpCommenterManager->Draw();
	SetDrawAreaFull();
}

void GameCycle::QuestionNumberDraw()
{
	// 最初はfalseにする
	int numberWidth,numberHeight;
	GetDrawFormatStringSizeToHandle(&numberWidth, &numberHeight, NULL, mnQuestionNumberHandle, "Q: %d / %d", mnQuestionNumber, mnMaxQuestionNumber);

	DrawFormatStringToHandle(
		Master::gridWidth * 45 - numberWidth / 2 + mnShakeX,
		Master::gridHeight * 15 - numberHeight / 2 + mnShakeY,
		GetColor(0, 0, 0),
		mnQuestionNumberHandle,
		"Q: %d / %d",
		mnQuestionNumber,
		mnMaxQuestionNumber
	);
	mbQuestionNumberDrawn = true;
}

void GameCycle::QuestionDraw()
{
	// イージング関数
	constexpr auto easing = [](float t) {return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f; };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * easing(mfQuestionDrawEasing)));
	DrawExtendGraph(
		mnQuestionImageX1 + mnShakeX,
		mnQuestionImageY1 + mnShakeY,
		mnQuestionImageX2 + mnShakeX,
		mnQuestionImageY2 + mnShakeY,
		mnNowQuestionImageHandle,
		TRUE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 1,0f / (60.0f * x) -> x 秒かけての表示
	constexpr float easingCount = 1.0f / (60.0f * 1.0f);
	mfQuestionDrawEasing += easingCount;

	// 終わった
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
		Master::mpViewers->LeaveViewers();


		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	if (data.answer == answer)
	{
		mTextInputManager.ResetInput();
		answer = "";
		Master::mpViewers->ComeViewers();

		mnCorrectAnswer->Play(Master::gridWidth * 45, Master::gridHeight * 50);

		return true;
	}
	else
	{
		mTextInputManager.ResetInput();
		answer = "";
		Master::mpViewers->LeaveViewers();

		mnFallingCrossMiss->Play(Master::gridWidth * 45, Master::gridHeight * 60);
		mnShakeTime = 20;

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