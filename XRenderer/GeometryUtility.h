#pragma once

#include "geometry.h"


class GeometryUtility
{
	public:
		//static bool IsInTriangle(Vec3f a, Vec3f b, Vec3f c, Vec3f p);
		template<typename T> static bool IsInTriangle(Vec3<T> a, Vec3<T> b, Vec3<T> c, Vec3<T> p);
};

//Vec3f barycentric(Vec2i pts[], Vec2i P) {
//	Vec3f v1 = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
//	Vec3f v2 = Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
//	Vec3f u = v1 ^ v2;
//
//	/* `pts` and `P` has integer value as coordinates
//	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
//	   triangle is degenerate, in this case return something with negative coordinates */
//	if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
//	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//}


template<typename T>
inline static bool GeometryUtility::IsInTriangle(Vec3<T> a, Vec3<T> b, Vec3<T> c, Vec3<T> p)
{
	return false;
}

//void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color) {
//	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
//	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 2; j++) {
//			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
//			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
//		}
//	}
//	Vec3f P;
//	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
//		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
//			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
//			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
//			P.z = 0;
//			for (int i = 0; i < 3; i++) P.z += pts[i].z * bc_screen[i];
//			if (zbuffer[int(P.x + P.y*width)] < P.z) {
//				zbuffer[int(P.x + P.y*width)] = P.z;
//				image.set(P.x, P.y, color);
//			}
//		}
//	}
//}