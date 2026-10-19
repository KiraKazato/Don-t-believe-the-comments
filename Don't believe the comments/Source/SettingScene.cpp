#include"SettingScene.h"
#include"Master.h"

SettingScene::SettingScene()
	:settingGraph(-1)
	,volumeBar(-1)
	,volumeBarBackGround(-1)
	,volumeDial(-1)
	,dialWidth(0)
	,dialHeight(0)
	,grabTheDial(false)
	,barLeftX(0)
	,barRightX(0)
	,overallLength(0)
	,nowVolume(0)
	,volumeRatio(0)
	,volumeGaugeHeight(0)
	,volumeGaugeWidth(0)
{

}

SettingScene::~SettingScene()
{

}

void SettingScene::Initialize()
{
	//設定画面背景
	settingGraph = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/SettingBackGround.png");

	//音量調節バー
	volumeBar = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/VolumeBar.png");

	//音量調節バーの背景(中身)
	volumeBarBackGround = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/BarBackGround.png");

	//音量調節に使うつまみ
	volumeDial = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/VolumeDial.png");

	//つまみサイズ
	GetGraphSize(volumeDial, &dialWidth, &dialHeight);

	//つまみのサイズ調整
	dialWidth = (int)(dialWidth * 0.9f);
	dialHeight = (int)(dialHeight * 0.8f);

	//設定画面背景の登録
	Master::mpUIManager->AddUI(settingGraph, 0, 0, Master::Width, Master::Height);

	//音量バーの登録
	Master::mpUIManager->AddUI(volumeBar, Master::gridWidth * 32, Master::gridHeight * 93, Master::gridWidth * 87, Master::gridHeight * 100);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_SETTING);//bgm再生

	//バーの右・左端座標
	barLeftX = 510;
	barRightX = 1365;

	//バー全体の長さを計算
	overallLength = (int)(barRightX - barLeftX);

	//現在の音量取得
	nowVolume = Master::mpSoundManager->GetBGMVolume();

	//取得した音量から比率をとる
	volumeRatio = (nowVolume / 100.0f);

	//音量調節つまみの位置
	dialX = barLeftX + (overallLength * volumeRatio);
	dialY = Master::gridHeight * 96.5f - (dialHeight / static_cast<float>(2));

	//ボリュームゲージの位置
	volumeGaugeX = (int)(Master::gridWidth * 32.8f);
	volumeGaugeY = (int)(Master::gridHeight * 94.0f);
}

void SettingScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&MouseX, &MouseY);

	//左クリックが押されているか
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		//押された範囲内がつまみ画像内だったら
		if (MouseX > dialX && MouseX < (dialX + dialWidth) &&
			MouseY > dialY && MouseY < (dialY + dialHeight))
		{
			//つまみを動かすフラグを発動
			grabTheDial = true;
		}
	}
	else//押されていない時はフラグを常にfalse
	{
		grabTheDial = false;
	}

	//つまみを動かすフラグがTrueだったら
	if (grabTheDial == true)
	{
		//つまみ座標のXをカーソルの位置に合わせる
		dialX = MouseX - (float(dialWidth / 2));
		
		//左側に飛び出ないように制限する
		if (dialX < barLeftX)
		{
			dialX = barLeftX;
		}

		//同じく右側も
		if (dialX > barRightX)
		{
			dialX = barRightX;
		}

		//バー全体の長さを求める
		float barLength = barRightX - barLeftX;

		//バーの左端からつまみの距離を求める
		float currentDistance = (float)(dialX - barLeftX);

		//距離の割合に応じた音量を設定
		float volume = ((currentDistance / barLength) * 100.0f);

		//設定した音量を反映させる
		Master::mpSoundManager->ChangeBGMVolume(int(volume));

		nowVolume = Master::mpSoundManager->GetBGMVolume();

		//ゲージと同期させるためここで更新
		volumeRatio = (volume / 100.0f);
	}

	bool isVolumeChanged = false; 

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		nowVolume++; 
		if (nowVolume > 100)
		{
			nowVolume = 100; 
		}
		isVolumeChanged = true; 
	}

	
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		nowVolume--;
		if (nowVolume < 0)
		{
			nowVolume = 0;
		}
		isVolumeChanged = true; 
	}

	if (isVolumeChanged == true)
	{
		Master::mpSoundManager->ChangeBGMVolume(nowVolume);

		volumeRatio = (nowVolume / 100.0f);

		dialX = barLeftX + (overallLength * volumeRatio);
	}

	//バックスペースキーでタイトルに戻る
	if (CheckHitKey(KEY_INPUT_BACK))
	{
		//BGMを止めてシーン遷移
		Master::mpSoundManager->StopBGM();
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}

	Scene::Update();
}

void SettingScene::Draw()
{
	Master::mpUIManager->DrawUI();

	//ゲージの幅・高さの定義
	volumeGaugeHeight = 126;
	volumeGaugeWidth = 2093;

	//ゲージの太さ
	int displayGaugeHeight = 45;

	//画面左端からつまみまでの長さ
	int displayWidth = (int)(overallLength * volumeRatio);

	//切り抜き幅の計算
	int clipWidth = (int)(volumeGaugeWidth * volumeRatio);

	DrawRectExtendGraph(
		(int)volumeGaugeX,//音量ゲージのX1座標
		(int)volumeGaugeY,//音量ゲージのY1座標
		(int)volumeGaugeX + displayWidth,//音量ゲージのX2座標
		(int)volumeGaugeY + displayGaugeHeight,//音量ゲージのY2座標
		0,//ゲージの元画像からの切り抜きX座標
		0,//ゲージの元画像からの切り抜きY座標
		clipWidth,//ゲージの元画像から切り抜く幅
		volumeGaugeHeight,//ゲージの元画像から切り抜く高さ
		volumeBarBackGround,//ゲージハンドル
		TRUE//背景透過するか？
	);

	DrawExtendGraph(
		(int)dialX,                      // 左端のX座標
		(int)dialY,                      // 上端のY座標
		(int)dialX + dialWidth,          // 右端のX座標（左端 ＋ つまみの幅）
		(int)dialY + dialHeight,         // 下端のY座標（上端 ＋ つまみの高さ）
		volumeDial,                 // 描きたい画像のハンドル
		TRUE                        // 背景を透過するかどうか
	);

	int color = GetColor(0, 0, 0);
	
	int size = GetFontSize();

	SetFontSize(32);

	//位置がおかしかったので調整
	if (nowVolume >= 10 && nowVolume <= 99)
	{
		DrawFormatString((int)dialX - 8, (int)dialY - 50, color, "%d", nowVolume);
	}
	if (nowVolume <= 9)
	{
		DrawFormatString((int)dialX+2, (int)dialY - 50, color, "%d", nowVolume);
	}
	if (nowVolume == 100)
	{
		DrawFormatString((int)dialX - 15, (int)dialY - 50, color, "%d", nowVolume);

	}
	SetFontSize(size);
	Scene::Draw();
}

void SettingScene::Finalize()
{
	Master::mpUIManager->ClearUI();
}
