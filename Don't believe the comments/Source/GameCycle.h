#pragma once
#include "DxLib.h"
#include "QuestionManager.h"
class GameCycle
{
public:
	GameCycle();
	~GameCycle();
	void Initialize();
	void Update();
	void Finalize();

private:
	void CommentDraw();//コメントの表示
	
	bool QuestionNumberDraw();//何問目かの表示

	bool QuestinDraw();//問題文の表示
	
	void InputAnswer();//回答の入力
	
	bool AnswerJudge();//回答の判定

private:
	int questionNumberStringHandle = 0;
};