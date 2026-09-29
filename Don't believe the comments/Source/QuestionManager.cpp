#include "QuestionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "DxLib.h"
#include "Master.h"

// {1,2,3} {4,5,6} {7.8}  {9,10}
// 減少大　減少中  増加大 増加中

void QuestionManager::Intialize()
{
	Load("Resource/QuizData/Quiz.csv");
	std::vector<GroupRate> initializeGroup =
	{
		GroupRate({1,{1,2,3},50,-5}),
		GroupRate({2,{4,5,6},30,-2}),
		GroupRate({3,{7,8},15,2}),
		GroupRate({4,{9,10},5,5})
	};
	SetQuestionRate(STAGE_1, initializeGroup);
}

void QuestionManager::Finalize()
{
	stageData.clear();
	ClearSpawnedQuestion();
}

void QuestionManager::SetQuestionRate(StageNumber _stage, std::vector<GroupRate> _group)
{
	stageData.push_back({ _stage,_group });
}

void QuestionManager::SpawnQuestion(StageNumber _number, int _questionNumber)
{
	//乱数をランダムに
	SRand((int)time(NULL));

	/*auto random = [](int _min, int _max) {return _min + GetRand(_max - _min); };
	auto isClampIn = [](int _value,int _min, int _max) {return _min < _value && _value < _max; };*/

	int setDifficultIndex = 0;
	int setQuestionIndex = 0;


	// 探索限界数
	static const int LOOP_MAX = 5;



	// stageData探索
	for (auto& data : stageData)
	{
		// ステージ番号を確認
		if (data.first != _number)
		{
			continue;
		}

		//確率の合計
		int totalRate = 0;
		//グループの確率の確定
		//問題数が進んでいたならここで確率の増減を適用する。
		for (auto& rate : data.second)
		{
			rate.rate += (_questionNumber - 1) * rate.changeRate;
			rate.rate = std::min(std::max(minRate, rate.rate), maxRate);
			totalRate += rate.rate;
		}

		int difficultGroupIndex = 0;
		for (auto& rate : data.second)
		{
			float normalizedRate = (float)rate.rate / totalRate;

			rate.rate = normalizedRate * 100;

		}
		for (auto& rate : data.second)
		{
			// 確率計算
			if (rate.rate > GetRand(100))
			{
				// 難易度
				setDifficultIndex = GetRand(static_cast<int>(rate.difficultgroup.size() - 1));
				difficultGroupIndex = setDifficultIndex;

				// 抽選
				setQuestionIndex = GetRand(static_cast<int>(mQuestions[setDifficultIndex].size()) - 1);


				bool isFound = true;
				// 何回ループしたか
				int loopCount = 0;
				// 既に出題した問題があったか
				while (isFound)
				{
					if (mSpawnedQuestion.empty())
					{
						isFound = false;
					}

					// 同じものがないか探索
					for (const auto& spawned : mSpawnedQuestion)
					{
						// もしも同じ問題があった場合
						if (setQuestionIndex == spawned.first && setDifficultIndex == spawned.second)
						{
							// ループ（被った問題探し）を抜ける
							break;
						}
						// 被った問題がないのなら
						isFound = false;
					}
					// ループを抜ける
					if (!isFound)
					{
						break;
					}

					// もう一度抽選
					setQuestionIndex = GetRand(static_cast<int>(mQuestions[setDifficultIndex].size()) - 1);
					loopCount++;
					// 限界まで探しても見つからない場合
					if (LOOP_MAX <= loopCount)
					{
						loopCount = 0;
						// 入れる難易度を変える
						if (setDifficultIndex < data.second.size())
						{
							setDifficultIndex++;
							if (setDifficultIndex < 0)
							{
								setDifficultIndex = static_cast<int>(data.second.size() - 1);
							}
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
						setQuestionIndex,
						setDifficultIndex
					)
				);
			}
			difficultGroupIndex++;
		}

	}

	// 問題を設定
	SetQuestion(setQuestionIndex, setDifficultIndex);
}

void QuestionManager::ClearSpawnedQuestion()
{
	mSpawnedQuestion.clear();
}

void QuestionManager::Load(const std::string& _filePath)
{
	mQuestions.resize(DIFFICULT_MAX);

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
		if (getline(ss, cell, ','))
		{
			int questionGraphID = -1;
			try
			{
				// 問題IDを得る
				questionGraphID = stoi(cell);
			}
			catch (...)
			{
				questionGraphID = -1;

				inData.questionGraphHandle = -1;
			}

			if (questionGraphID != -1)
				inData.questionGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/QuizImage/Quiz" + std::to_string(questionGraphID) + ".png");
		}

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
		mQuestions[inData.difficulty - 1].emplace_back(inData);
	}
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
		return mQuestions.at(mnCurrentDifficulty).at(mnCurrentIndex);
	}
	catch (...)
	{
		static const QuestionData ErrorData = QuestionData(-1, "NULL_ERROR", INT_MAX, { "これはエラー用コメントです" }, { "これはエラー用コメントです" });
		return  ErrorData;
	}
}
