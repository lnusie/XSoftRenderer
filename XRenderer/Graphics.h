#pragma once

#include "tgaimage.h"
#include "geometry.h"

class Graphics
{

public:
	static void DrawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
	static void DrawTriangle(Vec2i v1, Vec2i v2, Vec2i v3, TGAImage &image, TGAColor color);

};

