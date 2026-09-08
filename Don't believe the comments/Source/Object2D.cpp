#include "Object2D.h"
#include "Texture.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "TextureAnimation.h"

Object2D::Object2D(std::string filename, VECTOR initPos) :mvPosition(initPos), mbDeleteFlag(false),mpTextureAnimation(nullptr)
{
	//Œ»İƒV[ƒ“‚ÌObjectManager‚É©g(this)‚ğ’Ç‰Á‚·‚é
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	//‰æ‘œ¶¬
	mpTexture = new Texture(filename, initPos, true);
}

Object2D::Object2D(std::string filename, VECTOR initPos, int allNum, int xNum, int yNum, int interval):mvPosition(initPos), mbDeleteFlag(false),mpTexture(nullptr)
{
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	mpTextureAnimation = new TextureAnimation(initPos, filename, allNum, xNum, yNum, interval);
}

Object2D::~Object2D()
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}
	if (mpTextureAnimation != nullptr)
	{
		delete mpTextureAnimation;
	}
}

void Object2D::Update()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Update();
		mpTexture->SetPosition(mvPosition);
	}
	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Update();
		mpTextureAnimation->SetPosition(mvPosition);
	}


	
}

void Object2D::Draw()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Draw();
	}
	if (mpTextureAnimation != nullptr)
	{
		mpTextureAnimation->Draw();
	}
}

float Object2D::GetRadius()
{
	if (mpTexture != nullptr)
	{
		return mpTexture->GetRadius();
	}
	if (mpTextureAnimation != nullptr)
	{
		return mpTextureAnimation->GetRadius();
	}
	
	return 0.0f;
}
