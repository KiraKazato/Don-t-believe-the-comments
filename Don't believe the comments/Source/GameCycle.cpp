#include "GameCycle.h"

GameCycle::GameCycle()
	:mnCorrectAnswer(0)
	,mnFallingCrossMiss(0)
	, mn10CorrectAnswersEffect(nullptr) 
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
	Master::mpTextInputManager->SetIsAlphabet(false);
	Master::mpQuestionManager->SetMaxQuestionCount(mnMaxQuestionNumber);
	Master::mpQuestionManager->ResetCorrectAnswer();
	Master::mpQuestionManager->SetIsAllClear(false);
	Master::mpCommenterManager->SetCommentSpawnKind(STATE_BEFORE);
	Master::mpCommenterManager->SetIsSuperHintDraw(false);

	mnCorrectAnswer = new EffekseerEffect("Resource/Effect/CorrectAnswer.efk",300.0f);
	mnFallingCrossMiss = new EffekseerEffect("Resource/Effect/FallingCrossMiss.efk", 95.0f);
	mn10CorrectAnswersEffect = new EffekseerEffect("Resource/Effect/10CorrectAnswersEffect4.efk", 30.0f);

	mn10CorrectAnswersEffect->SetManualDraw(true);
	//切り抜き用の仮キャンバスを作成する
	mn10CorrectEffectScreen = MakeScreen(Master::Width, Master::Height, FALSE);


	Master::mpCommenterManager->ResetCommentCount();
	mnNextButtonHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/NextButton.png");
	//Master::mpUIManager->AddUI(mnNextButtonHandle,mnNextButtonX1, mnNextButtonY1, mnNextButtonX2, mnNextButtonY2, false);
	
	//ここで時間の初期化
	lastTime = GetNowCount();

	mfNextButtonDrawTime = 1.0f;
	if (Master::mpViewers->GetCurrentViewers() <= 0)
	{
		Master::mpViewers->Initialize();
		Master::mpSubscribers->Initialize();
	}
}
void GameCycle::Update()
{
	Master::mpCommenterManager->Update();
	Master::mpViewers->Update();
	Master::mpSubscribers->Update();

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
	
	if (mn10CorrectAnswersEffect != nullptr)
	{
		// 再生中なら画面揺れに合わせて位置を追従させる
		if (mn10CorrectAnswersEffect->IsPlaying())
		{
			mn10CorrectAnswersEffect->SetPosition(
				static_cast<float>(Master::gridWidth * 45 + mnShakeX),
				static_cast<float>(Master::gridHeight * 50 + mnShakeY)
			);
		}
		mn10CorrectAnswersEffect->Update();
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
		mbIsDrawSuperHint = false;
		Master::mpTextInputManager->SetIsAlphabet(Master::mpQuestionManager->GetQuestionData().isInputEnglish);
		mnQuestionTimer = 0;
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
		mnQuestionTimer = 0;
		if (mnQuestionNumber >= mnMaxQuestionNumber)
		{
			// 海中背景の真ん中で再生する
			mn10CorrectAnswersEffect->Play(
				static_cast<float>(Master::gridWidth * 45),
				static_cast<float>(Master::gridHeight * 50)
			);

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
	//Master::mpSubscribers->Draw();
	if (mbQuestionNumberDrawn && mbQuestionDrawn)
	{
		QuestionTimer();
		Master::mpTextInputManager->Draw();
	}

	if (mbGameEnd)
	{
		DrawNextButton();
	}
	else
	{
		QuestionDraw();

		Master::mpViewers->Draw();
		if (mbQuestionNumberDrawn && mbQuestionDrawn)
		{
			mTextInputManager.Draw();
		}
	}

	//変更：問題画面・入力画面をすべて描き終わった「一番手前」にエフェクトを描く
	Draw10CorrectEffect();
}


void GameCycle::Finalize()
{
	if (mn10CorrectAnswersEffect != nullptr)
	{
		mn10CorrectAnswersEffect->Stop();
		delete mn10CorrectAnswersEffect;
		mn10CorrectAnswersEffect = nullptr;
	}

	//仮キャンバスのメモリを解放する
	if (mn10CorrectEffectScreen != -1)
	{
		DeleteGraph(mn10CorrectEffectScreen);
		mn10CorrectEffectScreen = -1;
	}

	Master::mpTextInputManager->Finalize();
	Master::mpCommenterManager->GameEnd();
}
void GameCycle::CommentDraw()
{
	

	//その上からコメントを描画する
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
		if (Master::mpTextInputManager->IsAlphabet())
		{
			answer = Master::mpTextInputManager->GetInputStringAlphabet();
		}
		else
		{
			answer = Master::mpTextInputManager->GetInputString();
		}

		
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
		Master::mpSubscribers->AddSubscribers();

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
	if (mfNextButtonDrawTime >= 0.0f)
	{
		mfNextButtonDrawTime -= 1.0f / 60.0f;
		return;
	}

	int mouseX, mouseY;
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
	bool inside = Master::mpUIManager->InsideUI(static_cast<int>(mnNextButtonX1), static_cast<int>(mnNextButtonY1), static_cast<int>(mnNextButtonX2), static_cast<int>(mnNextButtonY2), mouseX, mouseY);
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * mfNextButtonDrawEasing));
	if (inside)
	{
		int centerX = int((mnNextButtonX1 + mnNextButtonX2) * 0.5f);
		int centerY = int((mnNextButtonY1 + mnNextButtonY2) * 0.5f);

		int halfX = int((mnNextButtonX2 - mnNextButtonX1) * 0.5f * 1.1f);
		int halfY = int((mnNextButtonY2 - mnNextButtonY1) * 0.5f * 1.1f);

		int drawX1 = centerX - halfX;
		int drawY1 = centerY - halfY;
		int drawX2 = centerX + halfX;
		int drawY2 = centerY + halfY;

		DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, mnNextButtonHandle, TRUE);

		if (Master::mpUIManager->ClickedUI(&mbClickButton, inside))
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
			Master::mpTransitor->AddTransitorProcessingUpdate("CycleViewerCount", [&]() {Master::mpViewers->Update(); });
			Master::mpTransitor->XFadeInit(SceneManager::SCENE_RESULT);
		}
	}
	else
	{
		DrawExtendGraph(int(mnNextButtonX1), int(mnNextButtonY1), int(mnNextButtonX2), int(mnNextButtonY2), mnNextButtonHandle, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	mfNextButtonDrawEasing += 1.0f / (60.0f * 1.5f);
}

void GameCycle::QuestionTimer()
{
	int currentTime = GetNowCount();
	//ポーズ中でないかつ既に大ヒントを出していないとき
	if (!Master::mpPause->IsPause() && !mbIsDrawSuperHint)
	{
		//カウント
		mnQuestionTimer += currentTime - lastTime;
	}
	lastTime = currentTime;

	//大ヒントを出す時間になったとき大ヒントフラグをtrueにする
	if (mnQuestionTimer >= mnLimitQuestionTimer)
	{
		//時間リセット
		mnQuestionTimer = 0;
		Master::mpCommenterManager->SetIsSuperHintDraw(true);
		mbIsDrawSuperHint = true;	//大ヒントを出した
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
			Master::mpSubscribers->Update();
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

void GameCycle::Draw10CorrectEffect()
{
	// 再生中でない、または仮キャンバスが準備できていなければ何もしない
	if (mn10CorrectAnswersEffect == nullptr || !mn10CorrectAnswersEffect->IsPlaying() || mn10CorrectEffectScreen == -1)
	{
		return;
	}

	//今の裏画面（Stage1・BackBoard・InputPlace・問題・入力文字が描かれた状態）を仮キャンバスにコピー
	GetDrawScreenGraph(0, 0, Master::Width, Master::Height, mn10CorrectEffectScreen);

	//描き先を仮キャンバスに切り替え、一番上にエフェクトを描く
	SetDrawScreen(mn10CorrectEffectScreen);
	RefreshDxLibDirect3DSetting(); // ★追加：Effekseerに描画先の変更を伝える
	mn10CorrectAnswersEffect->Draw();

	//描き先を元の裏画面に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// Stage1.png の枠内だけ切り抜いて貼り付ける（枠外にはみ出た部分は表示されない）
	SetDrawArea(
		Master::gridWidth * 4 + mnShakeX,
		Master::gridHeight * 6 + mnShakeY,
		Master::gridWidth * 86 + mnShakeX,
		Master::gridHeight * 95 + mnShakeY
	);
	DrawGraph(0, 0, mn10CorrectEffectScreen, FALSE);
	SetDrawAreaFull();
}