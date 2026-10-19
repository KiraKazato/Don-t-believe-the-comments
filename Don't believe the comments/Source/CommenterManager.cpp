#include "CommenterManager.h"
#include"Commenter.h"
#include "Master.h"
#include <iostream>
#include <fstream>
#include <sstream>

CommenterManager::CommenterManager():mnBaseCommentGridPositionX(89), mnBaseCommentGridPositionY(7)
{

}

CommenterManager::~CommenterManager()
{
}

void CommenterManager::Initialize()
{
	LoadTemplateComment("Resource/TemplateComment/Comment.csv");
	LoadCommenterName("");
	while (!mCommenterNames.empty())
	{
		GenerateCommenter();
	}
	mCommenterNames.clear();
}
void CommenterManager::Finalize()
{
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
	//DisplayCommenterは多分、Commenterをいれてるからdeleteしなくていい。
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
		printfDx("コメントが読み込めませんでした");
		return;
	}

	while (getline(ifs, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		mTemplateComments.push_back(line);
	}
}

void CommenterManager::LoadCommenterName(const std::string& _filepath)
{
	mCommenterNames.push_back("あ");
	mCommenterNames.push_back("べ");
	mCommenterNames.push_back("し");
	mCommenterNames.push_back("ん");
	mCommenterNames.push_back("ぞ");
	mCommenterNames.push_back("う");
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
	if (mCommenters.empty() || mDisplayCommenters.size() > 9)
	{
		return;
	}
	static int  ID = 0;
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
		new Commenter(commenter->GetName(), commenter->GetState(), comment, ID);
		ID++;
		break;
	}

}

std::string CommenterManager::GetDisplayComment(int _state)
{
	
	int index{};

	float rate = GetRand(10000) / 10000.0f;

	if (rate < 0.6f && !mTemplateComments.empty())
	{
		index = GetRand(static_cast<int>(mTemplateComments.size() - 1));
		return mTemplateComments[index];
	}

	if (_state == 0)
	{
		auto comment = Master::mpQuestionManager->GetQuestionData().trueComment;
		if (!comment.empty())
		{
			index = GetRand(static_cast<int>(comment.size() - 1));
			return comment[index];
		}
	}
	else if (_state == 1)
	{
		auto comment = Master::mpQuestionManager->GetQuestionData().falseComment;
		if (!comment.empty())
		{
			index = GetRand(static_cast<int>(comment.size() - 1));
			return comment[index];
		}
	}


	return "やっほー";
}

void CommenterManager::Update()
{
	//ここは途中
	//一秒たったらの処理多分後でcpp作るかも
	ChooseDisplayCommenter();
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
	mCommenters.push_back(_commenter);
}

void CommenterManager::SetDisplayCommenter(Commenter* _commenter)
{
	mDisplayCommenters.push_back(_commenter);
}

void CommenterManager::GetBasePosition(int* _x, int* _y)
{
	*_x = mnBaseCommentGridPositionX;
	*_y = mnBaseCommentGridPositionY;
}
