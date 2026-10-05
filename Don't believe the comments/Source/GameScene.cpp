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
	BackGround = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/Stage1.png");
	QuestionScreen = Master::mpResourceManager->LoadGraphics("Resource/GameScene/WhiteBoard.png");
	InputField = Master::mpResourceManager->LoadGraphics("Resource/GameScene/InputPlace.png");
	UIManager::AddUI(BackGround, Master::gridWidth * 4, Master::gridHeight * 6, Master::gridWidth * 86, Master::gridHeight * 95);
	UIManager::AddUI(QuestionScreen, Master::gridWidth * 18, Master::gridHeight * 12, Master::gridWidth * 72, Master::gridHeight * 73);
	UIManager::AddUI(InputField, Master::gridWidth * 22, Master::gridHeight * 80, Master::gridWidth * 68, Master::gridHeight * 90);

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
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(255, 255, 255), TRUE);
	UIManager::DrawUI();
	mGameCycle.Draw();
	Scene::Draw();
}

void GameScene::Finalize()
{
	UIManager::ClearUI();
	mGameCycle.Finalize();
}

