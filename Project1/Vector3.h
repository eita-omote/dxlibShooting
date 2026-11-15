#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "DxLib.h"

#include <array> //vector配列と基本同じだが【有限】の個数である違いがあるxyzなど3つ固定の個数ならstd::vectorよりstd::array型がよい
#include <cmath>
#include <limits> // 無限大などをstd::numeric_limits<float>::infinity()で取得
#include <float.h> // FLT_EPSILONでfloat型のとりうる最小の極小値を得る
#include <memory> // 共有ポインタを使う

#ifndef GET_CLAMP // GET_CLAMPマクロの定義がなかったら定義して使う
// a と b 小さい方の値を求める
#define    GET_MIN( a, b )            ( ( (a) < (b) ) ? (a) : (b) )
// a と b 大きい方の値を求める
#define    GET_MAX( a , b )        ( ( (a) > (b) ) ? (a) : (b) )
// aを min から max の値に留める
#define    GET_CLAMP( a, min, max )    GET_MAX( min, GET_MIN( a, max ) )
#endif

// 軸のタイプ X軸:Axis3D::X, Y軸:Axis3D::Y, Z軸:Axis3D::Z
enum class Axis3D { X = 0, Y, Z };

//★【UnityのVector演算コード】
// https://github.com/Unity-Technologies/UnityCsReference/blob/02d565cf3dd0f6b15069ba976064c75dc2705b08/Runtime/Export/Math/Vector3.cs

// 3変数x,y,zを持つ構造体。3つの数字をまとめて扱うならXYZ座標以外にも使える。x,y,z別々に足したり引いたり面倒なときこそ
struct Vector3
{
    // struct型はpublic:がなくてもデフォルトがpublic:  private:を書けばプライベートにもできる
      //float x = 0; // x座標
      //float y = 0; // y座標
      //float z = 0; // z座標 3つの定義をunionで配列xyzと共用することでxyz[1]=5.5f;とすると yも5.5となる【運命共用体】(メモリ上は同じ位置になる)

    union { // ★共用体unionテクニック https://inemaru.hatenablog.com/entry/2016/03/02/005408
        struct { // [参考ビットサイズ] https://marycore.jp/prog/c-lang/data-type-ranges-and-bit-byte-sizes/
            float x;// x座標
            float y;// y座標
            float z;// z座標
        }; //[匿名共用体とは] https://zenn.dev/block/articles/beda29f11f05afc147ef
        std::array<float, 3> xyz; // float xyz[3];と同じ意味 float 3個ぶんのデータサイズでx,y,z 3個ぶんと一致するので★unionで共用
        struct { float r, g, b; }; // 赤緑青のRGB成分としてつかいたいとき
#ifdef DX_LIB_H // DXlibを#includeしていればDX_LIB_Hも定義済みなので下記も定義される VECTORの定義はDXライブラリ版のVector3
        VECTOR dxVec;
#endif
    }; // unionは異なる複数のものをメモリ上の同一の番地に割り当てられる⇒x,y,z分けて記述するの面倒なとき配列xyz[3]をfor文i=0～3で回せる

    // float型のポインタへ(float*)キャストすればx,y,zデータの配列の先頭のアドレスを返す
    inline operator float* () const { return (float*)this; }
    // float型のポインタへ(const float*)キャストすればx,y,zデータの配列の先頭のアドレスを返す
    inline operator const float* () const { return (const float*)this; }

    // コンストラクタ 初期化 z = 0.fでzのデフォルト値があるので2Dとしても使える
    inline Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        this->xyz = { x,y,z };// たった1行で書ける
    }
    // デストラクタ
    ~Vector3() {}

    // Unityのstatic変数を参考に https://docs.unity3d.com/ja/current/ScriptReference/Vector3.html
    static const Vector3 zero;// = { 0.f, 0.f, 0.f };
    static const Vector3 one; // = { 1.f, 1.f, 1.f };
    static const Vector3 forward;// = { 0.f, 0.f, 1.f };
    static const Vector3 back; // = { 0.f, 0.f, -1.f };
    static const Vector3 up; // = { 0.f, 1.f, 0.f };
    static const Vector3 down;// = { 0.f, -1.f, 0.f };
    static const Vector3 left; // = {-1.f, 0.f, 0.f };
    static const Vector3 right;// = { 1.f, 0.f, 0.f };
    //[負の値の最小値] https://stackoverflow.com/questions/20016600/negative-infinity
    static const Vector3 negativeInfinity;
    //[極大値]https://cpprefjp.github.io/reference/limits/numeric_limits/lowest.html
    static const Vector3 positiveInfinity;

    // destArrayにデータを保管しておく
    Vector3& copyToArray(std::array<float, 3>& destArray)
    {
        destArray = this->xyz;//配列としてコピー
        return *this;
    }
    // sourceArray配列からVector3データを生成
    Vector3 fromArray(std::array<float, 3>& sourceArray) const //[後ろにconstをつけないと..] https://www7b.biglobe.ne.jp/~robe/cpphtml/html02/cpp02010.html
    {
        return Vector3{ sourceArray[0], sourceArray[1], sourceArray[2] }; //配列から初期化
    }


    // ベクトルの長さ(原点0から点(x,y,z)までの距離)
    inline float magnitude() const //[後ろにconstをつけないと..] https://www7b.biglobe.ne.jp/~robe/cpphtml/html02/cpp02010.html
    {
        return std::sqrt(x * x + y * y + z * z); // √(x*x + y*y + z*z)
    };

    // ベクトルの 2 乗の長さを返します(平方根しないぶん計算速い)
    inline float sqrMagnitude() const //[後ろにconstをつけないと..] https://www7b.biglobe.ne.jp/~robe/cpphtml/html02/cpp02010.html
    {
        return x * x + y * y + z * z; // x*x + y*y + z*z
    };


    // 正規化したベクトルを返す
    inline Vector3 normalized() const //[後ろにconstをつけないと..] https://www7b.biglobe.ne.jp/~robe/cpphtml/html02/cpp02010.html
    {
        float mag = magnitude();
        if (mag < 0.00001f) // ほぼ0ベクトルか?
            return *this;
        else
            return Vector3{ x / mag, y / mag, z / mag }; // x / |x|, y / |y|, z / |z|
    }


    /*-----　演算子オーバーロード　-----*/

    // 逆ベクトル
    inline Vector3 operator - () const
    {
        return Vector3(-this->x, -this->y, -this->z);
    }

    // Vectorをそのまま足し合わせる
    inline Vector3 operator + () const
    {
        return *this;
    }

    inline Vector3& operator += (const Vector3& add_v3)
    {
        this->x += add_v3.x;
        this->y += add_v3.y;
        this->z += add_v3.z;

        return *this; //[関数連鎖] *thisを返すことで v1 + v2 + v3見たく数珠繋ぎできる
    }

    inline Vector3& operator -= (const Vector3& minus_v3)
    {
        this->x -= minus_v3.x;
        this->y -= minus_v3.y;
        this->z -= minus_v3.z;

        return *this;
    }

    inline Vector3& operator *= (float multiply_num)
    {
        this->x *= multiply_num;
        this->y *= multiply_num;
        this->z *= multiply_num;

        return *this;
    }

    // 0.fで割ったときは±無限大を返す
    inline Vector3& operator /= (float divide_num)
    {
        if (divide_num == 0.0f) // 0で割ったら±無限大を返す
        {
            *this = Vector3{ ((this->x < 0) ? 1 : -1) * std::numeric_limits<float>::infinity(),
                             ((this->y < 0) ? 1 : -1) * std::numeric_limits<float>::infinity(),
                             ((this->z < 0) ? 1 : -1) * std::numeric_limits<float>::infinity() };
            return *this;
        }

        this->x /= divide_num;
        this->y /= divide_num;
        this->z /= divide_num;

        return *this;
    }

    // 代入演算子 x,y,zを全部 change_numに変える
    inline Vector3& operator = (float change_num)
    {
        this->x = change_num;
        this->y = change_num;
        this->z = change_num;

        return *this;
    }

    // 一致演算子 x,y,zが全部一致するか 一つでも違えばfalse
    inline bool operator == (const Vector3& v3_other) const
    {
        if (this->x != v3_other.x) return false;
        if (this->y != v3_other.y) return false;
        if (this->z != v3_other.z) return false;
        return true;
    }

    // 不一致演算子
    inline bool operator != (const Vector3& v3_other) const
    {
        return    !(*this == v3_other);
    }

    /*---- 以下 DXライブラリ向けにVector3とDX版Vectorの変換方法を用意 ---------------*/

  // DXlibを#includeしていればDX_LIB_Hも定義済みなので下記関数も定義される VECTORの定義はDXライブラリ版のVector3
#ifdef DX_LIB_H
  // Vector3からDxLibのVECTORへの変換
    inline VECTOR Vec3ToVec()
    {
        VECTOR Result;
        Result.x = this->x;
        Result.y = this->y;
        Result.z = this->z;
        return Result;
    }

    // 左辺値がDXのVECTOR型のときの=代入対応
    inline operator VECTOR() const
    {
        VECTOR Result;
        Result.x = this->x;
        Result.y = this->y;
        Result.z = this->z;
        return Result;
    }

    // 右辺値がDXのVECTOR型のときの=代入対応
    inline Vector3& operator = (const VECTOR& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    // コンストラクタ DXのVECTOR型から = 初期化できるように
    inline Vector3(VECTOR other) : x{ other.x }, y{ other.y }, z{ other.z } {}

#endif

#ifdef BT_VECTOR3_H
    // 右辺値が 物理エンジンBullet の btVector3型ときの = 代入対応 (btVector3型:右手→左手:DirectX系 位置変換)
    inline Vector3& operator = (const btVector3& right)
    {   // Z軸にはマイナスをつけて右手系を左手系に変換 http://dolpetticoat.blog.fc2.com/blog-entry-92.html
        x = (float)right.getX(), y = (float)right.getY(), z = -(float)right.getZ();
        return *this;
    }

    // コンストラクタ 物理エンジンBullet の btVector3型 から = 初期化できるように (btVector3型:右手→左手:DirectX系 位置変換)
    inline Vector3(const btVector3& other) : x{ (float)other.getX() }, y{ (float)other.getY() }, z{ -(float)other.getZ() } {}

    inline operator btVector3() const { return btVector3{ x, y, -z }; }
#endif
};

#ifdef BT_VECTOR3_H

// btVector3型:右手→左手:DirectX系 オイラー回転を変換 http://dolpetticoat.blog.fc2.com/blog-entry-92.html
inline Vector3 rotationXYZ(const btVector3& eulerZYX) { return Vector3{ -(float)eulerZYX.getX(), -(float)eulerZYX.getY(),(float)eulerZYX.getZ() }; }

#endif


// Vector3どうしの足し算 割り算 掛け算 割り算などの 基本演算の グローバル定義


// Vector同士の足し算 x,y,z個別に足し合わせる
inline Vector3 operator + (const Vector3& left, const Vector3& right)
{
    Vector3 v3; //注意 vec1 = vec1 + vec2のとき vec1の数値が書き変わったら嫌だからv3を新たに用意
    v3.x = left.x + right.x; //[コレはダメ] left.x = left.x + right.x;
    v3.y = left.y + right.y;
    v3.z = left.z + right.z;

    return v3;
}

// Vector同士の引き算 x,y,z個別に足し合わせる
inline Vector3 operator - (const Vector3& left, const Vector3& right)
{
    Vector3 v3; //注意 vec1 = vec1 - vec2のとき vec1の数値が書き変わったら嫌だからv3を新たに用意
    v3.x = left.x - right.x; //[コレはダメ] left.x = left.x - right.x;
    v3.y = left.y - right.y;
    v3.z = left.z - right.z;

    return v3;
}

// Vectorと数値の掛け算 x,y,z個別に掛け合わせる
inline Vector3 operator * (const Vector3& left, float right)
{
    Vector3 v3;
    v3.x = left.x * right;
    v3.y = left.y * right;
    v3.z = left.z * right;

    return v3;
}


// 数値とVectorの掛け算 x,y,z個別に掛け合わせる
inline Vector3 operator * (float left, const Vector3& right)
{
    Vector3 v3;
    v3.x = left * right.x;
    v3.y = left * right.y;
    v3.z = left * right.z;

    return v3;
}

// Vectorと数値の割り算 x,y,z個別に割り合わせる
// 0.fで割ると±無限大を返す
inline Vector3 operator / (const Vector3& left, float right)
{
    if (right == 0.0f) // 0で割ったら±無限大を返す
        return Vector3{ ((left.x < 0) ? 1 : -1) * std::numeric_limits<float>::infinity(),
                        ((left.y < 0) ? 1 : -1) * std::numeric_limits<float>::infinity(),
                        ((left.z < 0) ? 1 : -1) * std::numeric_limits<float>::infinity() };

    Vector3 v3;
    v3.x = left.x / right;
    v3.y = left.y / right;
    v3.z = left.z / right;

    return v3;
}

// 比較不等号 < 演算子
inline bool operator < (const Vector3& left, const Vector3& right)
{
    if (left.x != right.x) return left.x < right.x;
    if (left.y != right.y) return left.y < right.y;
    return left.z < right.z;
}

// 比較不等号 > 演算子
inline bool operator > (const Vector3& left, const Vector3& right)
{
    if (left.x != right.x) return left.x > right.x;
    if (left.y != right.y) return left.y > right.y;
    return left.z > right.z;
}

// 比較不等号 <= 演算子
inline bool operator <= (const Vector3& left, const Vector3& right)
{
    return    !(left > right);
}

// 比較不等号 >= 演算子
inline bool operator >= (const Vector3& left, const Vector3& right)
{
    return    !(left < right);
}

// 2つのベクトルの内積
inline float dot(const Vector3& left, const Vector3& right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z; //普通にx同士,y同士,z同士掛けるだけ
}

// 2つのベクトルの外積
inline Vector3 cross(const Vector3& left, const Vector3& right)
{
    return Vector3{
      left.y * right.z - left.z * right.y, // x = 左y * 右z - 左z * 右y
      left.z * right.x - left.x * right.z, // y = 左z * 右x - 左x * 右z
      left.x * right.y - left.y * right.x };// z = 左x * 右y - 左y * 右x //行列独特の特殊な掛け方
}

// ベクトルの 2 乗の長さを返します(平方根しないぶん計算速い)
inline float sqrMagnitude(const Vector3& vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; // x*x + y*y + z*z
}

// ベクトルの長さ(原点0から点(x,y,z)までの距離)
inline float magnitude(const Vector3& vec)
{
    return std::sqrt(sqrMagnitude(vec)); // √x*x + y*y + z*z
}

// start と end の間の距離
inline float distance(const Vector3& start, const Vector3& end)
{
    return (start - end).magnitude();
}

// 直線上にある2つのベクトル間を補間(start:開始位置  end:終了位置  t = 補間の割合 tは0～1の範囲にクランプしない)
inline Vector3 lerpUnclamped(const Vector3& start, const Vector3& end, float t)
{
    return start + (end - start) * t;
}

// 線形補間する start:開始位置  end:終了位置  t = 補間の割合（start側が100%  0.0～1.0  end側が100% でブレンド) tは0～1の範囲にクランプする
inline Vector3 lerp(const Vector3& start, const Vector3& end, float t)
{
    return lerpUnclamped(start, end, GET_CLAMP(t, 0.0f, 1.0f));
}


// 2つのベクトルのx,y,zのそれぞれ大きいほうの数値で構成したベクトルを求める 例. v1(1,2,3) v2(3,2,1) なら(3,2,3)になる
inline Vector3 maximize(const Vector3& v1, const Vector3& v2)
{
    Vector3 result;
    result.x = (v1.x > v2.x) ? v1.x : v2.x;
    result.y = (v1.y > v2.y) ? v1.y : v2.y;
    result.z = (v1.z > v2.z) ? v1.z : v2.z;
    return result;
}


// 2つのベクトルのx,y,zのそれぞれ小さいほうの数値で構成したベクトルを求める 例. v1(1,2,3) v2(3,2,1) なら(1,2,1)になる
inline Vector3 minimize(const Vector3& v1, const Vector3& v2)
{
    Vector3 result;
    result.x = (v1.x < v2.x) ? v1.x : v2.x;
    result.y = (v1.y < v2.y) ? v1.y : v2.y;
    result.z = (v1.z < v2.z) ? v1.z : v2.z;
    return result;
}

// 2次補間する start:開始位置 mid:中間制御点 end:終了位置  t = 補間の割合（start側が100%  0.0～1.0  end側が100% でブレンド)
inline Vector3 quadratic(const Vector3& start, const Vector3& mid, const Vector3& end, float t)
{    //[ベジェ曲線補間の3点ver.] https://ja.javascript.info/bezier-curve
    return start * (1.0f - t) * (1.0f - t) + 2 * mid * t * (1.0f - t) + end * t * t;
}


// ベジェ曲線の補間をする start:開始位置 mid1:中間制御点1  mid2:中間制御点2 end:終了位置  t = 補間の割合（start側が100%  0.0～1.0  end側が100%)
inline Vector3 bezier(const Vector3& start, const Vector3& mid1, const Vector3& mid2, const Vector3& end, float t)
{    //[ベジェ曲線補間の4点ver.] https://ja.javascript.info/bezier-curve
    Vector3 vec;
    vec.x = start.x * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3 * (1.0f - t) * (1.0f - t) * t * mid1.x + 3 * (1.0f - t) * t * t * mid2.x + t * t * t * end.x;
    vec.y = start.y * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3 * (1.0f - t) * (1.0f - t) * t * mid1.y + 3 * (1.0f - t) * t * t * mid2.y + t * t * t * end.y;
    vec.z = start.z * (1.0f - t) * (1.0f - t) * (1.0f - t) + 3 * (1.0f - t) * (1.0f - t) * t * mid1.z + 3 * (1.0f - t) * t * t * mid2.z + t * t * t * end.z;
    return vec;
}


// 2つのベクトルの各成分をかけ算
inline Vector3 scale(const Vector3& left, const Vector3& right)
{
    return Vector3{ left.x * right.x, left.y * right.y, left.z * right.z };
}

// ベクトルを別のベクトルに投影
inline Vector3 project(const Vector3& vector, const Vector3& on_normal)
{
    float sqrMag = on_normal.sqrMagnitude();
    if (sqrMag < 0.00001f)
        return Vector3::zero;
    return on_normal * dot(vector, on_normal) / sqrMag;
}

// 平面に垂直な法線ベクトルにより定義される平面上にベクトルを射影
inline Vector3 projectOnPlane(const Vector3& vector, const Vector3& planeNormal)
{
    return vector - project(vector, planeNormal);
}

// 仰角:radPolar[単位:ラジアン] 方位角:radAzimuth[単位:ラジアン]から方位ベクトル(極座標の位置の単位ベクトル)を求める
inline Vector3 polarDir(float radPolar, float radAzimuth)
{   //[球体表面上の点へ向かう方位ベクトル] https://www.pentacreation.com/blog/2020/12/201225.html
    Vector3 result;
    result.x = std::cos(radPolar) * std::sin(radAzimuth);
    result.y = -std::sin(radPolar);
    result.z = std::cos(radPolar) * std::cos(radAzimuth);
    return result;
}

// 2つのベクトルの角度θを求める(単位:ラジアン)
inline float radAngle(const Vector3& start, const Vector3& end)
{
    float cos_theta = dot(start, end) / (start.magnitude() * end.magnitude());
    return std::acos(GET_CLAMP(cos_theta, -1.0f, 1.0f));
}

// 三角ポリゴンの面法線を求める p0,p1,p2:三角ポリゴンの3点の位置ベクトル
inline Vector3 polygonNorm(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{   //[DirectX:左手系(時計回りの親指が上)]
    // 直線p1p0 と 直線p2p1 の外積を求めて、それを単位ベクトルNorm化する
    return cross(p0 - p1, p1 - p2).normalized();
}

// 反射ベクトルを求める input:入射ベクトル  eNorm:単位法線ベクトル
inline Vector3 reflection(const Vector3& input, const Vector3& eNorm)
{   //                               内積↓
    // [計算式]反射ベクトル = 2 * ( -入射・法線 ) * 法線 + 入射
    return 2 * dot(-input, eNorm) * eNorm + input;
}

// 壁ずりベクトル(単位化済)を求める input:入射ベクトル  eNorm:単位法線ベクトル
inline Vector3 wallscratch(const Vector3& input, const Vector3& eNorm)
{   // http://marupeke296.com/COL_Basic_No5_WallVector.html
    return (input - dot(input, eNorm) * eNorm).normalized();
}

// 屈折ベクトルを求める input:入射ベクトル eNorm:単位法線ベクトル 屈折比率: n = n1 / n2 (入射角度:θ1 屈折角度:θ2)
inline Vector3 refraction(const Vector3& input, const Vector3& eNorm, float n)
{
    //[ベクトルの屈折] https://tyfkda.github.io/blog/2009/07/19/refraction.html
    Vector3 inverse = -input; // 逆ベクトル
    float cos_theta1 = dot(inverse, eNorm); // cosθ1 = -入射ベクトル・法線ベクトル
    float cosT2 = 1.0f - n * n * (1.0f - cos_theta1 * cos_theta1); //
    if (cosT2 <= 0.0f) return Vector3{ 0.0f, 0.0f, 0.0f }; // 屈折光はでてこない
    float abs_cosT2 = (cosT2 < 0) ? -cosT2 : cosT2;

    // 屈折比率: n = n1 / n2 (入射角度:θ1 屈折角度:θ2)
    // 屈折ベクトル = n * -入射 + 法線 * (n * cosθ1 - √(1 - n * n * (1 - cosθ1 * cosθ1) )
    return n * inverse + eNorm * (n * cos_theta1 - std::sqrt(abs_cosT2));
}

// 円周率×2
#define PI_2 3.14159265358979323846264f * 2

// ラジアン: 0～2π (0°～ 360°)の範囲に変換
#define RAD_NORMALIZE( rad ) ( ( rad >= 0.0f ) ? std::fmod( rad, PI_2 ): std::fmod( rad, PI_2 ) + PI_2 )

// ベクトルから Pitch(ピッチ) を求める https://watako-lab.com/2019/01/23/roll_pitch_yaw/
inline float pitch(const Vector3& vec)
{
    float length = vec.magnitude();
    if (length == 0) return 0.0f; // ベクトルの長さが0のときは0°

    // こたえを 0～2π (0°～ 360°)の範囲に変換してreturn
    return RAD_NORMALIZE(std::asin(-vec.y / length));
}

// ベクトルから Yaw(ヨー) を求める https://watako-lab.com/2019/01/23/roll_pitch_yaw/
inline float yaw(const Vector3& vec)
{
    float length = vec.magnitude();
    if (length == 0) return 0.0f; // ベクトルの長さが0のときは0°

    return RAD_NORMALIZE(std::atan2(vec.x / length, vec.z / length));
}

// 一時定義したマクロを #undef で未定義に戻す
#undef PI_2
#undef RAD_NORMALIZE

// 重心座標を求める 点 p0:重心座標を求めたい位置 三角形の点: p1 p2 p3 https://zellij.hatenablog.com/entry/20131207/p1
inline Vector3 barycentric(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    Vector3 v2 = p2 - p1;
    Vector3 v3 = p3 - p1;
    Vector3 v0 = p0 - p1;
    float d22 = dot(v2, v2);
    float d23 = dot(v2, v3);
    float d33 = dot(v3, v3);
    float d02 = dot(v0, v2);
    float d03 = dot(v0, v3);
    float denom = d22 * d33 - d23 * d23;
    float v = (d33 * d02 - d23 * d03) / denom;
    float w = (d22 * d03 - d23 * d02) / denom;
    return  p1 * (1.0f - v - w) + p2 * v + p3 * w;
}

// 調べたい点point から一番近い直線上の点を求める linePos: 線の起点 lineVec:線のベクトル isInfinite:無限に延びる線か? result_t:接点までの直線パラメータ t
inline Vector3 nearestPointOnLine(const Vector3& point, const Vector3& lineStart, const Vector3& lineDir, bool isInfinite = true, float* result_t = nullptr)
{
    float t; // 直線のパラメータ t (tが0だと始点startの位置、tが増えると始点からより遠くに延長された位置になる)
    float lineLengthSq = lineDir.sqrMagnitude();
    if (lineLengthSq < FLT_EPSILON) // 線分のベクトルの長さがほぼ0なら
    {
        if (result_t != nullptr)
            *result_t = 0.0f; // 直線パラメータ t は起点の位置だから 0 の位置

        return lineStart; // 線分の始点が最も近い
    }

    Vector3    linePosToPoint = point - lineStart; // 線の起点linePos から 調べたい点pointへのベクトル

    t = dot(lineDir, linePosToPoint) / lineLengthSq; // 調べたい座標に最も近い lineVec線上の座標 への直線パラメータ t を計算

    if (isInfinite == false) // 有限線分のとき (線が無限に延びてないタイプのとき)
        t = GET_CLAMP(t, 0.0f, 1.0f); // 有限の線だから GET_CLAMP でtの値を 0.0f～1.0fの間に限定する

    if (result_t != nullptr) // result_tがnullptrじゃなければ 直線パラメータ t もついでに計算結果を返却しておく
        *result_t = t;

    return lineDir * t + lineStart; // lineVecの方向をt倍のばして一番近い直線上の点の位置を求める
}


// 平行で交わらない線分line0 と 線分line1 の一番近い点を求める isInfinite:無限の長さの線かどうか result0:line0上の最も近い点 result1:line1上の最も近い点
inline void nearestPointOfParallelLine(const Vector3& line0Start, const Vector3& line0End, const Vector3& line1Start, const Vector3& line1End,
    bool isInfinite = false, Vector3* result0 = nullptr, Vector3* result1 = nullptr)
{
    float t0, t1, t; // 直線のパラメータを求める

    Vector3 answer0 = nearestPointOnLine(line1Start, line0Start, line0End - line0Start, true, &t0); // 始点line1Start から一番近い直線line0上の点を求める
    if (result0 != nullptr) *result0 = answer0; // resultがnullptrじゃなければ答えを代入して返す

    if (isInfinite == true) // 無限の長さの線ならば
    {
        Vector3 answer1 = line1Start; // line1の起点が一番近い
        if (result1 != nullptr) *result1 = answer1; // resultがnullptrじゃなければ答えを代入して返す
    }
    else
    {
        Vector3    tmpPoint = nearestPointOnLine(line1End, line0Start, line0End - line0Start, true, &t1); // 終点line1end から一番近い直線line0上の点を求める
        if ((t0 < 0.0f) && (t1 < 0.0f)) // 最も近い点 が line0もline1も両方【始点】より外側の延長線上にあるとき
        {
            answer0 = line0Start; // 始点が答え(最も近い点)になる
            Vector3 answer1 = (t0 < t1) ? line1End : line1Start;
            if (result0 != nullptr) *result0 = answer0; // resultがnullptrじゃなければ答えを代入して返す
            if (result1 != nullptr) *result1 = answer1;
        }
        else if ((t0 > 1.0f) && (t1 > 1.0f)) // 最も近い点 が line0もline1も両方【終点】より外側の延長線上にあるとき
        {
            answer0 = line0End; // 終点が答え(最も近い点)になる
            Vector3 answer1 = (t0 < t1) ? line1Start : line1End;
            if (result0 != nullptr) *result0 = answer0; // resultがnullptrじゃなければ答えを代入して返す
            if (result1 != nullptr) *result1 = answer1;
        }
        else
        {
            t = 0.5f * (GET_CLAMP(t0, 0.0f, 1.0f) + GET_CLAMP(t1, 0.0f, 1.0f));
            answer0 = (line0End - line0Start) * t + line0Start; // line0の方向をt倍のばして一番近い直線上の点の位置を求める
            Vector3 answer1 = nearestPointOnLine(answer0, line1Start, line1End - line1Start, true); // 点answer0 から一番近い直線line1上の点を求める
            if (result0 != nullptr) *result0 = answer0; // resultがnullptrじゃなければ答えを代入して返す
            if (result1 != nullptr) *result1 = answer1;
        }
    }
}

// 線分line0 と 線分line1 の【有限の線上範囲内】での一番近い点を求める result0:line0上に収まる最も近い点 result1:line1上に収まる最も近い点
inline void nearestPointsOnFiniteLines(const Vector3& line0Start, const Vector3& line0End, const Vector3& line1Start, const Vector3& line1End,
    float t0, float t1, Vector3& result0, Vector3& result1)
{
    Vector3 v0 = line0End - line0Start; // line0 の start点 から end点 へ向かう ベクトル
    Vector3 v1 = line1End - line1Start; // line1 の start点 から end点 へ向かう ベクトル
    if ((t0 < 0.0f || 1.0f < t0) && (t1 < 0.0f || 1.0f < t1)) // t0 も t1 も両方 0～1.0の範囲外(line0とline1の始点から終点の範囲内におさまってない)
    {
        t0 = GET_CLAMP(t0, 0.0f, 1.0f); // まず t0 を 0～1.0以内に収める(0以下 や 1以上 でも 0 や 1 にする)
        Vector3 answer0 = v0 * t0 + line0Start; // v0の方向を t0倍 のばした直線line0上 の 範囲内の点 の位置を一旦、求める
        Vector3 answer1 = nearestPointOnLine(answer0, line1Start, v1, true, &t1); // 範囲内の点↑answer0 から 一番近い 直線line1(無限長)上の点 を求める
        if (t1 < 0.0f || 1.0f < t1) // t1が 直線line1 の 始点0 ～ 終点1.0 の↑範囲外ならば t1を0～1.0の範囲に収めてさらに再計算
        {
            t1 = GET_CLAMP(t1, 0.0f, 1.0f); // t1 = 0～1.0以内に収めて再計算
            answer1 = v1 * t1 + line1Start; // v1の方向を t1倍 のばして一番近い直線上の点の位置を求める
            answer0 = nearestPointOnLine(answer1, line0Start, v0, false); // 点answer1 から 一番近い 直線line0(有限)上の点 を求めなおす
            answer1 = nearestPointOnLine(answer0, line1Start, v1, false); // 点answer0 から 一番近い 直線line1(有限)上の点 を求めなおす
        }
        result0 = answer0; // 求めたanswer で 結果 result を確定して関数外へ返す
        result1 = answer1;
    }
    else if (t0 < 0.0f || 1.0f < t0) // t0 が 0～1.0の範囲におさまっていない(line0の始点から終点の範囲内におさまってない)
    {
        t0 = GET_CLAMP(t0, 0.0f, 1.0f); // t0 を 0～1.0以内に収める(0以下 や 1以上 でも 0 や 1 にする)
        Vector3 answer0 = v0 * t0 + line0Start; // v0の方向を t0倍 のばした直線line0上 の 範囲内の点 の位置を一旦、求める
        Vector3 answer1 = nearestPointOnLine(answer0, line1Start, v1, false); // 点answer0 から 一番近い 直線line1(有限)上の点 を求める

        result0 = answer0; // 求めたanswer で 結果 result を確定して関数外へ返す
        result1 = answer1;
    }
    else if (t1 < 0.0f || 1.0f < t1) // t1 が 0～1.0の範囲におさまっていない(line1の始点から終点の範囲内におさまってない)
    {
        t1 = GET_CLAMP(t1, 0.0f, 1.0f); // t1 を 0～1.0以内に収める(0以下 や 1以上 でも 0 や 1 にする)
        Vector3 answer1 = v1 * t1 + line1Start; // v0の方向を t0倍 のばした直線line0上 の 範囲内の点 の位置を一旦、求める
        Vector3 answer0 = nearestPointOnLine(answer1, line0Start, v0, false); // 点answer1 から 一番近い 直線line0(有限)上の点 を求める

        result0 = answer0; // 求めたanswer で 結果 result を確定して関数外へ返す
        result1 = answer1;
    }
}


// 点:point と box:AABB(軸並行境界ボックス：Axis-Aligned Bounding Box)の最短距離の2乗 http://marupeke296.com/COL_3D_No11_AABBvsPoint.html
inline float sqrNearestLengthPoint_AABB(const Vector3& point, const Vector3& boxMin, const Vector3& boxMax)
{
    float sqLength = 0.0f; // 長さのべき乗の値を足していく
    for (auto i = 0; i < 3; ++i)
    {   // 各軸で点が最小値以下もしくは最大値以上ならば、差を考慮
        if (point.xyz[i] < boxMin.xyz[i])  // i=0:X,,i=1:Y, i=2はZの意味です
            sqLength += (point.xyz[i] - boxMin.xyz[i]) * (point.xyz[i] - boxMin.xyz[i]); // 距離の2乗
        if (point.xyz[i] > boxMax.xyz[i])
            sqLength += (point.xyz[i] - boxMax.xyz[i]) * (point.xyz[i] - boxMax.xyz[i]); // 距離の2乗
    }
    return sqLength; //  // 距離の2乗を足していった合計を返す
}

// 点:point と box:AABB(軸並行境界ボックス：Axis-Aligned Bounding Box)の最短距離 http://marupeke296.com/COL_3D_No11_AABBvsPoint.html
inline float nearestLengthPoint_AABB(const Vector3& point, const Vector3& boxMin, const Vector3& boxMax)
{
    return std::sqrtf(sqrNearestLengthPoint_AABB(point, boxMin, boxMax)); // 距離の2乗を足していった合計の√を取る
}

// 点:point0 と box1:OBB(有向境界ボックス：Oriented Bounding Box)の各軸の超過ベクトル result:OBB上の一番近い点を返す
inline Vector3 overLengthPoint_OBB(const Vector3& point0,
    const Vector3& centerOBB1, const Vector3& sizeOBB1, const std::array<Vector3, 3>& axisOBB1,
    Vector3* result = nullptr)
{
    Vector3 v10 = point0 - centerOBB1; // OBBの中心→pointのベクトル
    Vector3 overLength{ 0.0f, 0.0f, 0.0f }; // 最終的な長さを求めるベクトル
    float dist;
    // 各軸についてはみ出た部分のベクトルを算出 http://marupeke296.com/COL_3D_No12_OBBvsPoint.html
    for (int i = 0; i < 3; ++i)
    {
        float L = sizeOBB1.xyz[i] / 2; // 軸の中心からはしっこまでの距離
        dist = dot(v10, axisOBB1[i]); // OBBの中心→pointのベクトルとOBBの軸の内積
        if (L > 0) // L=0は計算できない
        {
            float s = dist / L; // distをLで割る
            // sの値から、はみ出した部分があればそのベクトルを加算
            float abs_s = (s < 0) ? -s : s; // sの絶対値を求める
            if (abs_s > 1) // sの絶対値が1を超えた部分がはみ出したベクトル
                overLength += (1 - abs_s) * L * axisOBB1[i]; // はみ出した部分のベクトル算出
        }

        if (result != nullptr) // resultがnullptrじゃなければOBB上の一番近い点も求める
        {   // OBB上の一番近い点の算出 http://distancevector.web.fc2.com/collision.html#OBBPoint
            if (dist > L) dist = L; // distがLの長さを超えても L の長さに制限する
            else if (dist < -L) dist = -L;
            *result += dist * axisOBB1[i]; // OBB上の一番近い点に i の軸方向の距離を反映
        }
    }
    return overLength; // 超過ベクトルを返す
}

// 点:point と box:OBB(有向境界ボックス：Oriented Bounding Box)の最短距離 result:OBB上の一番近い点を返す
inline float nearestLengthPoint_OBB(const Vector3& point,
    const Vector3& centerOBB, const Vector3& sizeOBB, const std::array<Vector3, 3>& axisOBB,
    Vector3* result = nullptr)
{   // 各軸の超過距離のベクトルの大きさを出力 http://marupeke296.com/COL_3D_No12_OBBvsPoint.html
    return overLengthPoint_OBB(point, centerOBB, sizeOBB, axisOBB, result).magnitude();
}

// 点:point と box:OBB(有向境界ボックス：Oriented Bounding Box)の最短距離の2乗 result:OBB上の一番近い点を返す
inline float sqrNearestLengthPoint_OBB(const Vector3& point,
    const Vector3& centerOBB, const Vector3& sizeOBB, const std::array<Vector3, 3>& axisOBB,
    Vector3* result = nullptr)
{   // 各軸の超過距離のベクトルの大きさの2乗を出力 http://marupeke296.com/COL_3D_No12_OBBvsPoint.html
    return overLengthPoint_OBB(point, centerOBB, sizeOBB, axisOBB, result).sqrMagnitude();
}

#endif