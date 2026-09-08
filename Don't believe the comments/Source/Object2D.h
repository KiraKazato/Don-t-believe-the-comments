#pragma once
#include "DxLib.h"
#include <string>

class Texture;
class TextureAnimation;

class Object2D
{
public:

	enum Tag
	{

		Player2D=2000,
		Enemy2D=2100,
		Bullet2D=2200,
		Back2D =2300,
		Whale2D=2400,
	};

public:
	Object2D(std::string filename, VECTOR initPos);
	Object2D(std::string filename, VECTOR initPos, int allNum, int xNum, int yNum, int interval);
	virtual ~Object2D();
	virtual void Update();
	virtual void Draw();
public:
	void SetPosition(VECTOR pos) { mvPosition = pos; }
	VECTOR GetPosition() { return mvPosition; }
	
	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }
		
	void SetTag(Tag tag) { mnTag = tag; }
	Tag GetTag() { return mnTag; }

	float GetRadius();
	
protected:
	Texture* mpTexture;
	TextureAnimation* mpTextureAnimation;
	VECTOR mvPosition;

private:
	bool mbDeleteFlag;
	Tag mnTag;

};
