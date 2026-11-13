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

	Master::mpUIManager->AddUI(commentFieldHandle, Master::gridWidth * 88, Master::gridHeight * 3, Master::gridWidth * 118, Master::gridHeight * 115);

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
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(229, 242, 248), TRUE);

	
	//GameCycle ���猻�݂̗h��̃Y���ʂ��󂯎��
	int shakeX = mGameCycle.GetShakeX();
	int shakeY = mGameCycle.GetShakeY();

	//�z�M�w�i�E���{�[�h�E�������͘g�̍��W�� shakeX �� shakeY �𑫂��ĕ`�悷��

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

