#include "QuestionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "DxLib.h"

void QuestionManager::Intialize()
{
	Load("Data/Quiz.csv");
}

void QuestionManager::SpawnQuiz(const std::vector<int>& _difficulties, const std::vector<int>& _probability)
{
	int difficultIndex = 0;
	int questionIndex = 0;

	// 探索限界数
	static const int LOOP_MAX = 5;

	for (int i = 0; i < _difficulties.size(); i++)
	{
		if (_probability[i] > GetRand(100))
		{
			questionIndex = GetRand(static_cast<int>(mQuestions[_difficulties[i]].size()) - 1);
			difficultIndex = _difficulties[i];
			
			bool isFound = true;
			int loopCount = 0;
			while (isFound)
			{
				// 同じものがないか探索
				for (const auto& spawned : mSpawnedQuestion)
				{
					// もしも同じ問題があった場合
					if (questionIndex == spawned.first && difficultIndex == spawned.second)
					{
						// ループ（被った問題探し）を抜ける
						break;
					}
					// 被った問題がないのなら
					isFound = false;
				}
				// ループを抜ける
				if(!isFound)
				{
					break;
				}

				questionIndex = GetRand(static_cast<int>(mQuestions[_difficulties[i]].size()) - 1);
				loopCount++;
				// 限界まで探しても見つからない場合
				if (LOOP_MAX <= loopCount)
				{
					loopCount = 0;
					// 入れる難易度を変える
					if (i < _difficulties.size())
					{
						i++;
						difficultIndex = _difficulties[i];
					}
					// もう一度探索
					isFound = true;
				}
			}

			// 出す問題をもう出した問題としてカウント
			mSpawnedQuestion.emplace_back
			(
				std::make_pair
				(
					questionIndex,
					_difficulties[i]
				)
			);
			
			break;
		}
	}

	// 問題を設定
	SetQuestion(questionIndex, difficultIndex);
}

void QuestionManager::Load(const std::string& _filePath)
{
	mbIsLoadFinish = false;

	// 入れるためのデータを用意する
	QuestionData inData = QuestionData();

	std::ifstream ifs(_filePath, std::ios::in);

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

			// 難易度数値が出るごとに大きくしていく
			if (mQuestions.size() < stoi(cell))
			{
				mQuestions.resize(stoi(cell));
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
		mQuestions[inData.difficulty].emplace_back(inData);
	}
	mbIsLoadFinish = true;
}

const std::vector<std::string> QuestionManager::LoadComment(std::stringstream& _ss, std::string& _cell)
{
	std::vector<std::string> ret;

	// } で区切る
	while (getline(_ss, _cell, '}'))
	{
		size_t pos = 0;

		// 処理をする上で {～ というようになっているため削除
		while ((pos = _cell.find("{")) != std::string::npos)
		{
			_cell.replace(pos, 1, "");
		}

		// 入れる
		ret.emplace_back(_cell);
	}
	return ret;
}

void QuestionManager::SetQuestion(int _index, int _difficulty)
{
	mnCurrentIndex = _index;
	mnCurrentDifficulty = _difficulty;
}

const QuestionData& QuestionManager::GetQuestionData()
{
	// mnCurrentIndexがエラー部分を参照したらエラーデータを返す
	try
	{
		return mQuestions[mnCurrentDifficulty][mnCurrentIndex];
	}
	catch (...)
	{
		static const QuestionData ErrorData = QuestionData("エラー", "エラー", INT_MAX, { "エラー" }, { "エラー" });
		return  ErrorData;
	}
}
