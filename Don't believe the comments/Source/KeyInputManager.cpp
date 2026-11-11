#include "KeyInputManager.h"
#include "DxLib.h"

void KeyInputManager::Update()
{
    GetHitKeyStateAllEx(mnKeyBuffer);
}

void KeyInputManager::Draw()
{
    //v‚¢‚Â‚¢‚½‚ç‘‚­
}

bool KeyInputManager::KeyInputDown(int _key) const
{
    return KeyInputPress(_key) == 1;
}

int KeyInputManager::KeyInputPress(int _key) const
{
    return mnKeyBuffer[_key] > 0 ? mnKeyBuffer[_key] : 0;
}

int KeyInputManager::KeyInputLeave(int _key) const
{
    return mnKeyBuffer[_key] < 0 ? mnKeyBuffer[_key] : 0;
}

bool KeyInputManager::KeyInputUp(int _key) const
{
    return KeyInputLeave(_key) == -1;
}
