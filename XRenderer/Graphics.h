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
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, Vec4f &color, float z) = 0;
	TGAColor flat_col;
	
};

class Graphics
{
public:
	static Matrix LookAtLH(Vec3f eye_pos, Vec3f target_pos, Vec3f up);
	static Matrix Projection(float fov_y, float aspect, float z_near, float z_far);
	static Matrix OrthogonalProj(float width, float height, float z_near, float z_far);
	static Matrix Viewport(Vec2i pos, Vec2i size, int depth);
	static Matrix Object2World(Vec3f pos, Vec3f x, Vec3f y, Vec3f z);
	static void DrawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
	static void DrawTriangleFrame(Vec2f* pts, TGAImage &image, TGAColor color);
	static void DrawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color, float *zBuffer);
	static void DrawTriangleWithTexture(Vec3f * pts, Vec2f * uvs, TGAImage & image, float * zBuffer, TGAImage *texture);
	static void DrawTriangle(Vec4f * pts, IShader & shader, TGAImage & image, TGAImage & zbuffer);
	static void DrawTriangle(Vec4f * pts, IShader & shader, TGAImage & image, float * zBuffer);
	static void DrawTriangle(Vec4f *pts, IShader &shader, TGAImage &image);
};

