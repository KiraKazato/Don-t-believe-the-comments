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
	//DisplayCommenterÇÕëΩï™ÅACommenterÇÇ¢ÇÍÇƒÇÈÇ©ÇÁdeleteÇµÇ»Ç≠ÇƒÇ¢Ç¢ÅB
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

	while (getline(ifs, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		std::stringstream ss(line);
		std::string cell;

		std::string comment = "";
		int commentNumber = 0;

		if (std::getline(ss,cell,','))
		{
			comment = cell;
		}
		if (std::getline(ss,cell,','))
		{
			commentNumber = std::stoi(cell);
		}

		mTemplateComments.emplace_back(comment, commentNumber);
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
		
		int commentY = mnBaseCommentGridPositionY + (commenter->GetTotalHeight() * (ID+1));
		if (mnMaxCommentGridPositionY <= commentY)
		{
			int diff = commentY - mnMaxCommentGridPositionY;
			mnBaseCommentGridPositionY -= diff;
		}
		ID++;

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
		if (continued > 5)
		{
			return "Ç‚Ç¡ÇŸÅ[";
		}
		if (!mbQuestionStarted || (rate < 0.6f && !mTemplateComments.empty()))
		{
			index = GetRand(static_cast<int>(mTemplateComments.size() - 1));
			return mTemplateComments[index].first;
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


void CommenterManager::Update()
{
	//Ç±Ç±ÇÕìríÜ
	//àÍïbÇΩÇ¡ÇΩÇÁÇÃèàóùëΩï™å„Ç≈cppçÏÇÈÇ©Ç‡
	ChooseDisplayCommenter();
}

void CommenterManager::Draw()
{
	if (mDisplayCommenters.empty())return;
	int totalHeight{};
	for (const auto& commenter : mDisplayCommenters)
	{
		commenter->Draw(totalHeight);
		totalHeight += commenter->GetTotalHeight();
	}
	DrawBox(Master::gridWidth*mnBaseCommentGridPositionX, Master::gridHeight*mnBaseCommentGridPositionY, Master::gridWidth*mnMaxCommentGridPositionX,Master::gridHeight* mnMaxCommentGridPositionY, GetColor(0, 0, 0), TRUE);
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
	if (_x)
	{
		*_x = mnBaseCommentGridPositionX;
	}
	if (_y)
	{
		*_y = mnBaseCommentGridPositionY;
	}
}

void CommenterManager::SetBaseWidthPosition(int _x)
{
	mnBaseCommentGridPositionX = _x;
}

void CommenterManager::SetBaseHeightPosition(int _y)
{
	mnBaseCommentGridPositionY = _y;
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
	//äÓèÄÇÃéûä‘
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
