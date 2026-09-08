#include "ObjectManager.h"
#include "Scene.h"

//コン
Scene::Scene()
{
	// ObjectManager を生成
	mpObjectManager = new ObjectManager();
}

Scene::~Scene()
{
	// ObjectManager を解放
	delete mpObjectManager;
	//mpObjectManager = nullptr;
}
//更新
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		 mpObjectManager->Update();
		 //mpObjectManager->DeleteAllIfNeeded(); // ★ フレーム最後
	}
}
//描画
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
}

