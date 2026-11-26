#include "GameScene.h"
#include "Master.h"
#include "UIManager.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	Master::mpSoundManager->StopBGM();
	backGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/Stage1.png");
	questionScreenHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackBoard.png");
	inputFieldHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/InputPlace.png");
	commentFieldHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/CommentField.png");
	mnBackGround = Master::mpResourceManager->LoadGraphics("Resource/GameScene/GameBackGround.png");

	Master::mpUIManager->AddUI(commentFieldHandle, Master::gridWidth * 88, Master::gridHeight * 3, Master::gridWidth * 118, Master::gridHeight * 115);

	commentCountFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 50);

	mpBubbleEffect = new EffekseerEffect("Resource/Effect/AquaBubbleEffect.efk", 40.0f);
	mpBubbleEffect->SetManualDraw(true); // 手動描画モードにして好きな順番で描く

	//Stage1.pngの真ん中あたりで再生開始
	mpBubbleEffect->Play(
		static_cast<float>(Master::gridWidth * 45),
		static_cast<float>(Master::gridHeight * 50)
	);

	//背景＋バブルを描くための仮キャンバス
	mnBubbleScreen = MakeScreen(Master::Width, Master::Height, FALSE);


	mGameCycle.Initialize();
	Master::mpPause->Initialize();
	Master::mpQuestionManager->SetRandomNumber();
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME);
}

void GameScene::Update()
{
	Master::mpPause->Update();
	if (Master::mpPause->IsPause())
	{
		Scene::Update();
		return;
	}

	//バブルエフェクトのループ再生と位置更新
	if (mpBubbleEffect != nullptr)
	{
		// エフェクトの基準位置（Stage1.pngの中央あたり）
		float bubbleX = static_cast<float>(Master::gridWidth * 45 + mGameCycle.GetShakeX());
		float bubbleY = static_cast<float>(Master::gridHeight * 50 + mGameCycle.GetShakeY());

		// 再生が終わっていたらもう一度再生する（ループ）
		if (!mpBubbleEffect->IsPlaying())
		{
			mpBubbleEffect->Play(bubbleX, bubbleY);
		}
		else
		{
			// 再生中は画面揺れ（シェイク）に合わせて位置を追従させる
			mpBubbleEffect->SetPosition(bubbleX, bubbleY);
		}

		mpBubbleEffect->Update();
	}

	

	mGameCycle.Update();
	Scene::Update();
}

void GameScene::Draw()
{
	int shakeX = mGameCycle.GetShakeX();
	int shakeY = mGameCycle.GetShakeY();

	// Stage1.png の表示範囲を変数にまとめておく
	int bgX1 = Master::gridWidth * 4 + shakeX;
	int bgY1 = Master::gridHeight * 6 + shakeY;
	int bgX2 = Master::gridWidth * 86 + shakeX;
	int bgY2 = Master::gridHeight * 95 + shakeY;

	//まず仮キャンバスに「Stage1.png → バブルエフェクト」の順で描く
	if (mnBubbleScreen != -1)
	{
		SetDrawScreen(mnBubbleScreen); // 描き先を仮キャンバスへ
		ClearDrawScreen();

		//Stage1の背景を描く
		DrawExtendGraph(bgX1, bgY1, bgX2, bgY2, backGroundHandle, TRUE);

		//そのすぐ上にバブルエフェクトを描く
		if (mpBubbleEffect != nullptr)
		{
			mpBubbleEffect->Draw();
		}

		SetDrawScreen(DX_SCREEN_BACK); // 描き先を元の裏画面に戻す
	}

	//裏画面に大元の背景（GameBackGround.png）を描く
	DrawExtendGraph(0, 0, Master::Width, Master::Height, mnBackGround, TRUE);

	//Stage1.pngの枠内だけ切り抜いて、仮キャンバス（背景＋バブル）を貼り付ける
	if (mnBubbleScreen != -1)
	{
		SetDrawArea(bgX1, bgY1, bgX2, bgY2);
		DrawGraph(0, 0, mnBubbleScreen, FALSE);
		SetDrawAreaFull();
	}
	else
	{
		// 万が一仮キャンバスが作れなかったときの保険
		DrawExtendGraph(bgX1, bgY1, bgX2, bgY2, backGroundHandle, TRUE);
	}

	//その上から問題画面と入力画面を描く（これでバブルが裏側に回る！）
	DrawExtendGraph(
		Master::gridWidth * 18 + shakeX, Master::gridHeight * 12 + shakeY,
		Master::gridWidth * 72 + shakeX, Master::gridHeight * 73 + shakeY,
		questionScreenHandle, TRUE
	);
	DrawExtendGraph(
		Master::gridWidth * 22 + shakeX, Master::gridHeight * 80 + shakeY,
		Master::gridWidth * 68 + shakeX, Master::gridHeight * 90 + shakeY,
		inputFieldHandle, TRUE
	);

	Master::mpUIManager->DrawUI();
	mGameCycle.Draw();
	Scene::Draw();

	DrawDecoration();

	if (Master::mpPause->IsPause())
	{
		Master::mpPause->Draw();
	}
}

void GameScene::Finalize()
{
	//バブルエフェクトと仮キャンバスの後片付け
	if (mpBubbleEffect != nullptr)
	{
		mpBubbleEffect->Stop();
		delete mpBubbleEffect;
		mpBubbleEffect = nullptr;
	}
	if (mnBubbleScreen != -1)
	{
		DeleteGraph(mnBubbleScreen);
		mnBubbleScreen = -1;
	}

	Master::mpUIManager->ClearUI();
	mGameCycle.Finalize();
}

void GameScene::DrawDecoration()
{
	const int baseX1 = Master::gridWidth * 4;
	const int baseX2 = Master::gridWidth * 86;
	const int baseY1 = Master::gridHeight * 95;
	const int baseY2 = Master::gridHeight * 115;

	int width = GetDrawFormatStringWidthToHandle(commentCountFontHandle, "コメント数: ");
	int countWidth = GetDrawFormatStringWidthToHandle(commentCountFontHandle, "%d", Master::mpCommenterManager->GetCommentsCount());

	int drawX = baseX2 - width - (Master::gridWidth * 10);
	DrawFormatStringToHandle(drawX, baseY1 + (Master::gridHeight * 2), GetColor(0, 0, 0), commentCountFontHandle, "コメント数: ");
	DrawFormatStringToHandle(baseX2 - countWidth, baseY1 + (Master::gridHeight * 2), GetColor(0, 0, 0), commentCountFontHandle, "%d", Master::mpCommenterManager->GetCommentsCount());
}

