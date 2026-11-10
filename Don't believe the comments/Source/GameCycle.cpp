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
	Master::mpQuestionManager->SetMaxQuestionCount(mnMaxQuestionNumber);
	Master::mpQuestionManager->ResetCorrectAnswer();
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_BEFORE);
	Master::mpCommenterManager->ResetCommentCount();
	mnNextButtonHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/NextButton.png");
	Master::mpUIManager->AddUI(mnNextButtonHandle,mnNextButtonX1, mnNextButtonY1, mnNextButtonX2, mnNextButtonY2, false);

	if (Master::mpViewers->GetCurrentViewers() <= 0)
	{
		Master::mpViewers->Initialize();
	}
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
	GameOver();
}


void GameCycle::Draw()
{
	CommentDraw();

	QuestionNumberDraw();

	if (mbGameEnd)
	{
		DrawNextButton();
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

	DrawFormatStringToHandle(Master::gridWidth * 45 - numberWidth / 2,Master::gridHeight * 15 - numberHeight / 2, GetColor(0, 0, 0), mnQuestionNumberHandle, "Q: %d / %d", mnQuestionNumber, mnMaxQuestionNumber);

	mbQuestionNumberDrawn = true;
}

void GameCycle::QuestionDraw()
{
	// イージング関数
	constexpr auto easing = [](float t) {return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f; };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * easing(mfQuestionDrawEasing)));
	DrawExtendGraph(mnQuestionImageX1, mnQuestionImageY1, mnQuestionImageX2, mnQuestionImageY2, mnNowQuestionImageHandle, TRUE);
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
		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	if (data.answer == answer)
	{
		mTextInputManager.ResetInput();
		answer = "";
		Master::mpViewers->ComeViewers();
		Master::mpQuestionManager->AddCorrectAnswer();
		return true;
	}
	else
	{
		mTextInputManager.ResetInput();
		answer = "";
		Master::mpViewers->LeaveViewers();
		return false;
	}
}

void GameCycle::DrawNextButton()
{
	auto button=Master::mpUIManager->GetUIData(mnNextButtonHandle);
	button->displayed = true;
	int mouseX, mouseY;
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
	bool inside = Master::mpUIManager->InsideUI(mnNextButtonX1, mnNextButtonY1, mnNextButtonX2, mnNextButtonY2, mouseX, mouseY);
	if(inside)
	{
		Master::mpUIManager->ChangeUISize(mnNextButtonHandle, 1.1f);
		if (Master::mpUIManager->ClickedUI(mnNextButtonHandle, inside))
		{
			Master::mpTransitor->XFadeInit(SceneManager::SCENE_RESULT);
			button->displayed = false;
		}
	}
	else
	{
		Master::mpUIManager->ResetUISize(mnNextButtonHandle);
	}
}

void GameCycle::GameOver()
{
	constexpr float gameOverFadeTime = 4.0f;
	if (Master::mpViewers->GetTargetViewers() > 0)
	{
		return;
	}

	mTextInputManager.SetNoInput();
	mTextInputManager.Finalize();

	// 評価を初期化
	Master::mpCommenterManager->ResetCommenterReputation();

	// ラムダ式でトランジション中にやりたい処理を保存
	Master::mpTransitor->AddTransitorUpdate([&]() {Master::mpViewers->Update(); });
	// フェードアウト、リザルトシーンへ変更
	Master::mpTransitor->XFadeInit(false, gameOverFadeTime, SceneManager::SCENE_TYPE::SCENE_RESULT);
}

void GameCycle::GameEnd()
{
	mbGameEnd = true;
	mTextInputManager.Finalize();
	Master::mpCommenterManager->QuestionStarted(false);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_AFTER);
}