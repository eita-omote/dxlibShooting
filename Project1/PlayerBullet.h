#ifndef PLAYERBULLET_H_
#define PLAYERBULLET_H_

#include <cmath> //sin,cosを使うのに必要

#include "DxLib.h"
#include "Image.h"
#include "Screen.h"
#include "Vector3.h"

#include "GameObject.h"

// 自機弾クラス
class PlayerBullet : public GameObject
{
public:
   
    const float Speed = 25; // 移動速度
    const int VisibleRadius = 16; // 見た目の半径

 
    float collisionRadius = 16; // 当たり判定の半径
    float angle; // 移動角度


    // コンストラクタ
    PlayerBullet(Vector3 pos, float angle) : GameObject( pos )
    {
        this->tag = "PlayerBullet";
        this->angle = angle;
        this->v = Vector3((float)std::cos(angle) * Speed, (float)std::sin(angle) * Speed, 0);
    }

    // 更新処理
    void Update()
    {
        position += v;

        // 画面外に出たら、死亡フラグを立てる
        if (position.x + VisibleRadius < 0 || position.x - VisibleRadius > Screen::Width ||
            position.y + VisibleRadius < 0 || position.y - VisibleRadius > Screen::Height)
        {
            isDead = true;
        }
    }

    // 描画処理
    void Draw()
    {
        DrawRotaGraphF(position.x, position.y, 1, angle, Image::playerBullet, TRUE);
    }
    // 敵にぶつかった時の処理
    void OnCollisionEnemy(std::shared_ptr<GameObject> other)
    {
        isDead = true;
    }
};


#endif

