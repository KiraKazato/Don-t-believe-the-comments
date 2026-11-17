#pragma once

class Viewers
{
public:
	Viewers();
	~Viewers();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	//正解した時
	void ComeViewers();
	//不正解だった時
	void LeaveViewers();
	//最初の視聴者数決める
	void DecideViewers();
	//カウント
	void ViewersCount(float _duration);
	//視聴者数が変わったときの共通処理
	void ChangeViewers(int _changeValue);
	
	//ゲーム終了後の視聴者数の減少（演出面）
	void GameEndLeave(int _min, int _max, float _base);

	void SetGameStartViewers(int _count);
	void SetGameEndViewers(int _count);

	void SetIsGameEnd(bool _flag);

	int GetGameStartViewersCount();
	int GetGameEndViewersCount();

	int GetViewersCount();

	int GetTargetViewers();
	int GetCurrentViewers();

	void IdleFluctuation();
private:
	int mnCurrentViewers{};			//今の視聴者数
	int mnCountStartViewers{};		//カウントの開始値
	int mnTargetViewers{};			//カウントの目標値
	int mnMaxDefauktViewers = 500;
	int mnMinDefauktViewers = 300;
	int mnMaxAddValue = 600;
	int mnMinAddValue = 200;
	int mnMaxSubtractValue = 150;
	int mnMinSubtractValue = 50;
	int mnStringHandle{};
	int mnDrawX1{};
	int mnDrawY1;
	float mfScale = 1.0f; // 1.0f が通常の大きさ
	int mnMaxFluctuation = 2501;
	int mnMinFluctuation = 500;

	bool mbIsCountFinish = false;		//カウントが終わったか
	int mnViewersCount = 0;				//視聴者数の増減値

	int mnGameStartViewers = 0;			//ゲーム開始時点の視聴者数
	int mnGameEndViewers = 0;			//ゲーム終了時点の視聴者数

	float mfViewersCountTime = 0.0f;	//カウントの経過時間

	float mfViwersGameEndLeaveTime = 0.0f;	//ゲーム終了後の視聴者数減少間隔
	float mfRandLeaveMul = 0.7f;
	bool mbIsGameEnd = false;
};