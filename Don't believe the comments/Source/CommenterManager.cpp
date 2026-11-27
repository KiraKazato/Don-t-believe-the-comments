#include "CommenterManager.h"
#include"Commenter.h"
#include "Master.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "EffekseerEffect.h"

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

		mpSuperChatEffect = new EffekseerEffect("Resource/Effect/SuperChatEffect.efk", 50.0f);
	mpSuperChatEffect->SetManualDraw(true);

		mnEffectScreen = MakeScreen(Master::Width, Master::Height, FALSE);
	mnCommentBackHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/CommentBack.png");
}

void CommenterManager::Finalize()
{
		if (mpSuperChatEffect != nullptr)
	{
		delete mpSuperChatEffect;
		mpSuperChatEffect = nullptr;
	}
	if (mnEffectScreen != -1)
	{
		DeleteGraph(mnEffectScreen);
		mnEffectScreen = -1;
	}

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
		mCommenterNames.emplace_back(line);
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
	int totalHeight{};
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
		new Commenter(commenter, commenter->GetName(), commenter->GetState(), comment, mnID, mbIsSuperCommentDraw);
		if (mbIsSuperCommentDraw)
		{
			mbIsSuperCommentDraw = false;	// 一回出したら次を指示されるまでもう出さない
		}

		for (const auto& displayCommenter : mDisplayCommenters)
		{
			totalHeight += displayCommenter->GetTotalHeight();
		}
		int commentY = mnCurrentCommentGridPositionY + totalHeight;
		if (mnMaxCommentGridPositionY <= commentY)
		{
			int diff = commentY - mnMaxCommentGridPositionY;
			
			if (!mbIsScrolled)
			{
				mnCurrentCommentGridPositionY -= diff;
			}
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
		if (mbIsSuperCommentDraw)
		{
			
			if (mpSuperChatEffect != nullptr)
			{
				mpSuperChatEffect->Play(
					static_cast<float>(Master::gridWidth * 103),
					static_cast<float>(Master::gridHeight * 53)
				);
			}

			
			const std::vector<std::string>& chooseSuperHintComment = Master::mpQuestionManager->GetQuestionData().superHintComment;
			index = GetRand(static_cast<int>(chooseSuperHintComment.size()) - 1);
			return chooseSuperHintComment.at(index);
		}

		//テンプレート
		if (!mbQuestionStarted || (rate < 0.7f && !mTemplateComments.empty())|| continued > 5)
		{
			std::vector<std::string>&& chooseTemplateComment = ChooseTemplateCommentKind(mnCommentKind);
			index = GetRand(static_cast<int>(chooseTemplateComment.size() - 1));
			return chooseTemplateComment.at(index);
		}

		//↓以後ヒント

		const auto& comments = (_state == 0) ? Master::mpQuestionManager->GetQuestionData().trueComment : Master::mpQuestionManager->GetQuestionData().falseComment;

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

		mUsedComments.emplace_back(comments[index]);

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

	if (mpSuperChatEffect != nullptr)
	{
		mpSuperChatEffect->Update();
	}
	ScrollComments();
	ChooseDisplayCommenter();
	
	if (mDisplayCommenters.empty())return;
	int drawTotalHeight{};
	for (const auto& commenter : mDisplayCommenters)
	{
		commenter->updateReputation(drawTotalHeight);
		drawTotalHeight += commenter->GetTotalHeight();
	}
	

}

void CommenterManager::Draw()
{

	
	for (const auto& commenter : mDisplayCommenters)
	{
		commenter->Draw();
	}
	bool isEffectPlaying = (mpSuperChatEffect != nullptr && mpSuperChatEffect->IsPlaying());

	if (isEffectPlaying && mnEffectScreen != -1)
	{
		GetDrawScreenGraph(0, 0, Master::Width, Master::Height, mnEffectScreen);

		SetDrawScreen(mnEffectScreen); 
		
		mpSuperChatEffect->Draw();     
		SetDrawScreen(DX_SCREEN_BACK); 
	}

	
	SetDrawArea(Master::gridWidth * 89, Master::gridHeight * 12, Master::gridWidth * 117, Master::gridHeight * 113);

	
	if (isEffectPlaying && mnEffectScreen != -1)
	{
		DrawGraph(0, 0, mnEffectScreen, FALSE);
	}



	
	SetDrawAreaFull();
	DrawBackScrollButton();
}

void CommenterManager::SetCommenter(Commenter* _commenter)
{
	mCommenters.emplace_back(_commenter);
}

void CommenterManager::SetDisplayCommenter(Commenter* _commenter)
{
	// データ上に残すコメントの数
	constexpr int saveComments = 1000;

	/**/if(mDisplayCommenters.size() > saveComments)
	{
		auto itr = mDisplayCommenters.begin();
		int deleteHeight = (*itr)->GetTotalHeight();
		mnCurrentCommentGridPositionY += deleteHeight;
		delete *itr;
		mDisplayCommenters.erase(itr);
	}
	mDisplayCommenters.emplace_back(_commenter);
	mnCountComments++;
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

void CommenterManager::SetIsSuperHintDraw(bool _flag)
{
	mbIsSuperCommentDraw = _flag;
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

	if (diffTime >= 1200)
	{
		lastTime = currentTime;
		return true;
	}

	return false;
}

void CommenterManager::GameEnd()
{
	
	if (mpSuperChatEffect != nullptr)
	{
		mpSuperChatEffect->Stop();
	}
	if (!mDisplayCommenters.empty())
	{
		for (auto& commenter : mDisplayCommenters)
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

void CommenterManager::ResetCommentCount()
{
	mnCountComments = 0;
}

void CommenterManager::ResetCommenterReputation()
{
	for (auto& commenter : mCommenters)
	{
		commenter->SetReputation(1.0f);
	}
}

void CommenterManager::ScrollComments()
{

	mnTotalHeight = 0;
	for (auto comment : mDisplayCommenters)
	{
		mnTotalHeight += comment->GetTotalHeight();
	/*	if (totalHeight > (mnCommentFrameInY2 - mnCommentFrameInY1))
		{
			break;
		}*/
	}
	
	if (mnTotalHeight < (mnCommentFrameInY2 - mnCommentFrameInY1))
	{
		return;
	}
	
	Master::mpMouseManager->GetMouseWheelVolume(&mnMouseWheelValue);
	if (mnMouseWheelValue > 0)
	{
		mnMouseWheelValue += 5;
	}
	else if(mnMouseWheelValue < 0)
	{
		mnMouseWheelValue -= 5;
	}
	
	mnCurrentCommentGridPositionY += mnMouseWheelValue;
	if (mnCurrentCommentGridPositionY > mnBaseCommentGridPositionY)
	{
		mnCurrentCommentGridPositionY = mnBaseCommentGridPositionY;
	}
	if (mnCurrentCommentGridPositionY < mnCommentFrameInY2 - mnTotalHeight)
	{
		mnCurrentCommentGridPositionY = mnCommentFrameInY2 - mnTotalHeight;
	}
	if (mnCurrentCommentGridPositionY > mnCommentFrameInY2 - mnTotalHeight)
	{
		mbIsScrolled = true;
	}
	else
	{
		mbIsScrolled = false;
		
	}
}

void CommenterManager::DrawBackScrollButton()
{
	if (mbIsScrolled)
	{
		int mouseX{}, mouseY{};
		bool pressed;
		Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);
		DrawExtendGraph(Master::gridWidth * 101, Master::gridHeight * 104, Master::gridWidth * 105, Master::gridHeight * 111, mnCommentBackHandle, TRUE);
		bool inside = Master::mpUIManager->InsideUI(Master::gridWidth * 101, Master::gridHeight * 104, Master::gridWidth * 105, Master::gridHeight * 111, mouseX, mouseY);
		if (Master::mpUIManager->ClickedUI(&pressed, inside))
		{
			mnCurrentCommentGridPositionY = mnCommentFrameInY2 - mnTotalHeight;
		}
	}
}
