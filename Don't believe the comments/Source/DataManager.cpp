#include "DataManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "DxLib.h"

void DataManager::Load(const string& filePath)
{
	mbIsLoadFinish = false;

	// 入れるためのデータを用意する
	QuestionData inData = QuestionData();

	ifstream ifs(filePath, ios::in);

	if (!ifs)
	{
		printfDx("クイズが読み込めませんでした");
		return;
	}

	string line;

	// 最初の行は項目名のため飛ばす
	getline(ifs, line);

	if (getline(ifs, line))
	{
		size_t pos = 0;

		// 行の最後は\n\rとなるため、\rを消す
		while ((pos = line.find("\r")) != std::string::npos)
		{
			line.replace(pos, 2, "");
		}

		stringstream ss(line);
		string cell;

		// 問題
		if (getline(ss, cell, ',')) inData.question = cell;

		// 答え
		if (getline(ss, cell, ',')) inData.answer = cell;
		
		// 難易度
		if (getline(ss, cell, ',')) inData.difficulty = stoi(cell);

		// 真コメント
		if (getline(ss, cell, ','))
		{
			stringstream ssComent(cell);
			string cellComment;

			inData.trueComment = LoadComment(ssComent, cellComment);

		}

		// 偽コメント
		if (getline(ss, cell, ','))
		{
			stringstream ssComent(cell);
			string cellComment;

			inData.falseComment = LoadComment(ssComent, cellComment);
		}

		// 入れる
		mQuestions.emplace_back(inData);
	}
	mbIsLoadFinish = true;
}

const vector<string> DataManager::LoadComment(stringstream& ss, string& cell)
{
	vector<string> ret;

	// } で区切る
	while (getline(ss, cell, '}'))
	{
		size_t pos = 0;

		// 処理をする上で {～ というようになっているため削除
		while ((pos = cell.find("{")) != std::string::npos)
		{
			cell.replace(pos, 1, "");
		}

		// 入れる
		ret.emplace_back(cell);
	}

	return ret;
}

void DataManager::SetQuestion(int index)
{
	mnCurrentIndex = index;
}

const QuestionData& DataManager::GetQuestionData()
{
	return mQuestions[mnCurrentIndex];
}
