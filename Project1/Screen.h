
#ifndef _SCREEN_H
#define _SCREEN_H
// ＃ifndefは「_SCREEN_H」がifもしもn(not) def(定義)されていないなら＃endifに挟まれたコードまでを有効化する(だから2度目以降は無効になる)
// 二重定義を防止するために＃から始まる【プリプロセッサ】を使っている
//　【プリプロセッサ】はビルドする段階でコード自体に【前処理】として影響を与える
// たとえば
// ・ファイルの読み込み (including) →＃includeがこれ
// ・マクロの展開（シンボルを、あらかじめ定義された規則に従って置換する）
// ・コンパイル条件によるソースコードの部分的選択→【＃ifndef　～ ＃endifに囲まれた部分】は一度(＃defineで)定義されると無効化
// ・コメントの削除 【 // 】のコメントの削除も【プリプロセス＝前段階で行われる】

// 画面解像度
class Screen
{
public: //publicはC＃と違いpublic:以下にまとめて書かれるスタイル
	static const int Width = 960; // 幅
	static const int Height = 540; // 高さ C＃と違いstaticクラスではなく個々の【すべての変数にstaticをつけて】staticなクラスとする
};// C＃と違ってクラスの定義も;セミコロンで終わる
#endif

