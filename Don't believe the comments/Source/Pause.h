#pragma once

class Pause
{
public:
	Pause();
	~Pause();
	void Update();
	void Draw();
	void StartPause();
	bool GetStartPause();
private:
	bool mbPause = false;
};