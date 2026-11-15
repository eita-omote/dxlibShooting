#ifndef BOSS_H_
#define BOSS_H_

#include "Enemy.h"

#include "Image.h"

#include <cmath> // Sin Cosの計算に使う
#include "MyMath.h"
#include "MyRandom.h" // 乱数方向弾とバリアーの星の飛び出す方向の抽選に使う
#include "Item1.h"
#include "PlayerBullet.h"

// ボスクラス。Enemyを継承して作る
class Boss : public Enemy
{
public:
    // ボスの状態種別
    enum class State
    {
        Appear, // 登場
        Normal, // 通常時
        Swoon, // 気絶時
        Angry, // 発狂モード
        Dying, // 死亡
    };

    State state = State::Appear; // 現在の状態
    int swoonTime = 120; // 残り気絶時間
    int dyingTime = 180; // 死亡時のアニメーション時間

    int coolTime = 0; // クールタイム(冷却時間。0になるまで次の弾が撃てない)
    float sinMove = 0; // Sinの動きをする際の0～360度
    float sinRadius = 120; // Sinの動きをするときの半径

    bool isCritical = false; // 弾に当たった瞬間の yが ±2 以内のときクリティカルと判定
    Vector3 criticalPos{ 0,0,0 }; // クリティカルヒットした位置

    // コンストラクタ
    Boss(Vector3 pos) : Enemy(pos)
    {
        this->tag = "Boss"; // オブジェクトの種類判別タグ
        life = 100; // ライフ
        collisionRadius = 70; // 当たり判定半径
    }

    // 更新処理
    void Update() override
    {
        if (state == State::Appear) // 登場状態
        {
            position.x -= 1; // 左へ移動

            if (position.x <= 750) // x座標が750以下になったら
            {
                state = State::Normal; // 通常状態へ移行
            }
        }
        else if (state == State::Normal) // 通常状態
        {
            coolTime--;

            if (coolTime <= 0)
            {   // ボスの弾はスピード16で2倍の速度で撃ち出す
                gm.enemyBullets.emplace_back(std::make_shared<EnemyBullet>(position, 180 * MyMath::Deg2Rad, 16));
                coolTime += 200; // そのかわり発射間隔は長め
            }

            float prevSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;
            sinMove += 2.5f; // Sinの波運動の角度を進める
            float currentSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;

            // (前の角度でのY) - (今の角度でのY)の差を足すことで【ベースのyの位置 ± sinの動き】になる
            position.y += currentSinY - prevSinY; // 上下方向のSinの波運動
        }
        else if (state == State::Swoon) // 気絶状態
        {
            swoonTime--; // タイマー減少

            if (swoonTime <= 0) // タイマーが0になったら
            {
                coolTime = 0; // クールタイムを0にリセットしてから
                state = State::Angry; // 発狂モードへ
            }
        }
        else if (state == State::Angry) // 発狂モード
        {
            coolTime--;

            if (coolTime <= 0)
            {
                gm.enemyBullets.emplace_back(std::make_shared<EnemyBullet>(position, 180 * MyMath::Deg2Rad, 16));
                coolTime += 20;
            }

            float prevSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;
            sinMove += 10.5f; // Sinの波運動の角度を進める
            float currentSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;

            // (前の角度でのY) - (今の角度でのY)の差を足すことで【ベースのyの位置 ± sinの動き】になる
            position.y += currentSinY - prevSinY; // 上下方向のSinの波運動
        }
        else if (state == State::Dying) // 死亡中
        {
            dyingTime--; // タイマー減少

            if (dyingTime <= 0) // タイマーが0になったら
            {
                isDead = true; // 完全に消滅
            }
        }
    }

    // 描画処理
    void Draw() override
    {
        if (state == State::Appear || state == State::Normal) // 登場時と通常時
        {
            DrawRotaGraphF(position.x, position.y, 1, 0, Image::boss1, TRUE); // 普通の画像
        }
        else if (state == State::Swoon) // 気絶時
        {
            DrawRotaGraphF(position.x, position.y, 1, 0, Image::boss2, TRUE); // 気絶時の画像
        }
        else if (state == State::Angry) // 発狂モード
        {
            DrawRotaGraphF(position.x, position.y, 1, 0, Image::boss3, TRUE); // 怒ってる画像
        }
        else if (state == State::Dying) // 死亡中
        {
            DrawRotaGraphF(position.x, position.y, 1, 0, Image::boss2, TRUE); // 気絶時の画像
        }
        if (isCritical)
        {
            // 画面のスクリーンと同じサイズの大きな白い四角(フラッシュ)を描く
            DrawBox(0, 0, Screen::Width, Screen::Height, GetColor(200, 200, 200), TRUE);
            // 白フラッシュの上に黒丸を描くとクリティカルヒットした位置が目立つ(衝撃波っぽい)
            DrawCircle(position.x, position.y, 10, GetColor(0, 0, 0), TRUE);
            isCritical = false; // クリティカルフラグをfalseにすぐ戻す(フラッシュ点滅)
        }
    }

    // 自機弾に当たったときの処理をoverride（上書き）する
    virtual void OnCollisionPlayerBullet(std::shared_ptr<GameObject> playerBullet) override
    {
        // 登場時、気絶時、死亡時は被弾しても何もしない
        if (state == State::Appear || state == State::Swoon || state == State::Dying)
            return;

        life -= 1; // ライフを減らす

        float posY = position.y; // [ボス中心y]と[当たった瞬間の弾のy]が ±5 ならクリティカル
        if (posY - 5 <= playerBullet->position.y && playerBullet->position.y <= posY + 5)
        {
            isCritical = true; // クリティカルフラグをtrueに
            criticalPos = playerBullet->position; // クリティカルヒットした位置を記録する
            DropItem(); // バリアの星アイテムを落とす
        }
        if (life <= 0)
        {
            // ライフが無くなったら、すぐ消滅するのではなく、死亡状態へ移行
            state = State::Dying;
        }
        else if (state == State::Normal && life <= 50)
        {
            // 通常状態でライフが50以下になったら、気絶する
            state = State::Swoon;
        }
    }
    // アイテムを落とす処理
    void DropItem() override
    {
        if (MyRandom::Percent(100.0f)) // 100%の確率でバリアになる星をドロップ
        {
            float itemSpeed = 25.0f; // アイテムの飛び出すスピード
            float angle = MyRandom::Range(0.0f, 360.0f) * MyMath::Deg2Rad; // 飛び出す角度
            Vector3 v_Item{ std::cos(angle) * itemSpeed, std::sin(angle) * itemSpeed }; // 飛ぶ方向ベクトル
            int barrierLife = 3; // バリアは敵弾に3回当たるまでガードしてくれる
            gm.items.emplace_back(std::make_shared<Item1>(position, v_Item, barrierLife));
        }
    }
};

#endif