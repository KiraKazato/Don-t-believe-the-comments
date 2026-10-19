#pragma once
#include <string>
#include "Master.h"

class Commenter
{
public:
	//名前と評価とステータスだけ
	Commenter(std::string _name, int _state);
	//コメントが決まったら
	Commenter(std::string _name, int _state, std::string _comment, int _number);
	~Commenter();

	void Draw();


	int GetState() const { return mnCommenterState; }
	float GetReputation()const { return mfCommenterReputation; }
	std::string GetName() const { return msCommenterName; }
private:
	//コメンターの名前
	std::string msCommenterName{};

	//0なら真のことを言う
	//1なら偽のことを言う
	int mnCommenterState = 0;

	//コメントを持たせるやつ
	std::string msComment{};

	//コメンターの評価(出やすさ)
	//%だと考えて、初期値は1
	float mfCommenterReputation = 1.00f;

	//何個目のコメントか
	int mnCommentNumber;

	//コメントのフォント
	int mnCommentFontHandle;
};