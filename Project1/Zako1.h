#ifndef ZAKO_1_H_
#define ZAKO_1_H_

#include "Enemy.h"

#include <cmath> // Sinの計算に使う
#include "MyMath.h"
#include "Image.h"

#include "MyRandom.h" // 乱数でアイテムのドロップと飛ぶ角度を抽選する
#include "Item0.h"
// ザコ1クラス
class Zako1 : public Enemy
{
public:
    float sinMove = 0; // Sinの動きをする際の0～360度
    float sinRadius = 70; // Sinの動きをするときの半径

    // コンストラクタ
    Zako1(Vector3 pos) : Enemy(pos)
    {
        this->tag = "Zako1";//オブジェクトの種類判別タグ
        life = 5;
    }

    // 更新処理
    void Update() override
    {
        position.x -= 1; // 左へ移動

        float prevSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;
        sinMove += 2.5f; // Sinの波運動の角度を進める
        float currentSinY = std::sin(sinMove * MyMath::Deg2Rad) * sinRadius;

        // (前の角度でのY) - (今の角度でのY)の差を足すことで【ベースのyの位置 ± sinの動き】になる
        position.y += currentSinY - prevSinY; // 上下方向のSinの波運動
    }

    // 描画処理
    void Draw() override
    {
        //DrawRotaGraphF(position.x, position.y, 1, 0, Image::zako1, TRUE);
        DxLib::MV1SetScale(Image::zako1, Vector3{ 0.25f, 0.25f, 0.25f }); // スケールを設定
        DxLib::MV1SetRotationXYZ(Image::zako1, VGet(0.0f, 3.1416 * -90 / 180, 0.0f)); // 回転角を指定
        DxLib::MV1SetPosition(Image::zako1, VGet(position.x, -position.y, position.z)); // モデルを三次元空間に配置
        DxLib::MV1DrawModel(Image::zako1); // 自機の描画
    }
    // アイテムを落とす処理
    void DropItem() override
    {
        if (isDead) // 消えるまえに
        {
            if (MyRandom::Percent(1.0f)) // 50%の確率でアイテムをドロップ
            {
                float itemSpeed = 25.0f; // アイテムの飛び出すスピード
                float angle = MyRandom::Range(0.0f, 360.0f) * MyMath::Deg2Rad; // 飛び出す角度
                Vector3 v_Item{ std::cos(angle) * itemSpeed, std::sin(angle) * itemSpeed }; // 飛ぶ方向ベクトル
                int bounceNum = 10; // バウンド回数
                gm.items.emplace_back(std::make_shared<Item0>(position, v_Item, true, bounceNum));
            }
        }
    }
};

#endif