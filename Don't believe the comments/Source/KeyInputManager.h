#pragma once

class KeyInputManager
{
public:
	void Update();
	void Draw();

	//@return 押された瞬間か
	bool	KeyInputDown	(int _key) const;
	//@return 押されているフレーム数
	int		KeyInputPress	(int _key) const;
	//@return 離されているフレーム数
	int		KeyInputLeave	(int _key) const;
	//@return 離された瞬間か
	bool	KeyInputUp		(int _key) const;

private:
	int mnKeyBuffer[256] = { 0 };
};
