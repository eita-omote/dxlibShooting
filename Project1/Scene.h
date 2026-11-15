#ifndef SCENE_H_
#define SCENE_H_

#include <string> // シーンの判別文字列tagに使う

// シーンの基底クラス あいまいなabstract型タイプ
class Scene
{
public:
    std::string tag = ""; // シーンの種類の判別に使う

    // コンストラクタ
    Scene()
    {

    }
    //★仮想デストラクタ【忘れるとメモリがヤバいダメ絶対】
    //【注意！】ベースの基底では必ず定義しないとstringなどが浪費され極悪なメモリ被害に発展
    virtual ~Scene()
    {
        // 特に配列とかなくても★string文字列も内部では実質配列だから！5文字分のメモリとかがすり減ってく..
        // 仮想デストラクタはstringなど暗黙に内部に配列を持つ要素のデストラクタを暗黙に呼んでくれている！
    }

    //★純粋仮想関数=0は継承したPlaySceneなどの必須機能(継承したら絶対override必須縛り)
    // 必須縛りによってSceneを継承したものは【Update,Drawが実装されてるはずの確証があるので】
    // for文であいまいなSceneのまま【まとめてUpdate,Drawできる】のだ。

    // 初期化処理
    virtual void Initialize() = 0; //純粋仮想関数に(継承したらoverride必須)

    // 終了処理(大抵Initializeと同じくリセット処理を行うがInitと区別して終了時だけやりたいリセットもある)
    virtual void Finalize() = 0; //純粋仮想関数に(継承したらoverride必須)

    // 更新処理
    virtual void Update() = 0; //純粋仮想関数に(継承したらoverride必須)

    // 描画処理
    virtual void Draw() = 0; //純粋仮想関数に(継承したらoverride必須)
};

#endif