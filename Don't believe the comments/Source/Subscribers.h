#pragma once
#include <array>

class Subscribers
{
public:
	Subscribers();
	~Subscribers();

	void Draw();
	void Update();

private:
	std::array<int, 2> mnNumberGraphHandle{ -1,-1 };

};