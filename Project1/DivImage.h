#ifndef DIVIMAGE_H_
#define DIVIMAGE_H_

#include "DxLib.h"
#include <assert.h> // 画像の2のべき乗チェックに使う

// 分割画像を読込むためのデータ構造(画像数や画像ハンドル配列、3Dに使う場合の2のべき乗チェック機能をもつ)
class DivImage
{  // 2Dの分割画像や3Dに分割Div画像を使うと画像全体が使われてしまうのを回避するための情報
public:
    int XNum = 0;
    int YNum = 0;
    int XSize = 0;
    int YSize = 0;
    int* HandleArray = nullptr;
    int AllNum = 0;
    bool is3DTexture = false;

    // 初期化 : X方向分割画像数 XNum, Y方向分割画像数 YNum, 画像 横 幅XSizeピクセル, 画像 縦 高さYSizeピクセル
    DivImage(int XNum, int YNum, int XSize, int YSize, bool is3DTexture = false)
    { // 初期化コンストラクタ
        this->XNum = XNum; // X方向分割画像数
        this->YNum = YNum; // Y方向分割画像数
        this->XSize = XSize; // 画像 横 幅XSize(ピクセルドット数)
        this->YSize = YSize; // 画像 縦 高さYSize(ピクセルドット数)
        AllNum = XNum * YNum; // トータルの分割画像数
        this->is3DTexture = is3DTexture; // 3Dテクスチャ分割画像かtrueだと2のべき乗チェックが入る
        // ★div分割画像読込ハンドラ保存用のint配列メモリを確保し-1で初期化
        this->HandleArray = new int[AllNum]; // 配列を確保し-1で初期化
        for (int i = 0; i < AllNum; i++) HandleArray[i] = -1;
        //↑【演習★】初期化後の動的配列をデバッガで見てみよう[ary,256として再評価ボタン]
        // http://visualstudiostudy.blog.fc2.com/blog-entry-17.html
        int* ary = HandleArray;
#ifdef _DEBUG // デバッグ機能(デバッグの時だけ _DEBUG が定義)
        if (is3DTexture == true) ImagePowCheck((*this)); // *(this)でthisポインタの示す変数内容を表す
#endif
    };

    ~DivImage()
    { // デストラクタでメモリを解放
        if (this->HandleArray != nullptr)
            delete[] this->HandleArray;
    };
    // コピーコンストラクタ(動的new配列をクラス内に持つ場合にはカスタムが必要)
    DivImage(const DivImage& divImage)
    {
        // 基本はただのコピー書き移し
        XNum = divImage.XNum; YNum = divImage.YNum;
        XSize = divImage.XSize; YSize = divImage.YSize;
        AllNum = XNum * YNum;
        is3DTexture = divImage.is3DTexture;
        // ↓★この配列だけはデフォルトのコピーコンストラクタではコピーされない！
        HandleArray = new int[AllNum]; //★[勉強]こちらでもnew確保
        for (int i = 0; i < AllNum; i++)
            HandleArray[i] = divImage.HandleArray[i];
    };
    //【勉強 []添え字演算子を独自に定義】https://programming.pc-note.net/cpp/operator.html
    // 内部のHandleArrayへ今までの様にImage::explosion[5]とかで簡単アクセスできるように【あいだに噛ます】
    int const& operator [](int index) const
    {
        if (index < 0 || AllNum <= index) assert("画像データのアクセス番号がおかしい！" == "");
        return HandleArray[index];
    }
    int& operator [](int index)
    {
        if (index < 0 || AllNum <= index) assert("画像データのアクセス番号がおかしい！" == "");
        return HandleArray[index];
    }

#ifdef _DEBUG // デバッグ機能(デバッグの時だけ _DEBUG が定義)
    bool is_pow2(unsigned int x) // 2のべき乗か計算
    { // https://programming-place.net/ppp/contents/c/rev_res/math012.html
        return (x != 0) && (x & (x - 1)) == 0;
    }

    void ImagePowCheck(DivImage& divImage)
    { // ★3Dに使う画像は2のべき乗でなければ受け付けないようにする
      // https://yttm-work.jp/gmpg/gmpg_0031.html
      // https://yappy-t.hatenadiary.org/entry/20100110/1263138881
        if (divImage.XSize > 0 && divImage.YSize > 0
            && is_pow2(divImage.XSize) && is_pow2(divImage.YSize)) return;
        else assert("3Dに使うなら2のべき乗の画像サイズにしなきゃ" == "");
    }
#endif

private: //代入をプライベートにして禁止する
   
    // 代入演算子の禁止privateオーバーロード
    void operator=(const DivImage& divImage) {};
};

#endif