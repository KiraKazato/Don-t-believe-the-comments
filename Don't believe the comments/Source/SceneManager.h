#pragma once

// クラスの前方宣言
class Scene;


class SceneManager
{
public:	// enum, struct の定義

	//シーン種類
	// note: シーンの数を増やす必要があるならばここも追加していくこと
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,	// 定義なし
		SCENE_TITLE,	// タイトル
	};

public:
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();

	//シーン遷移（切り替え処理）が必要な状態なら遷移処理をする
	void ChageSceneIfNeeded();

	// 次に遷移するシーンの設定
	// note: シーン遷移をしたい場合は、必ずこの処理を経由して遷移させる
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }

	// 現在シーンの取得
	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;			// 現在のシーンのタイプ
	SCENE_TYPE mnNextSceneType;		// 次のシーンのタイプ
	Scene* mpCurrentScene;			// 現在のシーンのポインタ
};
