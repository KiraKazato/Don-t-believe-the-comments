#pragma once
#include "DxLib.h"
#include <vector>

struct UIData
{
    int handle;
    float x1, y1;
    float x2, y2;
};

class UIManager {
public:

    // UIデータを新しく登録する関数
    static void AddUI(int _handle, float _x1, float _y1, float _x2, float _y2, float _ratio);

    // 登録されているすべてのUIを一括で計算・描画する関数
    static void DrawUI();

    // データをすべてクリアする関数（シーン切り替え時など）
    static void ClearUI();

private:
    // 複数のUIデータをまとめて保管しておくリスト（配列のようなもの）
    static std::vector<UIData> uiList;
};