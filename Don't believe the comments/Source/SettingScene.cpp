#include "SettingScene.h"
#include "Master.h"

SettingScene::SettingScene()
	: settingGraph(-1)
	, volumeBar(-1)
	, volumeBarBackGround(-1)
	, volumeDial(-1)
	, mnBackButtonHandle(0)
	, dialWidth(0)
	, dialHeight(0)
	, volumeGaugeWidth(2093)
	, volumeGaugeHeight(126)
	, barLeftX(0.0f)
	, barRightX(0.0f)
	, overallLength(0)
{
}

SettingScene::~SettingScene()
{
}

void SettingScene::Initialize()
{
	// 画像読み込み
	settingGraph = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/SettingBackGround.png");
	volumeBar = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/VolumeBar.png");
	volumeBarBackGround = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/BarBackGround.png");
	volumeDial = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/VolumeDial.png");
	mnBackButtonHandle = Master::mpResourceManager->LoadGraphics("Resource/SettingScene/BackButton.png");

	// つまみサイズ取得と調整
	GetGraphSize(volumeDial, &dialWidth, &dialHeight);
	dialWidth = (int)(dialWidth * 0.9f);
	dialHeight = (int)(dialHeight * 0.8f);

	// 背景と戻るボタンをUIManagerに登録（※volumeBarは2本描くためDraw内で直接描画します）
	Master::mpUIManager->AddUI(settingGraph, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnBackButtonHandle, Master::gridWidth * 47, Master::gridHeight * 90, Master::gridWidth * 74, Master::gridHeight * 107);

	// BGM再生
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_SETTING);

	// バーの横方向の位置（BGMとSEで共通）
	barLeftX = 510.0f;
	barRightX = 1365.0f;
	overallLength = (int)(barRightX - barLeftX);
	volumeGaugeX = Master::gridWidth * 32.8f;

	// BGMとSEのスライダーをそれぞれ初期化（第2引数の数字を変えると上下の位置を調整できます）
	InitSlider(bgmSlider, 50.0f, Master::mpSoundManager->GetBGMVolume());
	InitSlider(seSlider, 70.0f, Master::mpSoundManager->GetSEVolume());
}

void SettingScene::InitSlider(VolumeSlider& slider, float gridY, int initialVolume)
{
	slider.barTopY = Master::gridHeight * gridY;
	slider.barBottomY = Master::gridHeight * (gridY + 7.0f);
	slider.nowVolume = initialVolume;
	slider.volumeRatio = slider.nowVolume / 100.0f;
	slider.dialX = barLeftX + (overallLength * slider.volumeRatio);
	slider.dialY = Master::gridHeight * (gridY + 3.5f) - (dialHeight / 2.0f);
	slider.volumeGaugeY = Master::gridHeight * (gridY + 1.0f);
	slider.grabTheDial = false;
}

void SettingScene::Update()
{
	// マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&MouseX, &MouseY);

	// 戻るボタンの処理
	bool insideStartButton = Master::mpUIManager->InsideUI(mnBackButtonHandle, MouseX, MouseY);
	if (insideStartButton)
	{
		Master::mpUIManager->ChangeUISize(mnBackButtonHandle, 1.1f);
	}
	else
	{
		Master::mpUIManager->ResetUISize(mnBackButtonHandle);
	}
	if (Master::mpUIManager->ClickedUI(mnBackButtonHandle, insideStartButton))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}

	// 1フレーム前にSEのつまみを掴んでいたかを覚えておく
	bool prevGrabSE = seSlider.grabTheDial;

	// スライダー更新（片方を掴んでいる間は、もう片方を掴めないようにする）
	UpdateSlider(bgmSlider, !seSlider.grabTheDial);
	UpdateSlider(seSlider, !bgmSlider.grabTheDial);

	// BGMを操作中ならBGM音量を反映
	if (bgmSlider.grabTheDial)
	{
		Master::mpSoundManager->ChangeBGMVolume(bgmSlider.nowVolume);
		bgmSlider.nowVolume = Master::mpSoundManager->GetBGMVolume();
	}

	// SEを操作中ならSE音量を反映
	if (seSlider.grabTheDial)
	{
		Master::mpSoundManager->ChangeSEVolume(seSlider.nowVolume);
		seSlider.nowVolume = Master::mpSoundManager->GetSEVolume();
	}

	// SEのつまみを「離した瞬間」に確認用のSEを1回鳴らす
	if (prevGrabSE && !seSlider.grabTheDial)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
	}

	// 長押しエフェクト（BGMかSEのどちらかを掴んでいる間だけ再生）
	if (bgmSlider.grabTheDial)
	{
		Master::mpEffectManager->KeepPlayingEffect(
			bgmSlider.dialX + (dialWidth / 2.0f),
			bgmSlider.dialY + (dialHeight / 2.0f)
		);
	}
	else if (seSlider.grabTheDial)
	{
		Master::mpEffectManager->KeepPlayingEffect(
			seSlider.dialX + (dialWidth / 2.0f),
			seSlider.dialY + (dialHeight / 2.0f)
		);
	}
	else
	{
		Master::mpEffectManager->StopHoldEffect();
	}

	// キーボードの左右キー（BGM音量の微調整）
	bool isVolumeChanged = false;
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		bgmSlider.nowVolume++;
		if (bgmSlider.nowVolume > 100) bgmSlider.nowVolume = 100;
		isVolumeChanged = true;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		bgmSlider.nowVolume--;
		if (bgmSlider.nowVolume < 0) bgmSlider.nowVolume = 0;
		isVolumeChanged = true;
	}
	if (isVolumeChanged)
	{
		Master::mpSoundManager->ChangeBGMVolume(bgmSlider.nowVolume);
		bgmSlider.volumeRatio = bgmSlider.nowVolume / 100.0f;
		bgmSlider.dialX = barLeftX + (overallLength * bgmSlider.volumeRatio);
	}

	// バックスペースキーでタイトルに戻る
	if (CheckHitKey(KEY_INPUT_BACK))
	{
		Master::mpTransitor->AddTransitorAfterUpdate("SettingBGMStop", [&]() { Master::mpSoundManager->StopBGM(); });
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}

	Scene::Update();
}

void SettingScene::UpdateSlider(VolumeSlider& slider, bool canGrab)
{
	// 左クリックが押されているか
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		// まだ掴んでおらず、もう片方も掴んでいない状態で、つまみの上をクリックしたら掴む
		if (!slider.grabTheDial && canGrab)
		{
			if (MouseX > slider.dialX && MouseX < (slider.dialX + dialWidth) &&
				MouseY > slider.dialY && MouseY < (slider.dialY + dialHeight))
			{
				slider.grabTheDial = true;
			}
		}
	}
	else
	{
		// マウスボタンを離したらフラグをfalseにする
		slider.grabTheDial = false;
	}

	// つまみを掴んでいる間はマウスに追従させる
	if (slider.grabTheDial)
	{
		slider.dialX = MouseX - (dialWidth / 2.0f);

		// バーの左右からはみ出ないように制限
		if (slider.dialX < barLeftX)  slider.dialX = barLeftX;
		if (slider.dialX > barRightX) slider.dialX = barRightX;

		// 割合と音量を計算
		float barLength = barRightX - barLeftX;
		float currentDistance = slider.dialX - barLeftX;
		float volume = (currentDistance / barLength) * 100.0f;

		slider.nowVolume = (int)volume;
		slider.volumeRatio = volume / 100.0f;
	}
}

void SettingScene::Draw()
{
	Master::mpUIManager->DrawUI();

	// BGM用・SE用のスライダーをそれぞれ描画
	DrawSlider(bgmSlider, "BGM");
	DrawSlider(seSlider, "SE");

	// タイトル文字
	int color = GetColor(0, 0, 0);
	int size = GetFontSize();
	SetFontSize(45);
	DrawString((int)(Master::gridHeight * 82), (int)(Master::gridWidth * 23), "  音  量  設  定  ", color);
	SetFontSize(size);

	int debugColor = GetColor(255, 0, 0); // 赤色

	// 1. つまみの当たり判定を赤枠で表示（画像の見た目とズレていないか確認）
	DrawBox(
		(int)bgmSlider.dialX, (int)bgmSlider.dialY,
		(int)bgmSlider.dialX + dialWidth, (int)bgmSlider.dialY + dialHeight,
		debugColor, FALSE
	);
	DrawBox(
		(int)seSlider.dialX, (int)seSlider.dialY,
		(int)seSlider.dialX + dialWidth, (int)seSlider.dialY + dialHeight,
		debugColor, FALSE
	);
	//デバッグ
	// 2. マウス座標・掴み状態・音量を左上に表示
	//DrawFormatString(10, 10, debugColor, "Mouse: X=%d, Y=%d", MouseX, MouseY);
	//DrawFormatString(10, 30, debugColor, "BGM: vol=%d (Manager=%d) grab=%d",
	//	bgmSlider.nowVolume, Master::mpSoundManager->GetBGMVolume(), bgmSlider.grabTheDial);
	//DrawFormatString(10, 50, debugColor, "SE : vol=%d (Manager=%d) grab=%d",
	//	seSlider.nowVolume, Master::mpSoundManager->GetSEVolume(), seSlider.grabTheDial);

	Scene::Draw();
}

void SettingScene::DrawSlider(const VolumeSlider& slider, const char* label)
{
	// 1. バーの外枠を描画
	DrawExtendGraph(
		(int)(Master::gridWidth * 32),
		(int)slider.barTopY,
		(int)(Master::gridWidth * 87),
		(int)slider.barBottomY,
		volumeBar,
		TRUE
	);

	// 2. ゲージの中身を描画
	int displayGaugeHeight = 45;
	int displayWidth = (int)(overallLength * slider.volumeRatio);
	int clipWidth = (int)(volumeGaugeWidth * slider.volumeRatio);

	DrawRectExtendGraph(
		(int)volumeGaugeX,
		(int)slider.volumeGaugeY,
		(int)volumeGaugeX + displayWidth,
		(int)slider.volumeGaugeY + displayGaugeHeight,
		0,
		0,
		clipWidth,
		volumeGaugeHeight,
		volumeBarBackGround,
		TRUE
	);

	// 3. つまみを描画
	DrawExtendGraph(
		(int)slider.dialX,
		(int)slider.dialY,
		(int)slider.dialX + dialWidth,
		(int)slider.dialY + dialHeight,
		volumeDial,
		TRUE
	);

	int color = GetColor(0, 0, 0);
	int size = GetFontSize(); // 元のフォントサイズを覚えておく

	// ▼ ここから「BGM」「SE」の描画（「音量設定」と同じサイズ 45 にする）
	SetFontSize(45);
	DrawString((int)(Master::gridWidth * 20), (int)(slider.barTopY + 8), label, color);

	// ▼ ここから音量数値（0〜100）の描画（元のサイズ 32 に切り替える）
	SetFontSize(32);
	if (slider.nowVolume >= 10 && slider.nowVolume <= 99)
	{
		DrawFormatString((int)slider.dialX - 8, (int)slider.dialY + 60, color, "%d", slider.nowVolume);
	}
	else if (slider.nowVolume <= 9)
	{
		DrawFormatString((int)slider.dialX + 2, (int)slider.dialY + 60, color, "%d", slider.nowVolume);
	}
	else if (slider.nowVolume == 100)
	{
		DrawFormatString((int)slider.dialX - 15, (int)slider.dialY + 60, color, "%d", slider.nowVolume);
	}

	// 最後に最初のフォントサイズへ戻す
	SetFontSize(size);
}

void SettingScene::Finalize()
{
	Master::mpUIManager->ClearUI();
}