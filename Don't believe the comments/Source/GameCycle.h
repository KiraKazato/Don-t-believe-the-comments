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
	int inputStringHandle = -1;
private:
	//今の問題画像を入れる
	int nowQuestionImageHandle = -1;
	//問題の画像の表示位置
	int QuestionImageX1 = Master::gridWidth * 18;
	int QuestionImageX2 = Master::gridWidth * 72;
	int QuestionImageY1 = Master::gridHeight * 12;
	int QuestionImageY2 = Master::gridHeight * 73;
private:
	//問題番号
	//最初は一問目なので1に設定
	int questionNumber = 1;
	//最大問題数
	int maxQuestionNumber = 10;
	
private:
	bool isQuestionNumberDraw = false;
	bool isQuestionDraw = false;
	bool isInputAnswer = false;
	bool QuestionSpawned = false;
private:
	std::string answer{};
private:
	TextInputManager mTextInputManager;
	StageNumber stageNumber = STAGE_1;
};