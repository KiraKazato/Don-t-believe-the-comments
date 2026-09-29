#include "GameScene.h"
#include "Master.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	mGameCycle.Initialize();
}

void GameScene::Update()
{
	mGameCycle.Update();
	Scene::Update();
}

void GameScene::Draw()
{
	mGameCycle.Draw();
	Scene::Draw();
}

void GameScene::Finalize()
{
	mGameCycle.Finalize();
}

