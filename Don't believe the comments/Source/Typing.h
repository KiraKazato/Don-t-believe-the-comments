#pragma once
#include <unordered_map>
#include <string>

class Typing
{
public:
	Typing();
	~Typing();

	const std::string& Dictionary(const std::string& _roma);

	bool IsRomaVowel(char _charactor);

private:

};