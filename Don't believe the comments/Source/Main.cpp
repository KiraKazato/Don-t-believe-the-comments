#include "DxLib.h"
#include <windows.h>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
/**
* @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

//Master クラスの静的メンバ変数定義
SceneManager* Master::mpSceneManager = new SceneManager();
SoundManager* Master::mpSoundManager = new SoundManager();
ResourceManager* Master::mpResourceManager = new ResourceManager();

/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(Master::Width, Master::Height, 32);
	// ウインドウモードで起動
	ChangeWindowMode(true);

	//実行ログを表示するか
	SetOutApplicationLogValidFlag(TRUE);
	

	// DXライブラリ初期化
	if(DxLib_Init() == -1)
	{
		return -1;
	}
	
	//サウンドマネージャーの初期化
	Master::mpSoundManager->Initialize();  
	// シーンマネージャーの生成と初期化
	Master::mpSceneManager->Initialize(); 
	

	// 描画先画面を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);
	
	//Zバッファに書き込む準備
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);


	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		int time = GetNowCount();

		Master::mpSceneManager->Update();
	
		//画面を初期化する
		ClearDrawScreen();

		
		Master::mpSceneManager->Draw();
		//裏画面の内容を表画面に映す
		ScreenFlip();
		//17ミリ秒　（秒間約６０フレームだった場合の１フレーム当たりの経過時間）
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			// 待つだけなのでここでは何も処理しない
		}

		//削除するオブジェクトがあれば削除する
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();

		//ループする直前にシーンの遷移チェックを入れておく
		Master::mpSceneManager->ChageSceneIfNeeded();
	}

	//終了処理
	//Finalize();
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;
	delete Master::mpResourceManager;

	
	

	// DXライブラリ使用の終了
	DxLib_End();

	//// ソフトの終了
	return 0;
}