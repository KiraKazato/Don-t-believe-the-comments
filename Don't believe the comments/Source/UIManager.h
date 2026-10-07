#pragma once
#include "DxLib.h"
#include <vector>

struct UIData
{
    //画像のhandleを入れる
    int handle;
    //もらった元の座標
    float originalX1, originalY1;
    float originalX2, originalY2;
    //表示する座標
    float drawX1, drawY1;
    float drawX2, drawY2;
};

class UIManager {
public:

    // UIデータを新しく登録する関数
     void AddUI(int _handle, float _x1, float _y1, float _x2, float _y2);

    // 登録されているすべてのUIを一括で計算・描画する関数
     void DrawUI();

    // データをすべてクリアする関数（シーン切り替え時など）
     void ClearUI();

     //その画像の大きさ変更
     void ChangeSize(int _handle, float _ratio);

     //大きさを変更した画像を元に戻す
     void ResetSize(int _handle);

     //指定UIの中に物体が入っているかの判定
     bool InsideUI(int _handle, int _objectX, int objexctY_);

    
private:
     //複数のUIデータをまとめて保管しておくリスト
     std::vector<UIData> mUIList;
};