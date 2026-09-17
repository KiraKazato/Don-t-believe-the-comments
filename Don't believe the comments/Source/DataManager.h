#pragma once
#include <string>
#include <vector>
#include <sstream>

struct QuestionData
{
	std::string question = "";	// 問題
	std::string answer = "";		// 答え
	int difficulty = 0;		// 難易度
	std::vector<std::string> trueComment;			// 真コメント
	std::vector<std::string> falseComment;		// 偽コメント

	// コンストラクタ
	// データを作る際、明示的に作ることができる
	QuestionData(const std::string& question, const std::string& answer, int difficulty, const std::vector<std::string>& trueComment, const std::vector<std::string>& falseComment)
		: question(question), answer(answer), difficulty(difficulty), trueComment(trueComment), falseComment(falseComment)
	{
	}
	// 空データを作成可能
	// QuestionData(void)
	QuestionData(void) = default;
};

class DataManager
{
private:
	std::vector<QuestionData> mQuestions{};
	int mnCurrentIndex = 0;

public:
	// シングルトンパターン
	// DataManagerの入手
	static DataManager* GetInstance() {
		static DataManager* instance = new DataManager();
		return instance;
	}

public:
	// DataManagerにおいて再生成ができないようにする
	DataManager() = default;
	DataManager(const DataManager&) = default;
	DataManager& operator=(const DataManager&) = default;

	// 読み込み
	void Load(const std::string& filePath);

	

	// 問題番号を指定して問題を出す
	void SetQuestion(int index);
	
	// 現在の問題の情報を得る
	const QuestionData& GetQuestionData();

	// ロードが終わったか
	bool IsLoadFinish() const { return mbIsLoadFinish; };

private:
	// コメント読み込み
	// 特殊のため分ける　
	const std::vector<std::string> LoadComment(std::stringstream&, std::string&);


	bool mbIsLoadFinish = false;	// ロード処理を作る時に使えるはず
};

// DataManagerのインスタンスにアクセスできる省略形
#define DataInstance DataManager::GetInstance()