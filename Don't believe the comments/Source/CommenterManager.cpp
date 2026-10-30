#include "CommenterManager.h"
#include"Commenter.h"
#include "Master.h"
#include <iostream>
#include <fstream>
#include <sstream>

CommenterManager::CommenterManager()
{

}

CommenterManager::~CommenterManager()
{
}

void CommenterManager::Initialize()
{
	LoadTemplateComment("Resource/TemplateComment/Comment.csv");
	LoadCommenterName("Resource/CommenterName/NameData.csv");
	while (!mCommenterNames.empty())
	{
		GenerateCommenter();
	}
	mCommenterNames.clear();
	mnCurrentCommentGridPositionX = mnBaseCommentGridPositionX;
	mnCurrentCommentGridPositionY = mnBaseCommentGridPositionY;
	
}
void CommenterManager::Finalize()
{
	if (!mDisplayCommenters.empty())
	{
		for (auto& commenter : mDisplayCommenters)
		{
			if (commenter != nullptr)
			{
				delete commenter;
			}
		}
	}
	if (!mCommenters.empty())
	{
		for (auto& commenter : mCommenters)
		{
			if (commenter != nullptr)
			{
				delete commenter;
			}
		}
	}
	
	mCommenters.clear();
	mDisplayCommenters.clear();
	mTemplateComments.clear();
}

void CommenterManager::LoadTemplateComment(const std::string& _filepath)
{
	std::ifstream ifs(_filepath, std::ios::in);
	std::string line;
	if (!ifs)
	{
		return;
	}

	mTemplateComments.resize(mnMaxCommentKind);

	while (getline(ifs, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		std::stringstream ss(line);
		std::string cell;

		std::string comment = "";
		int commentStateNumber = 0;

		if (std::getline(ss,cell,','))
		{
			comment = cell;
		}
		if (std::getline(ss,cell,','))
		{
			commentStateNumber = std::stoi(cell);
		}

		mTemplateComments.at(commentStateNumber).emplace_back(comment);
	}
}

void CommenterManager::LoadCommenterName(const std::string& _filepath)
{
	std::ifstream ifs(_filepath, std::ios::in);
	std::string line;

	if (!ifs)
	{
		
		return;
	}

	
	while (getline(ifs, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}
		mCommenterNames.push_back(line);
	}
}

void CommenterManager::GenerateCommenter()
{
	std::string name{};
	int randomNumber{};

	if (mCommenterNames.empty())
	{
		return;
	}

	randomNumber = GetRand((int)(mCommenterNames.size() - 1));
	name = mCommenterNames[randomNumber];
	mCommenterNames.erase(mCommenterNames.begin() + randomNumber);

	if (mCommenters.empty())
	{
		new Commenter(name, 0);
		return;
	}

	for (const auto& commenter : mCommenters)
	{
		if (commenter->GetName() == name)
		{
			return;
		}
	}
	int state{};
	state = (mCommenters.back()->GetState() == 0) ? 1 : 0;

	new Commenter(name, state);
}

void CommenterManager::ChooseDisplayCommenter()
{
	if (mCommenters.empty() || !IsOneSecond())
	{
		return;
	}
	
	float totalReputation{};
	float reputationPool{};
	std::string comment;
	for (const auto& commenter : mCommenters)
	{
		totalReputation += commenter->GetReputation();
	}
	reputationPool = GetRand(10000) / 10000.0f * totalReputation;
	for (const auto& commenter : mCommenters)
	{
		reputationPool -= commenter->GetReputation();
		if (reputationPool > 0.0f)
		{
			continue;
		}
		comment = GetDisplayComment(commenter->GetState());
		new Commenter(commenter,commenter->GetName(), commenter->GetState(), comment, mnID);

		int commentY = mnCurrentCommentGridPositionY + (commenter->GetTotalHeight() * (mnID+1));
		if (mnMaxCommentGridPositionY <= commentY)
		{
			int diff = commentY - mnMaxCommentGridPositionY;
			mnCurrentCommentGridPositionY -= diff;
		}
		mnID++;

		break;
	}

}

std::string CommenterManager::GetDisplayComment(int _state)
{
	int index{};
	int continued{};
	float rate = GetRand(10000) / 10000.0f;
	do
	{
		if (!mbQuestionStarted || (rate < 0.6f && !mTemplateComments.empty())|| continued > 5)
		{
			std::vector<std::string>&& chooseTemplateComment = ChooseTemplateCommentKind(mnCommentKind);
			index = GetRand(static_cast<int>(chooseTemplateComment.size() - 1));
			return chooseTemplateComment.at(index);
		}

		const auto comments = (_state == 0) ? Master::mpQuestionManager->GetQuestionData().trueComment : Master::mpQuestionManager->GetQuestionData().falseComment;

		if (comments.empty())
		{
			continued++;
			continue;
		}

		index = GetRand(static_cast<int>(comments.size() - 1));

		auto same = std::find(mUsedComments.begin(), mUsedComments.end(), comments[index]);

		if (same != mUsedComments.end())
		{
			continued++;
			continue;
		}

		mUsedComments.push_back(comments[index]);

		return comments[index];

	} while (true);
}

std::vector<std::string> CommenterManager::ChooseTemplateCommentKind(int _kind)
{
	std::vector<std::string>ret{};

	int inIndexMax = _kind;

	if (_kind == 0)
	{
		// 開始時は 0 番目だけ
		ret = mTemplateComments.at(0);
		return ret;
	}
	else if (_kind == 4)
	{
		// 終了時は 4 番目
		ret = mTemplateComments.at(_kind);
		return ret;
	}


	// 再確保処理をなくすため
	size_t capacity = 0;
	for (size_t i = inIndexMax; i >= 1; i--)
	{
		capacity += mTemplateComments.at(i).size();
	}

	// 自分以下の番号を集める
	ret.reserve(capacity);
	for (size_t i = inIndexMax; i >= 1; i--)
	{
		// 末尾に一括追加
		ret.insert(ret.end(), mTemplateComments.at(i).begin(), mTemplateComments.at(i).end());
	}

	return ret;
}


void CommenterManager::Update()
{
	ChooseDisplayCommenter();
	if (mDisplayCommenters.empty())return;
	int totalHeight{};
	for (const auto commenter : mDisplayCommenters)
	{
		commenter->updateReputation(totalHeight);
		totalHeight += commenter->GetTotalHeight();
	}
	
}

void CommenterManager::Draw()
{
	if (mDisplayCommenters.empty())return;

	for (const auto& commenter : mDisplayCommenters)
	{
		commenter->Draw();
	}
}

void CommenterManager::SetCommenter(Commenter* _commenter)
{
	mCommenters.emplace_back(_commenter);
}

void CommenterManager::SetDisplayCommenter(Commenter* _commenter)
{
	mDisplayCommenters.emplace_back(_commenter);
}

void CommenterManager::GetBasePosition(int* _x, int* _y)
{
	if (_x)
	{
		*_x = mnBaseCommentGridPositionX;
	}
	if (_y)
	{
		*_y = mnBaseCommentGridPositionY;
	}
}

void CommenterManager::GetCurrentPosition(int* _x, int* _y)
{
	if (_x)
	{
		*_x = mnCurrentCommentGridPositionX;
	}
	if (_y)
	{
		*_y = mnCurrentCommentGridPositionY;
	}
}



void CommenterManager::SetCurrentWidthPosition(int _x)
{
	mnCurrentCommentGridPositionX = _x;
}
void CommenterManager::SetCurrentHightPosition(int _y)
{
	mnCurrentCommentGridPositionY = _y;
}



void CommenterManager::QuestionStarted(bool _start)
{
	mbQuestionStarted = _start;
}

void CommenterManager::ChangeQuestion()
{
	mUsedComments.clear();
}

bool CommenterManager::IsOneSecond()
{
	//基準の時間
	static int lastTime = GetNowCount();
	int currentTime = GetNowCount();

	int diffTime = currentTime - lastTime;

	if (diffTime >= 2000)
	{
		lastTime = currentTime;
		return true;
	}

	return false;
}

void CommenterManager::GameEnd()
{
	if (!mDisplayCommenters.empty())
	{
		for (auto commenter : mDisplayCommenters)
		{
			delete commenter;
		}
		mDisplayCommenters.clear();
	}
	if (!mUsedComments.empty())
	{
		mUsedComments.clear();
	}
	if (!mCommenterNames.empty())
	{
		mCommenterNames.clear();
	}
	mnID = 0;
	mnCurrentCommentGridPositionX = mnBaseCommentGridPositionX;
	mnCurrentCommentGridPositionY = mnBaseCommentGridPositionY;
}
void CommenterManager::SetCommentSpawnKind(int _kind)
{
	mnCommentKind = _kind;
}
