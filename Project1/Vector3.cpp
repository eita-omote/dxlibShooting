#include "Vector3.h"

// Unityのstatic変数を参考に https://docs.unity3d.com/ja/current/ScriptReference/Vector3.html
Vector3 const Vector3::zero = Vector3(0.f, 0.f, 0.f);
Vector3 const Vector3::one = Vector3(1.f, 1.f, 1.f);
Vector3 const Vector3::forward = Vector3(0.f, 0.f, 1.f);
Vector3 const Vector3::back = Vector3(0.f, 0.f, -1.f);
Vector3 const Vector3::up = Vector3(0.f, 1.f, 0.f);
Vector3 const Vector3::down = Vector3(0.f, -1.f, 0.f);
Vector3 const Vector3::left = Vector3(-1.f, 0.f, 0.f);
Vector3 const Vector3::right = Vector3(1.f, 0.f, 0.f);
//[負の値の最小値] https://stackoverflow.com/questions/20016600/negative-infinity
Vector3 const Vector3::negativeInfinity = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
//[極大値]https://cpprefjp.github.io/reference/limits/numeric_limits/lowest.html
Vector3 const Vector3::positiveInfinity = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());