#pragma once
#include <string>
#include "Master.h"

class Commenter
{
	Commenter(std::string _name, int _state, std::string _comment, int _number);
	~Commenter();

	void Draw();
private:
	//コメンターの名前
	std::string commenterName{};

	//0なら真のことを言う
	//1なら偽のことを言う
	int commenterState = 0;

	//コメントを持たせるやつ
	std::string comment{};

	//コメンターの評価(出やすさ)
	//%だと考えて、初期値は1
	float commenterReputation = 1.00f;

	//何個目のコメントか
	int commentNumber;
};