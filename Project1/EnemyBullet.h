#ifndef ENEMYBULLET_H_
#define ENEMYBULLET_H_

#include <cmath> //sin,cosを使うのに必要

#include "DxLib.h"
#include "Image.h"
#include "Screen.h"
#include "Vector3.h"

#include "GameObject.h"

// 敵弾クラス
class EnemyBullet : public GameObject
{
public:
    const int VisibleRadius = 8; // 見た目の半径
    float collisionRadius = 8; // 当たり判定半径

    float angle; // 移動角度(垂直Vertical)
    float angleH = 0; // 移動角度(水平Horizontal)


    // コンストラクタ
    EnemyBullet(Vector3 pos, float angle, float speed) : GameObject(pos)
    {
        this->angle = angle;

        // 角度からx方向の移動速度を算出
        v.x = (float)std::cos(angle) * speed;
        // 角度からy方向の移動速度を算出
        v.y = (float)std::sin(angle) * speed;
    }

    // コンストラクタ
    EnemyBullet(Vector3 pos, float angleV, float angleH, float speed) : GameObject(pos)
    {
        this->angle = angleV;
        this->angleH = angleH;

        // [3Dの角度は水平・垂直に分割]https://teratail.com/questions/126004
        float cosAngleV = (float)std::cos(angle);
        // 角度からx方向の移動速度を算出
        v.x = cosAngleV * (float)std::sin(angleH) * speed;
        // 角度からy方向の移動速度を算出
        v.y = (float)std::sin(angle) * speed;
        // 角度からz方向の移動速度を算出
        v.z = cosAngleV * (float)std::cos(angleH) * speed;
    }

    // 更新処理
    void Update()
    {
        // 速度の分だけ移動
        position += v;

        // 画面外に出たら死亡フラグを立てる
        if (position.y + VisibleRadius < 0 || position.y - VisibleRadius > Screen::Height ||
            position.x + VisibleRadius < 0 || position.x - VisibleRadius > Screen::Width)
        {
            isDead = true;
        }
    }

    // 描画処理
    void Draw()
    {
        DrawRotaGraphF(position.x, position.y, 1, angle, Image::enemyBullet16, TRUE);
    }

    // プレイヤにぶつかった時の処理
    virtual void OnCollisionPlayer(std::shared_ptr<GameObject> other)
    {
        isDead = true;
    }
    // アイテム(プレイヤの回りをまわるバリア)にぶつかった時の処理
    void OnCollisionItem(std::shared_ptr<GameObject> other)
    {
        isDead = true;
    }
};

#endif