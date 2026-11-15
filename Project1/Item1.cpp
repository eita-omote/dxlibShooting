#include "Item1.h"

#include "Player.h" // アイテムとプレイヤは相互にインクルードしそうなのでcppで#include

// 自機に当たったときの処理 上書きoverrideする
void Item1::OnCollisionPlayer(std::shared_ptr<GameObject> object)
{
    if (!((int)collisionTo & (int)CollisionTo::Player))
        return; // プレイヤと当たり判定しないenumビット設定のときは何もせずreturn

    //[共有ポインタをダウンキャスト(親GameObject → 子Playerに)]
    // https://cpprefjp.github.io/reference/memory/shared_ptr/dynamic_pointer_cast.html
    if (std::shared_ptr<Player> pPlayer = std::dynamic_pointer_cast<Player>(object))
    {
        pParent = pPlayer.get(); // プレイヤへのリンクを親リンクとしてリンクされた(x,y)を中心に回転
        state = State::Barrier; // バリアモードへ移行
        collisionTo = CollisionTo::EnemyBullet; // 敵弾との当たり判定モードに移行
    }
}