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

struct IShader {
	virtual ~IShader();
	virtual Vec3i vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

class Graphics
{
	Matrix projection_matrix;
	Matrix world2view_matrix;
	Matrix ndc2viewport_matrix;

	void LookAtLH(Vec3f eye_pos, Vec3f target_pos, Vec3f up);
	void Projection(float fov_y, float aspect, float z_near, float z_far);
	void Viewport(Vec2i pos, Vec2i size, int depth);

public:
	static void DrawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
	static void DrawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color, float *zBuffer);
	static void DrawTriangleWithTexture(Vec3f * pts, Vec2f * uvs, TGAImage & image, float * zBuffer, TGAImage *texture);

};

