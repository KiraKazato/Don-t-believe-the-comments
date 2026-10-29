#include "LevelSelectScene.h"
#include"Master.h"


LevelSelectScene::LevelSelectScene()
	: mnSelectBackGroundGraphHandle(0)
	, mnDay1ButtonGraphHandle(0)
	, mnComingSoonGraphHandle(0)
	,mnRightArrowGraph(0)
	,mnLeftArrowGraph(0)
	,currentStage(StageNumber::STAGE_DAY1)
{

}

LevelSelectScene::~LevelSelectScene()
{

}

void LevelSelectScene::Initialize()
{
	//背景画像の読み込み
	mnSelectBackGroundGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/LevelSelectBackGround.png");

	//ボタンのステート設定
	mStageButtonHandle.resize(ButtonType::MAX_BUTTON);
	//day1ボタンの読み込み
	mStageButtonHandle[STAGE_DAY1] = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/Day1Button.png");
	mStageButtonHandle[STAGE_COMMINGSOON] = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/ComingSoon.png");
	mnRightArrowGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/RightArrow.png");
	mnLeftArrowGraph = Master::mpResourceManager->LoadGraphics("Resource/LevelSelectScene/LeftArrow.png");

	//必要なUI
	Master::mpUIManager->AddUI(mnSelectBackGroundGraphHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnRightArrowGraph, Master::gridWidth * 78, Master::gridHeight * 69, Master::gridWidth * 84, Master::gridHeight * 80);;
	Master::mpUIManager->AddUI(mnLeftArrowGraph, Master::gridWidth * 37, Master::gridHeight * 69, Master::gridWidth * 43, Master::gridHeight * 80, false);

	for (int number = 0; number < MAX_STAGE; number++)
	{
		if (number == STAGE_DAY1)
		{
			Master::mpUIManager->AddUI(mStageButtonHandle[number], stageButtonPositionX1, stageButtonPositionY1, stageButtonPositionX2, stageButtonPositionY2);
		}
		else
		{
			Master::mpUIManager->AddUI(mStageButtonHandle[number], stageButtonPositionX1, stageButtonPositionY1, stageButtonPositionX2, stageButtonPositionY2,false);
		}
	}
}

void LevelSelectScene::Update()
{
	Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
	//画像の中にカーソルがあったら
	Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage])->insideObject = Master::mpUIManager->InsideUI(mnDay1ButtonGraphHandle, mouseX, mouseY);
	if (Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage]) != nullptr)
	{
		Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage])->insideObject = Master::mpUIManager->InsideUI(mStageButtonHandle[currentStage], mouseX, mouseY);
		if (Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage])->insideObject)
		{
			//少し大きく
			Master::mpUIManager->ChangeUISize(mStageButtonHandle[currentStage], 1.1f);
		}
		else
		{
			//なかったらサイズをもとに戻す
			Master::mpUIManager->ResetUISize(mStageButtonHandle[currentStage]);
		}
		//左クリックされたら
		if (Master::mpUIManager->ClickedUI(mStageButtonHandle[currentStage], Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage])->insideObject))
		{
			if (currentStage != STAGE_COMMINGSOON)
			{
				//ゲームシーンへ
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
			}

		}
	}


	if (Master::mpUIManager->GetUIData(mnRightArrowGraph)->displayed)
	{
		if (Master::mpUIManager->GetUIData(mnRightArrowGraph) != nullptr)
		{
			Master::mpUIManager->GetUIData(mnRightArrowGraph)->insideObject = Master::mpUIManager->InsideUI(mnRightArrowGraph, mouseX, mouseY);

			if (Master::mpUIManager->GetUIData(mnRightArrowGraph)->insideObject)
			{
				//少し大きく
				Master::mpUIManager->ChangeUISize(mnRightArrowGraph, 1.07f);
			}
			else
			{
				//サイズをもとに戻す
				Master::mpUIManager->ResetUISize(mnRightArrowGraph);
			}


			//左クリックされたら
			if (Master::mpUIManager->ClickedUI(mnRightArrowGraph, Master::mpUIManager->GetUIData(mnRightArrowGraph)->insideObject))
			{
				//現在のlevelをComingSoonに
				if (currentStage + 1 < MAX_STAGE)
				{
					currentStage = static_cast<StageNumber>(currentStage + 1);
					ChangeStage();
				}
			}
		}

	}

	if (Master::mpUIManager->GetUIData(mnLeftArrowGraph)->displayed)
	{
		if (Master::mpUIManager->GetUIData(mnLeftArrowGraph) != nullptr)
		{
			Master::mpUIManager->GetUIData(mnLeftArrowGraph)->insideObject = Master::mpUIManager->InsideUI(mnLeftArrowGraph, mouseX, mouseY);

			if (Master::mpUIManager->GetUIData(mnLeftArrowGraph)->insideObject)
			{
				//少し大きく
				Master::mpUIManager->ChangeUISize(mnLeftArrowGraph, 1.07f);
			}
			else
			{
				//サイズをもとに戻す
				Master::mpUIManager->ResetUISize(mnLeftArrowGraph);
			}


			//左クリックされたら
			if (Master::mpUIManager->ClickedUI(mnLeftArrowGraph, Master::mpUIManager->GetUIData(mnLeftArrowGraph)->insideObject))
			{
				//現在のlevelをComingSoonに
				if (currentStage - 1 >= STAGE_DAY1)
				{
					currentStage = static_cast<StageNumber>(currentStage - 1);
					ChangeStage();
				}
			}
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

void LevelSelectScene::ChangeStage()
{
	switch (currentStage)
	{
	case STAGE_DAY1:
		//いらない矢印を非表示にし、レベルの変更
		Master::mpUIManager->GetUIData(mnLeftArrowGraph)->displayed = false;
		Master::mpUIManager->GetUIData(mnRightArrowGraph)->displayed = true;
		Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage + 1])->displayed = false;
		Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage ])->displayed = true;
		Master::mpQuestionManager->SetStage(currentStage);
		break;
	case STAGE_COMMINGSOON:
		//いらない矢印を非表示にし、表示レベルの変更
		Master::mpUIManager->GetUIData(mnRightArrowGraph)->displayed = false;
		Master::mpUIManager->GetUIData(mnLeftArrowGraph)->displayed = true;
		Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage - 1])->displayed = false;
		Master::mpUIManager->GetUIData(mStageButtonHandle[currentStage])->displayed = true;
		Master::mpQuestionManager->SetStage(currentStage);
		break;
	default:
		break;
	}
}
