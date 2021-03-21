//#include "GeometryUtility.h"
//
//#define UseBarycentricTechnique
//
//bool SameSide(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
//{
//	auto ab = b - a;
//	auto ac = c - a;
//	auto ap = p - a;
//	auto c1 = ab ^ ac;
//	auto c2 = ab ^ ap;
//	std::cout << "c1 : " << c1;
//	std::cout << "c2 : " << c2;
//	return (c1 * c2) >= 0;
//}
//
//bool SameSideMethod(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
//{
//	return SameSide(a, b, c, p) &&
//		SameSide(b, c, a, p) &&
//		SameSide(c, a, b, p);
//}
//
//bool BarycentricMethod(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
//{
//	auto v0 = c - a;
//	auto v1 = b - a;
//	auto v2 = p - a;
//
//	float dot00 = v0 * v0;
//	float dot01 = v0 * v1;
//	float dot02 = v0 * v2;
//	float dot11 = v1 * v1;
//	float dot12 = v1 * v2;
//	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
//
//	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
//	if (u < 0 || u > 1) // if u out of range, return directly
//	{
//		return false;
//	}
//
//	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
//	if (v < 0 || v > 1) // if v out of range, return directly
//	{
//		return false;
//	}
//	return u + v <= 1;
//}
//
////BarycentricTechnique
//bool GeometryUtility::IsInTriangle(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
//{
//#if defined(UseBarycentricTechnique)
//	return BarycentricMethod(a, b, c, p);
//#else
//	return SameSideMethod(a, b, c, p);
//#endif
//}
//
