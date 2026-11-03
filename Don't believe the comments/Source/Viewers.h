#pragma once
#include "DxLib.h"
#include "Master.h"
class Viewers
{
public:
	Viewers();
	~Viewers();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	//³‰ğ‚µ‚½
	void ComeViewers();
	//•s³‰ğ‚¾‚Á‚½
	void LeaveViewers();
	//Å‰‚Ì‹’®Ò”Œˆ‚ß‚é
	void DecideViewers();
private:
	int mnCurrentViwer{};
	int mnMaxDefauktViewers = 150;
	int mnMinDefauktViewers = 50;
	int mnMaxAddValue = 400;
	int mnMinAddValue = 100;
	int mnMaxSubtractValue = 200;
	int mnMinSubtractValue = 100;
	int mnStringHandle{};
	int mnDrawX1{};
	int mnDrawY1;

};