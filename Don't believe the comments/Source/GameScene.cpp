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
	backGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/Stage1.png");
	questionScreenHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackBoard.png");
	inputFieldHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/InputPlace.png");
	commentFieldHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/CommentField.png");

	Master::mpUIManager->AddUI(commentFieldHandle, Master::gridWidth * 88, Master::gridHeight * 3, Master::gridWidth * 118, Master::gridHeight * 115);

	mGameCycle.Initialize();
	Master::mpQuestionManager->SetRandomNumber();
}

void GameScene::Update()
{
	mGameCycle.Update();
	Scene::Update();
}

void GameScene::Draw()
{
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(229, 242, 248), TRUE);

	
	//GameCycle ‚©‚çŒ»Ý‚Ì—h‚ê‚ÌƒYƒŒ—Ê‚ðŽó‚¯Žæ‚é
	int shakeX = mGameCycle.GetShakeX();
	int shakeY = mGameCycle.GetShakeY();

	//”zM”wŒiE–â‘èƒ{[ƒhE•¶Žš“ü—Í˜g‚ÌÀ•W‚É shakeX ‚Æ shakeY ‚ð‘«‚µ‚Ä•`‰æ‚·‚é

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
}

void GameScene::Finalize()
{
	Master::mpUIManager->ClearUI();
	mGameCycle.Finalize();
}

