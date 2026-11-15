#ifndef ITEM_1_H_
#define ITEM_1_H_

#include "Item.h"
#include "Image.h"
#include "Screen.h"
#include "MyMath.h"
#include <cmath>

// アイテム1クラス プレイヤを敵弾から守り身代わりになるバリアアイテム
class Item1 : public Item
{
public:
    int life; // バリアー(星)の耐える回数
    float xForce = 0.1f; // 加速の力(流れ星の加速)
    enum class State
    {
        Popping, // ポップ(飛び出した勢いのあるあいだ)
        Shooting,// 流れ星(加速して流れていく)
        Barrier, // バリアモードのとき
    };
    State state = State::Popping; // 動作状態
    GameObject* pParent{ nullptr }; // 親となっているプレイヤなどへのリンク(バリアの中心点)
    float barrierRadius; // バリアの円回転の半径
    float barrierAngle = 0.0f; // バリアの回転角度
    bool isHiting = false; // 敵弾にヒットした時に一瞬拡大して表示

    // コンストラクタ
    Item1(Vector3 pos,
        Vector3 v = Vector3{ -0.5f,0.0f }, // デフォルトは-0.5fで遅めのスクロール量
        int life = 3, float xForce = 0.8f, float barrierRadius = 40.0f)
        : life{ life }, xForce{ xForce }, barrierRadius{ barrierRadius },
        Item(pos, v, 8)
    {
        this->tag = "Item1"; // オブジェクトの判別タグ
    }

    // 更新処理
    virtual void Update() override
    {
        if (state == State::Popping) // 飛び出した勢いは段々減る
        {
            position += v; // vのx,y量ぶん移動
            v *= 0.85f; // 速度が減衰
            if (v.magnitude() < 0.01f)
                state = State::Shooting; // 流れ星モードに移行
        }
        else if (state == State::Shooting)
        {
            position += v; // vのx,y量ぶん移動
            v.x -= xForce; // 流れ星が少しずつ加速する
        }
        else if (state == State::Barrier && pParent != nullptr)
        {
            barrierAngle += 5.5f; // 毎フレーム5.5度ずつ回転
            position.x = pParent->position.x + barrierRadius * std::cos(barrierAngle * MyMath::Deg2Rad);
            position.y = pParent->position.y + barrierRadius * std::sin(barrierAngle * MyMath::Deg2Rad);

            if (pParent->isDead)
                isDead = true; // 親のプレイヤリンク先がisDeadなら連動して消す
        }

        if (state != State::Barrier) // バリア状態でないときに画面外に出たら消す
        {
            // スクリーンの画面外になったらisDeadにして消す
            if (position.x + collisionRadius < 0 || Screen::Width < position.x - collisionRadius
                || position.y + collisionRadius < 0 || Screen::Height < position.y - collisionRadius)
            {
                isDead = true;
            }
        }
    }

    // 描画処理
    virtual void Draw() override
    {
        DrawRotaGraphF(position.x, position.y, (isHiting) ? 2.0f : 1, 0, Image::item1, TRUE);
    }

    // 自機に当たったときの処理 上書きoverrideする
    virtual void OnCollisionPlayer(std::shared_ptr<GameObject> object) override;

    // 敵弾とぶつかった時の処理
    virtual void OnCollisionEnemyBullet(std::shared_ptr<GameObject> object) override
    {
        if (!((int)collisionTo & (int)CollisionTo::EnemyBullet))
            return; // 敵弾と当たり判定しない設定のときは何もせずreturn

        if (life <= 0) // ライフ(バリアの耐久回数)が0以下になったら
        {
            isDead = true; // 当たった直後に消える
            isHiting = true; // 一瞬拡大して表示
        }

        --life; // ライフ(バリアの耐久回数)を減らす
    }
};

#endif


