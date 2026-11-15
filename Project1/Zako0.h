#ifndef ZAKO_0_H_
#define ZAKO_0_H_

#include "Enemy.h"
#include "Image.h"
#include "MyMath.h"
#include "EnemyBullet.h" // make_sharedなど【実際の弾を撃ち生成処理を行うのでインクルード必要】
// ザコ0クラス
class Zako0 : public Enemy
{
public:
    int counter = 0;
    // コンストラクタ
    Zako0(Vector3 pos)
        : Enemy(pos)
    {
        this->tag = "Zako0";
    }

    // 更新処理
    void Update() override
    {
        position.x -= 1; // とりあえず左へ移動
        counter++;

        if (counter % 10 == 0)
        {
            gm.enemyBullets.emplace_back(std::make_shared<EnemyBullet>(position, 180 * MyMath::Deg2Rad, 8));
        }
    }

    // 描画処理
    void Draw() override
    {
        //DrawRotaGraphF(position.x, position.y, 1, 0, Image::zako0, TRUE);
        DxLib::MV1SetScale(Image::zako0, Vector3{ 0.25f, 0.25f, 0.25f }); // スケールを設定
        DxLib::MV1SetRotationXYZ(Image::zako0, VGet(0.0f, 3.1416 * -90 / 180, 0.0f)); // 回転角を指定
        DxLib::MV1SetPosition(Image::zako0, VGet(position.x, -position.y, position.z)); // モデルを三次元空間に配置
        DxLib::MV1DrawModel(Image::zako0); // 自機の描画
    }
};

#endif
