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
	QuestionScreen = Master::mpResourceManager->LoadGraphics("Resource/GameScene/WhiteBoard.png");
	InputField = Master::mpResourceManager->LoadGraphics("Resource/GameScene/InputPlace.png");
	UIManager::AddUI(QuestionScreen, Master::gridWidth * 7, Master::gridHeight * 12, Master::gridWidth * 65, Master::gridHeight * 75);
	UIManager::AddUI(InputField, Master::gridWidth * 7, Master::gridHeight * 84, Master::gridWidth * 65, Master::gridHeight * 96);
	mGameCycle.Initialize();
}

void GameScene::Update()
{

	mGameCycle.Update();
	Scene::Update();
}

void GameScene::Draw()
{
	UIManager::DrawUI();
	
	mGameCycle.Draw();
	Scene::Draw();
}

void GameScene::Finalize()
{
	UIManager::ClearUI();
	mGameCycle.Finalize();
}

