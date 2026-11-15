#ifndef _MYMATH_H
#define _MYMATH_H

#include "Vector3.h"
class MyMath
{
public:
	static const float Sqrt2; 
	static const float PI; // ‰~ü—¦ 3.14159265359f;
	static const float Deg2Rad; // “x‚©‚çƒ‰ƒWƒAƒ“‚É•ÏŠ·‚·‚é’è” PI / 180f;

	/// <summary>
  /// ‰~‚Æ‰~‚ªd‚È‚Á‚Ä‚¢‚é‚©‚ğ’²‚×‚é
  /// </summary>
  /// <param name="pos1">‰~1‚Ì’†S</param>
  /// <param name="radius1">‰~1‚Ì”¼Œa</param>
  /// <param name="pos2">‰~2‚Ì’†S</param>
  /// <param name="radius2">‰~2‚Ì”¼Œa</param>
  /// <returns>d‚È‚Á‚Ä‚¢‚ê‚ÎtrueAd‚È‚Á‚Ä‚¢‚È‚¯‚ê‚Îfalse‚ğ•Ô‹p‚·‚é</returns>
	static bool CircleCircleIntersection(
		Vector3 pos1, float radius1,
		Vector3 pos2, float radius2)
	{ //[2æ‚Ì‚Ü‚Ü < ”äŠr‚·‚é‚Æ‚‘¬] https://www.stmn.tech/entry/2016/06/15/033835
		return ((pos1 - pos2).sqrMagnitude()
			< (radius1 + radius2) * (radius1 + radius2));
	}
};
#endif