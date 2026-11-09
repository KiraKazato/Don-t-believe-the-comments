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


	void SetStartViewers(int _count);

	int GetStartViewersCount();
	int GetViewersCount();
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
	int mnDrawY1{};

	bool mbIsCountFinish = false;		//カウントが終わったか
	int mnStartViewersCount = 0;		//視聴者数初期値
	int mnViewersCount = 0;				//視聴者数の増減値

	float mfViewersCountTime = 0.0f;	//カウントの経過時間
};