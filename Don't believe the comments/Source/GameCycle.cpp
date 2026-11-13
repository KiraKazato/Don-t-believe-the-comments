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
	Master::mpTextInputManager->Initialize(mnInputStringHandle);
	Master::mpTextInputManager->SetKeyInputDrawPosition(Master::gridWidth * 45, Master::gridHeight * 82);
	Master::mpQuestionManager->SetMaxQuestionCount(mnMaxQuestionNumber);
	Master::mpQuestionManager->ResetCorrectAnswer();
	Master::mpQuestionManager->SetIsAllClear(false);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_BEFORE);

	mnCorrectAnswer = new EffekseerEffect("Resource/Effect/CorrectAnswer.efk",300.0f);
	mnFallingCrossMiss = new EffekseerEffect("Resource/Effect/FallingCrossMiss.efk", 95.0f);

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
	Master::mpViewers->Update();

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

	if (mnCorrectAnswer != nullptr)
	{
		mnCorrectAnswer->Update();
	}
	if (mnFallingCrossMiss != nullptr)
	{
		mnFallingCrossMiss->Update();
	}

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
	GameOver();
}


void GameCycle::Draw()
{
	CommentDraw();

	QuestionNumberDraw();

	Master::mpViewers->Draw();
	if (mbQuestionNumberDrawn && mbQuestionDrawn)
	{
		Master::mpTextInputManager->Draw();
	}

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
	Master::mpTextInputManager->Finalize();
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
	if (mfQuestionDrawEasing >= 1.0f)
	{
		mfQuestionDrawEasing = 1.0f;
	}

	// 終わった
	mbQuestionDrawn = true;
}

void GameCycle::InputAnswer()
{
	//現在の入力状態
	int nowState = Master::mpTextInputManager->CheckInput();
	Master::mpTextInputManager->ResetState();
	if (nowState == 0)
	{
		Master::mpTextInputManager->Update();
	}
	else if (nowState == 1)
	{
		//文字入力
		answer = Master::mpTextInputManager->GetInputString();
		
		mbInputAnswered = true;
	}
	else if (nowState == 2)
	{
		Master::mpPause->StartPause();
	}
}

bool GameCycle::AnswerJudge()
{
	mbInputAnswered = false;
	
	if (answer == "")
	{
		Master::mpTextInputManager->ResetInput();
		answer = "";
		Master::mpViewers->LeaveViewers();


		return false;
	}

	auto& data = Master::mpQuestionManager->GetQuestionData();
	if (data.answer == answer)
	{
		Master::mpTextInputManager->ResetInput();
		answer = "";
		Master::mpViewers->ComeViewers();

		mnCorrectAnswer->Play(Master::gridWidth * 45, Master::gridHeight * 50);

		Master::mpQuestionManager->AddCorrectAnswer();
		Master::mpSoundManager->PlaySE(SoundManager::SE_CORRECTED);
		return true;
	}
	else
	{
		Master::mpTextInputManager->ResetInput();
		answer = "";
		Master::mpViewers->LeaveViewers();

		mnFallingCrossMiss->Play(Master::gridWidth * 45, Master::gridHeight * 65);
		mnShakeTime = 20;

		Master::mpSoundManager->PlaySE(SoundManager::SE_FAILED);
		return false;
	}
}

void GameCycle::DrawNextButton()
{
	auto button = Master::mpUIManager->GetUIData(mnNextButtonHandle);
	button->displayed = true;
	int mouseX, mouseY;
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
	bool inside = Master::mpUIManager->InsideUI(static_cast<int>(mnNextButtonX1), static_cast<int>(mnNextButtonY1), static_cast<int>(mnNextButtonX2), static_cast<int>(mnNextButtonY2), mouseX, mouseY);
	if (inside)
	{
		Master::mpUIManager->ChangeUISize(mnNextButtonHandle, 1.1f);
		if (Master::mpUIManager->ClickedUI(mnNextButtonHandle, inside))
		{
			Master::mpTransitor->AddTransitorProcessingUpdate("CycleViewerCount", [&]() {Master::mpViewers->Update(); });
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

	Master::mpTextInputManager->SetNoInput();
	Master::mpTextInputManager->Finalize();

	// 評価を初期化
	Master::mpCommenterManager->ResetCommenterReputation();

	// トランジション中にやりたい処理
	std::function<void()>transitorFunction = [&]() 
		{
			Master::mpViewers->Update(); 
		};

	// ラムダ式でトランジション中にやりたい処理を保存
	Master::mpTransitor->AddTransitorProcessingUpdate("GameOver", transitorFunction);
	// フェードアウト、リザルトシーンへ変更
	Master::mpTransitor->XFadeInit(false, gameOverFadeTime, SceneManager::SCENE_TYPE::SCENE_RESULT);
}

void GameCycle::GameEnd()
{
	mbGameEnd = true;
	Master::mpTextInputManager->Finalize();
	Master::mpQuestionManager->ClearSpawnedQuestion();
	Master::mpQuestionManager->SetIsAllClear(true);
	Master::mpCommenterManager->QuestionStarted(false);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_AFTER);
}