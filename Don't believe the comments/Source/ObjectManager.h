#pragma once
#include <list>
#include<vector>
#include "Object2D.h"


class ObjectManager
{
public:
	ObjectManager();

	~ObjectManager();

	void Update();

	void Draw();

public:
	//追加
	void AddObject(Object2D* object2D);;
	//全削除
	void DeleteAll2D();
	//削除する必要があるオブジェクトを取得
	void DeleteAll2DIfNeeded();

	//指定したタグの2Dオブジェクトを取得
	Object2D* GetObject2DByTag(Object2D::Tag tag);

	//指定したタグの2Dオブジェクトのリストを取得
	std::vector<Object2D*>GetObject2DListByTag(Object2D::Tag tag);


public:
	std::list<Object2D*>mObject2DList;//2Dオブジェクトを管理するリスト

};