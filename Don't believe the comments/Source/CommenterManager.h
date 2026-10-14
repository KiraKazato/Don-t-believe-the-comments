#pragma once
#include "DxLib.h"
#include "Master.h"

class Commenter;

class CommenterManager
{
public:
	CommenterManager();
	~CommenterManager();

	void Initialize();

	void LoadTemplateComment();
	void LoadCommenterName();

	void Update();
	void Draw();

	void SetCommenter(Commenter* _commenter);
private:
	std::vector<Commenter*>mCommenters;

	std::vector<std::string>mComments;
};