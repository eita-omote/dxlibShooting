#ifndef ZAKO_2_H_
#define ZAKO_2_H_

#include "Enemy.h"
#include "Image.h"
#include "Screen.h"
#include <cmath> // Sin Cosの計算に使う
#include "MyMath.h"
#include "MyRandom.h"
#include "EnemyBullet.h" // make_sharedなど【実際の弾を撃ち生成処理を行うのでインクルード必要】

// ザコ2クラス
class Zako2 : public Enemy
{
public:
    int coolTime = 0; // クールタイム（冷却時間。0になるまで次の弾が撃てない）

    // コンストラクタ
    Zako2(Vector3 pos) : Enemy(pos)
    {
        this->tag = "Zako2";
    }

    // 更新処理
    void Update() override
    {
        if (position.x > Screen::Width - 200)
        { // スクリーンの端から200の位置で止まる
            position.x -= 1; // とりあえず左へ移動
        }

        coolTime--;

        if (coolTime <= 0)
        {
            float bulletAngle = MyRandom::Range(-15.0f, 15.0f); // ±15度の乱数の角度を取得
            gm.enemyBullets.emplace_back(std::make_shared<EnemyBullet>(position, (180 + bulletAngle) * MyMath::Deg2Rad, 8));
            coolTime += 30;
        }
    }

    // 描画処理
    void Draw() override
    {
        //DrawRotaGraphF(position.x, position.y, 1, 0, Image::zako2, TRUE);
        DxLib::MV1SetScale(Image::zako2, Vector3{ 0.25f, 0.25f, 0.25f }); // スケールを設定
        DxLib::MV1SetRotationXYZ(Image::zako2, VGet(0.0f, 3.1416 * -90 / 180, 0.0f)); // 回転角を指定
        DxLib::MV1SetPosition(Image::zako2, VGet(position.x, -position.y, position.z)); // モデルを三次元空間に配置
        DxLib::MV1DrawModel(Image::zako2); // 自機の描画
    }
};

#endif