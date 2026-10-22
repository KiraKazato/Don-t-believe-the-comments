#include "LevelSelectScene.h"
#include"Master.h"


LevelSelectScene::LevelSelectScene()
	:selectBackGroundGraph(0)
	,day1ButtonGraph(0)
	,comingSoonGraph(0)
	,rightArrowGraph(0)
	,leftArrowGraph(0)
	,currentLevel(LevelType::LEVEL_DAY1)
{

}

LevelSelectScene::~LevelSelectScene()
{

}

void LevelSelectScene::Initialize()
{
	//背景画像の読み込み
	selectBackGroundGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/LevelSelectBackGround.png");

	//day1ボタンの読み込み
	day1ButtonGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/Day1Button.png");

	comingSoonGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/ComingSoon.png");

	rightArrowGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/RightArrow.png");

	leftArrowGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/LeftArrow.png");

	//背景の設置
	Master::mpUIManager->AddUI(selectBackGroundGraph, 0, 0, Master::Width, Master::Height);

	Master::mpUIManager->AddUI(day1ButtonGraph, stageButtonPositionX1, stageButtonPositionY1, stageButtonPositionX2, stageButtonPositionY2);

	Master::mpUIManager->AddUI(rightArrowGraph, Master::gridWidth * 78, Master::gridHeight * 69, Master::gridWidth * 84, Master::gridHeight * 80);

}

void LevelSelectScene::Update()
{
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);

	
	if (currentLevel == LevelType::LEVEL_DAY1)
	{
		//画像の中にカーソルがあったら
		if (Master::mpUIManager->InsideUI(day1ButtonGraph, mouseX, mouseY))
		{
			//少し大きく
			Master::mpUIManager->ChangeUISize(day1ButtonGraph, 1.1f);

			//左クリックされたら
			if (Master::mpMouseManager->IsLeftClick())
			{
				//ゲームシーンへ
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
			}
		}
		else
		{
			//なかったらサイズをもとに戻す
			Master::mpUIManager->ResetUISize(day1ButtonGraph);
		}

		//右矢印画像の中にカーソルがあったら
		if (Master::mpUIManager->InsideUI(rightArrowGraph, mouseX, mouseY))
		{
			//少し大きく
			Master::mpUIManager->ChangeUISize(rightArrowGraph, 1.07f);

			//左クリックされたら
			if (Master::mpMouseManager->IsLeftClick())
			{
				//現在のlevelをComingSoonに
				currentLevel = LevelType::LEVEL_COMINGSOON;

				//いらない右矢印とDay１画像を削除
				Master::mpUIManager->EraseUI(day1ButtonGraph);
				Master::mpUIManager->EraseUI(rightArrowGraph);


				//comingsoon画像と左矢印画像をAdd
				Master::mpUIManager->AddUI(comingSoonGraph, stageButtonPositionX1, stageButtonPositionY1, stageButtonPositionX2, stageButtonPositionY2);
				Master::mpUIManager->AddUI(leftArrowGraph, Master::gridWidth * 37, Master::gridHeight * 69, Master::gridWidth * 43, Master::gridHeight * 80);
			}
		}
		else
		{
			//サイズをもとに戻す
			Master::mpUIManager->ResetUISize(rightArrowGraph);
		}
	}




	else if (currentLevel == LevelType::LEVEL_COMINGSOON)
	{
		//画像の中にカーソルがあったら
		if (Master::mpUIManager->InsideUI(comingSoonGraph, mouseX, mouseY))
		{
			//少し大きく
			Master::mpUIManager->ChangeUISize(comingSoonGraph, 1.1f);
		}
		else
		{
			//なかったらサイズをもとに戻す
			Master::mpUIManager->ResetUISize(comingSoonGraph);
		}


		//画像の中にカーソルがあったら
		if (Master::mpUIManager->InsideUI(leftArrowGraph, mouseX, mouseY))
		{
			//少し大きく
			Master::mpUIManager->ChangeUISize(leftArrowGraph, 1.07f);

			//左クリックされたら
			if (Master::mpMouseManager->IsLeftClick())
			{
				//現在のlevelをDay1に
				currentLevel = LevelType::LEVEL_DAY1;

				//不要な画像を削除
				Master::mpUIManager->EraseUI(comingSoonGraph);
				Master::mpUIManager->EraseUI(leftArrowGraph);

				//画像をAdd
				Master::mpUIManager->AddUI(day1ButtonGraph, Master::gridWidth * 48, Master::gridHeight * 56, Master::gridWidth * 73, Master::gridHeight * 95);
				Master::mpUIManager->AddUI(rightArrowGraph, Master::gridWidth * 78, Master::gridHeight * 69, Master::gridWidth * 84, Master::gridHeight * 80);
			}
		}
		else
		{
			//サイズをもとに戻す
			Master::mpUIManager->ResetUISize(leftArrowGraph);
		}
	}

	Scene::Update();
}

void LevelSelectScene::Draw()
{
	Master::mpUIManager->DrawUI();

	Scene::Draw();

}

void LevelSelectScene::Finalize()
{
	Master::mpUIManager->ClearUI();

}
