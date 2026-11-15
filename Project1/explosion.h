#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Image.h"

#include "GameObject.h"

// 爆発エフェクトクラス
class Explosion : public GameObject
{
public:
    int counter = 0; // 時間を計るための変数
    int imageIndex = 0; // 表示すべき画像の番号

    // コンストラクタ
    Explosion(Vector3 pos) : GameObject(pos)
    {
        this->tag = "Explosion";
    }

    // 更新処理
    void Update() override
    {
        counter++;

        imageIndex = counter / 3; // そのままだと速すぎるので、3で割る

        // 絵は16枚しかない（0～15）ので、
        // 16番目を表示しようとしたら終了。
        if (imageIndex >= 16)
        {
            isDead = true;
        }
    }

    // 描画処理
    void Draw() override
    {
        // 分割して読み込んだ絵のハンドルが各要素に格納されているので、
        // 要素番号を指定してハンドルを取り出して使う。
        DrawRotaGraphF(position.x, position.y, 1, 0, Image::explosion[imageIndex], TRUE);
    }
};

#endif