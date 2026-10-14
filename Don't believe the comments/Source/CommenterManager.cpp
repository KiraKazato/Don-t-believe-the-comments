#include "CommenterManager.h"
#include"Commenter.h"
#include "Master.h"

CommenterManager::CommenterManager()
{
}

CommenterManager::~CommenterManager()
{
}

void CommenterManager::Initialize()
{
	LoadTemplateComment();
	LoadCommenterName();
	while (!mCommenterNames.empty())
	{
		GenerateCommenter();
	}
	mCommenterNames.clear();
}

void CommenterManager::LoadTemplateComment()
{

}

void CommenterManager::LoadCommenterName()
{
	mCommenterNames;
}

void CommenterManager::GenerateCommenter()
{
	static int  ID = 0;
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
		new Commenter(name, 0, "", ID);
	}
	else
	{
		for (const auto& commenter : mCommenters)
		{
			if (commenter->GetName() == name)
			{
				return;
			}
		}
		int state{};
		state = (mCommenters.back()->GetState() == 0) ? 1 : 0;
		ID++;
		new Commenter(name, state, "", ID);
	}
}
void CommenterManager::Update()
{
	//‚±‚±‚Í“r’†
	//ˆê•b‚½‚Á‚½‚ç‚Ìˆ—‘½•ªŒã‚Åcppì‚é‚©‚à
	if (0 && !mCommenters.empty())
	{
		isOneSecond = true;
	}
	if (isOneSecond)
	{
		float totalReputation{};
		for (const auto& commenter : mCommenters)
		{
			totalReputation;
		}
	}

}

void CommenterManager::Draw()

{

}

void CommenterManager::SetCommenter(Commenter* _commenter)
{
	mCommenters.push_back(_commenter);
}
