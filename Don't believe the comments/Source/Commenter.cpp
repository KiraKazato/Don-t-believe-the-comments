#include "Commenter.h"

Commenter::Commenter(std::string _name, int _state, std::string _comment, int _number) :commenterName(_name), commenterState(_state), comment(_comment), commentNumber(_number)
{
	Master::mpCommenterManager->SetCommenter(this);
}

Commenter::~Commenter()
{

}

void Commenter::Draw()
{
}