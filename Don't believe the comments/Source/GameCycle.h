#pragma once
#include "DxLib.h"
#include "Master.h"
#include "QuestionManager.h"
#include "TextInputManager.h"

class GameCycle
{
public:
	GameCycle();
	~GameCycle();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void SetStage(StageNumber _stageNumeber);
private:
	void CommentDraw();//コメントの表示
	
	void QuestionNumberDraw();//何問目かの表示

	void QuestionDraw();//問題文の表示
	
	void InputAnswer();//回答の入力
	
	bool AnswerJudge();//回答の判定

	void GameEnd();
private:
	//入力する文字のフォントハンドルを入れる
	int mnInputStringHandle = -1;
private:
	//今の問題画像を入れる
	int mnNowQuestionImageHandle = -1;
	//問題の画像の表示位置
	int mnQuestionImageX1 = Master::gridWidth * 20;
	int mnQuestionImageX2 = Master::gridWidth * 70;
	int mnQuestionImageY1 = Master::gridHeight * 22;
	int mnQuestionImageY2 = Master::gridHeight * 63;
private:
	//問題番号
	//最初は一問目なので1に設定
	int mnQuestionNumber = 1;
	//最大問題数
	int mnMaxQuestionNumber = 10;
	
private:
	bool mbQuestionNumberDrawn = false;
	bool mbQuestionDrawn = false;
	bool mbInputAnswered = false;
	bool mbQuestionSpawned = false;
private:
	std::string answer{};
private:
	TextInputManager mTextInputManager;
	StageNumber stageNumber = STAGE_1;
};