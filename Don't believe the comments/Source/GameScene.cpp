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
	mGameCycle.Update();
	Scene::Update();
}

void GameScene::Draw()
{
	DrawExtendGraph(0, 0, Master::Width, Master::Height, mnBackGround, TRUE);

	
	
	int shakeX = mGameCycle.GetShakeX();
	int shakeY = mGameCycle.GetShakeY();

	

	DrawExtendGraph(
		Master::gridWidth * 4 + shakeX, Master::gridHeight * 6 + shakeY,
		Master::gridWidth * 86 + shakeX, Master::gridHeight * 95 + shakeY,
		backGroundHandle, TRUE
	);
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

