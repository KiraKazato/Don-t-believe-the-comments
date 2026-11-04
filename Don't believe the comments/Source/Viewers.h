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
	//³‰ğ‚µ‚½
	void ComeViewers();
	//•s³‰ğ‚¾‚Á‚½
	void LeaveViewers();
	//Å‰‚Ì‹’®Ò”Œˆ‚ß‚é
	void DecideViewers();
private:
	int mnCurrentViwers{};
	int mnTargetViwers{};
	int mnMaxDefauktViewers = 500;
	int mnMinDefauktViewers =300;
	int mnMaxAddValue = 600;
	int mnMinAddValue = 200;
	int mnMaxSubtractValue = 150;
	int mnMinSubtractValue = 50;
	int mnStringHandle{};
	int mnDrawX1{};
	int mnDrawY1;

};