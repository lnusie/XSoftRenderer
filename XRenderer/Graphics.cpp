#include "Graphics.h"
#include "tgaimage.h"
#include "GeometryUtility.h"
#include <cmath>
#include <algorithm>

void Graphics::DrawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

int min(int a, int b, int c)
{
	auto t = std::min(a, b);
	return std::min(t, c);
}

int max(int a, int b, int c)
{
	auto t = std::max(a, b);
	return std::max(t, c);
}

void Graphics::DrawTriangle(Vec2i v1, Vec2i v2, Vec2i v3, TGAImage &image, TGAColor color)
{
	int xMin = min(v1.x, v2.x, v3.x);
	int xMax = max(v1.x, v2.x, v3.x);
	int yMin = min(v1.y, v2.y, v3.y);
	int yMax = max(v1.y, v2.y, v3.y);

	Vec3i a, b, c;

	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec3i p(i, j, 0);
			a = v1;
			b = v2;
			c = v3;
			if (GeometryUtility::IsInTriangle(a, b, c, p))
			{
				image.set(i, j, color);
			}
		}
	}

}

