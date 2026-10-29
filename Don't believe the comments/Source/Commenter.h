#pragma once
#include <string>
#include "Master.h"


struct BasePosition
{
	float baseX1;
	float baseY1;
	float baseX2;
	float baseY2;
};

struct ButtonData
{
	int handle;
	BasePosition position;
	//押されたか
	bool pressed;
};

class Commenter
{
public:
	//名前と評価とステータスだけ
	Commenter(std::string _name, int _state);
	//コメントが決まったら
	Commenter(std::string _name, int _state, std::string _comment, int _number);
	~Commenter();

	void Draw(int _currentHeight);


	int GetState() const { return mnCommenterState; }
	float GetReputation()const { return mfCommenterReputation; }
	std::string GetName() const { return msCommenterName; }

	//名前とコメントと空白の合計を返す
	int GetTotalHeight() { return totalHeightSpace; }

	//コメントの横幅によって改行する
	//表示する前に呼ぶ
	std::string CheckLineBreak(std::string _text, int _fontHandle);

	//評価ボタンのdraw
	void DrawReputatoionGraph(int _currentHeight);
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
	ButtonData mGoodButton{};
	ButtonData mBadButton{};

	//何個目のコメントか
	int mnCommentNumber{};

	//コメントのフォント
	int mnCommentFontHandle{};
	//名前のフォント
	int mnNameFontHandle{};

	//名前文の空白
	int mnNameHeight = 3;
	//名前とコメントと空白を合わせた高さ
	int totalHeightSpace = 9;
};