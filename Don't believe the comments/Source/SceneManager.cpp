#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "DxLib.h"

SceneManager::SceneManager()
	:mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
	, mpCurrentScene(nullptr)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	//初期シーンの設定
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;

	// シーン遷移をさせる
	ChageSceneIfNeeded();
}

void SceneManager::Update()
{
	// シーンの更新
	mpCurrentScene->Update();
}

void SceneManager::Draw()
{
	// シーンの描画
	mpCurrentScene->Draw();
}

void SceneManager::Finalize()
{
	mpCurrentScene->Finalize();
}

void SceneManager::ChageSceneIfNeeded()
{
	// 現在シーンと次シーンが一緒であるならば何もしない
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		// 現在シーンの終了処理をする
		mpCurrentScene->Finalize();

		// 一旦シーン自体も破棄しておく
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	// 次シーンにするためシーンタイプを更新
	mnSceneType = mnNextSceneType;

	// mnSceneType に応じてシーンを生成する
	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;
	default:
		break;
	}

	// シーンの生成がされているはずなので、初期化処理を呼んでおく
	mpCurrentScene->Initialize();
}


