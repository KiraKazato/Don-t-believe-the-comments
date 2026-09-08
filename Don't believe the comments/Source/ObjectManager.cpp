#include "ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Update()
{
	//オブジェクトの更新
	//リストの先頭・末尾の要素へのイテレーター（std::listの要素のことを指す)を取得する
	//リストの利点...要素と要素の間に新しい要素を差し込むことが容易にできる
	//std::list...双方向連結リスト(要素の前後の要素にアクセスすることが簡単)
	for (std::list < Object2D*>::iterator itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}
	
}

void ObjectManager::Draw()
{
	//auto..=より右側の型を推測してくれる
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Draw();

	}
	
}

void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		Object2D* temp = *itr;

		itr = mObject2DList.erase(itr);

		delete temp;
		temp = nullptr;
	}
}

void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		Object2D* obj = *itr;

		if (obj != nullptr && obj->IsDeleteFlag())
		{
			itr = mObject2DList.erase(itr);
			delete obj;
			obj = nullptr;
		}
		else
		{
			++itr;
		}
	}
}

Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag tag)
{
	auto itr = std::find_if(mObject2DList.begin(), mObject2DList.end(), [&](Object2D* obj) {return obj->GetTag() == tag; }//ラムダ式
	//[&]...今回の場合、	mobject2DListの要素を[参照]するという意味合い
	// (Object2D*obj)...参照したオブジェクトの型と引数名
	// {...}..処理内容(今回は条件式)
	// [](){...}1この形がラムダ式の基本
	);

	if (itr != mObject2DList.end())
	{
		return(*itr);
	}

	return nullptr;
}

std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag tag)
{
	std::vector<Object2D*>ret;;

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back((*itr));
		}
	}

	return ret;
}

