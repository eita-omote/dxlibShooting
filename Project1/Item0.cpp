#include "Item0.h"

#include "Player.h" // アイテムとプレイヤは相互にインクルードしそうなのでcppで#include

// 自機に当たったときの処理 上書きoverrideする
void Item0::OnCollisionPlayer(std::shared_ptr<GameObject> object)
{
    if (!((int)collisionTo & (int)CollisionTo::Player))
        return; // プレイヤと当たり判定しないenumビット設定のときは何もせずreturn
    //[共有ポインタをダウンキャスト(親GameObject → 子Playerに)]
    // https://cpprefjp.github.io/reference/memory/shared_ptr/dynamic_pointer_cast.html
    if (std::shared_ptr<Player> pPlayer = std::dynamic_pointer_cast<Player>(object))
    {
        pPlayer->RecoverLife(amount); // ライフを回復する
    }
    isDead = true; // 取ったら消される
}