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
    void ChangeUISize(int _handle, float _ratio);

    //大きさを変更した画像を元に戻す
    void ResetUISize(int _handle);

    //指定UIの中に物体が入っているかの判定
    bool InsideUI(int _handle, float _objectX, float objexctY_);

     //�w��摜�����X�g����폜
     void EraseUI(int _handle);


    //指定UIの座標取得
    void GetUIPosition(int _handle, float* _x1, float* _y1, float* _x2, float* _y2);
    //指定uiの座標変更
    void SetUIPosition(int _handle, float _x1, float _y1, float _x2, float _y2);

     //指定画像の座標取得
     //int GetUIPosition()
     //指定画像の座標変更
    
private:
    //複数のUIデータをまとめて保管しておくリスト
    std::vector<UIData> mUIList;
};