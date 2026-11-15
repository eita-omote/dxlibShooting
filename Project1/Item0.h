#ifndef ITEM_0_H_
#define ITEM_0_H_

#include "Item.h"
#include "Image.h"
#include "Screen.h"

// アイテム0クラス ライフを回復するアイテム
class Item0 : public Item
{
public:
    int amount; // 回復量など
    int bounceNum; // 跳ね返る回数
    bool isFlash; // 画面が一瞬真っ白になる(跳ね返る瞬間)

    // コンストラクタ
    Item0(Vector3 pos,
        Vector3 v = Vector3{ -0.5f,0.0f }, // デフォルトは-0.5fで遅めのスクロール量
        bool isFlash = false, int bounceNum = 0,
        int amount = 1)
        :isFlash{ isFlash }, bounceNum{ bounceNum }, amount{ amount }, Item(pos, v, 8)
    {
        this->tag = "Item0"; // オブジェクトの判別タグ
    }

    // 更新処理
    virtual void Update() override
    {
        this->position += this->v; // vのx,y量ぶん移動
        if (bounceNum > 0) // 反射可能回数が 0 以上なら
        {
            bool isBounce = false; // 反射するか
            if (position.x < collisionRadius)
            {
                v.x = -v.x; // x逆方向に反射(マイナスをつけて方向反転)
                position.x = collisionRadius; // めりこんだぶんをスクリーンの境界線に沿わす
                isBounce = true;
            }
            else if (Screen::Width - collisionRadius < position.x)
            {
                v.x = -v.x; // x逆方向に反射(マイナスをつけて方向反転)
                position.x = Screen::Width - collisionRadius; // めりこんだぶんを境界線に沿わす
                isBounce = true;
            }

            if (position.y < collisionRadius)
            {
                v.y = -v.y; // y逆方向に反射(マイナスをつけて方向反転)
                position.y = collisionRadius; // めりこんだぶんをスクリーンの境界線に沿わす
                isBounce = true;
            }
            else if (Screen::Height - collisionRadius < position.y)
            {
                v.y = -v.y; // y逆方向に反射(マイナスをつけて方向反転)
                position.y = Screen::Height - collisionRadius; // めりこんだぶんを境界線に沿わす
                isBounce = true;
            }

            if (isBounce) // 反射判定がtrueならば
            {
                --bounceNum; // 跳ね返り可能回数を -1 する
                v *= 0.94f; // 反射時のスピード減衰率
                isFlash = true; // 画面を一瞬フラッシュする
            }
        }

        // スクリーンの画面外になったらisDeadにして消す
        if (position.x + collisionRadius < 0 || Screen::Width < position.x - collisionRadius
            || position.y + collisionRadius < 0 || Screen::Height < position.y - collisionRadius)
        {
            isDead = true;
        }
    }

    // 描画処理
    virtual void Draw() override
    {
        if (isFlash) // 画面を一瞬真っ白にする(跳ね返る瞬間)
        {
            // 画面のスクリーンと同じサイズの大きな白い四角を描く
            DrawBox(0, 0, Screen::Width, Screen::Height, GetColor(200, 200, 200), TRUE);
            // 白画面の上に黒丸を描くとバウンド位置が目立つ(衝撃波っぽい)
            DrawCircle(position.x, position.y, collisionRadius * 5, GetColor(0, 0, 0), TRUE);
        }
        DrawRotaGraphF(position.x, position.y, (isFlash) ? 2.0f : 1, 0, Image::item0, TRUE);
        isFlash = false; // すぐにfalseに戻せば一瞬だけしか光らない
    }

    // 自機に当たったときの処理 上書きoverrideする
    virtual void OnCollisionPlayer(std::shared_ptr<GameObject> object) override;
};

#endif