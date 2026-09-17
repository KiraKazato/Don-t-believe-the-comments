#include "DataManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "DxLib.h"

void DataManager::Load(const std::string& filePath)
{
	mbIsLoadFinish = false;

	// 入れるためのデータを用意する
	QuestionData inData = QuestionData();

	std::ifstream ifs(filePath, std::ios::in);

	if (!ifs)
	{
		printfDx("クイズが読み込めませんでした");
		return;
	}

	std::string line;

	// 最初の行は項目名のため飛ばす
	getline(ifs, line);

	while (getline(ifs, line))
	{
		size_t pos = 0;

		// 行の最後は\n\rとなるため、\rを消す
		while ((pos = line.find("\r")) != std::string::npos)
		{
			line.replace(pos, 2, "");
		}

		std::stringstream ss(line);
		std::string cell;

		// 問題
		if (getline(ss, cell, ',')) inData.question = cell;

		// 答え
		if (getline(ss, cell, ',')) inData.answer = cell;
		
		// 難易度
		if (getline(ss, cell, ',')) 
		{
			// エラー発生時の処理
			try
			{
				inData.difficulty = stoi(cell); 
			}
			catch (...)
			{
				inData.difficulty = 0;
			}
			
		}

		// 真コメント
		if (getline(ss, cell, ','))
		{
			std::stringstream ssComent(cell);
			std::string cellComment;

			inData.trueComment = LoadComment(ssComent, cellComment);

		}

		// 偽コメント
		if (getline(ss, cell, ','))
		{
			std::stringstream ssComent(cell);
			std::string cellComment;

			inData.falseComment = LoadComment(ssComent, cellComment);
		}

		// 入れる
		mQuestions.emplace_back(inData);
	}
	mbIsLoadFinish = true;
}

const std::vector<std::string> DataManager::LoadComment(std::stringstream& ss, std::string& cell)
{
	std::vector<std::string> ret;

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
	// mnCurrentIndexがエラー部分を参照したらエラーデータを返す
	try
	{
		return mQuestions[mnCurrentIndex];
	}
	catch (...)
	{
		return QuestionData("エラー", "エラー", INT_MAX, { "エラー" }, { "エラー" });
	}
}
