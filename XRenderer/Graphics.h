#pragma once

#include "tgaimage.h"
#include "geometry.h"

struct Vertice
{

public :
	Vec3f worldPos;
	Vec3f screenPos;
	Vec2f uv;
};

class Graphics
{

public:
	static void DrawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
	static void DrawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color, float *zBuffer);
	static void DrawTriangleWithTexture(Vec3f * pts, Vec2f * uvs, TGAImage & image, float * zBuffer, TGAImage *texture);

};

