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
	Master::mpUIManager->AddUI(backGroundHandle, Master::gridWidth * 4, Master::gridHeight * 6, Master::gridWidth * 86, Master::gridHeight * 95);
	Master::mpUIManager->AddUI(questionScreenHandle, Master::gridWidth * 18, Master::gridHeight * 12, Master::gridWidth * 72, Master::gridHeight * 73);
	Master::mpUIManager->AddUI(inputFieldHandle, Master::gridWidth * 22, Master::gridHeight * 80, Master::gridWidth * 68, Master::gridHeight * 90);
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
	
	Master::mpUIManager->DrawUI();
	mGameCycle.Draw();
	Scene::Draw();
}

void GameScene::Finalize()
{
	Master::mpUIManager->ClearUI();
	mGameCycle.Finalize();
}

