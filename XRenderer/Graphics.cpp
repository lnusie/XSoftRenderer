#include "graphics.h"
#include "tgaimage.h"
#include <cmath>
#include <algorithm>
#include "math.h"

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

IShader::~IShader()
{
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

Vec3f barycentric(Vec2i pts[], Vec2i P) {
	Vec3f v1 = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
	Vec3f v2 = Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
	Vec3f u = cross(v1,v2);

	/* `pts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void Graphics::DrawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color, float *zBuffer)
{
	int width = image.get_width();
	int height = image.get_height();

	int xMin = min(v1.x, v2.x, v3.x);
	xMin = std::max(0, xMin);
	int xMax = max(v1.x, v2.x, v3.x);
	xMax = std::min(width, xMax);
	int yMin = min(v1.y, v2.y, v3.y);
	yMin = std::max(0, yMin);
	int yMax = max(v1.y, v2.y, v3.y);
	yMax = std::min(height, yMax);
	Vec2i verts[3] = {Vec2i(v1.x, v1.y),Vec2i(v2.x, v2.y),Vec2i(v3.x, v3.y)};
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);
			Vec3f bcPoint = barycentric(verts, p);
			if (bcPoint.x < 0 || bcPoint.y < 0 || bcPoint.z < 0) continue;
			int idx = i + j * width;
			float zValue = v1.z * bcPoint.x + v2.z * bcPoint.y + v3.z * bcPoint.z;
			if (zBuffer[idx] > zValue) continue;
			zBuffer[idx] = zValue;
			image.set(i, j, color);
		}
	}
}

void Graphics::DrawTriangleWithTexture(Vec3f * pts, Vec2f * uvs, TGAImage & image, float * zBuffer, TGAImage *texture)
{
	int width = image.get_width();
	int height = image.get_height();
	Vec3f v1 = pts[0];
	Vec3f v2 = pts[1];
	Vec3f v3 = pts[2];

	int xMin = min(v1.x, v2.x, v3.x);
	xMin = std::max(0, xMin);
	int xMax = max(v1.x, v2.x, v3.x);
	xMax = std::min(width, xMax);
	int yMin = min(v1.y, v2.y, v3.y);
	yMin = std::max(0, yMin);
	int yMax = max(v1.y, v2.y, v3.y);
	yMax = std::min(height, yMax);

	Vec2i verts[3] = { Vec2i(v1.x, v1.y),Vec2i(v2.x, v2.y),Vec2i(v3.x, v3.y) };
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);
			Vec3f bcPoint = barycentric(verts, p);
			if (bcPoint.x < 0 || bcPoint.y < 0 || bcPoint.z < 0) continue;
			int idx = i + j * width;
			float zValue = v1.z * bcPoint.x + v2.z * bcPoint.y + v3.z * bcPoint.z;
			if (zBuffer[idx] > zValue) continue;
			zBuffer[idx] = zValue;
			float u = uvs[0].x * bcPoint.x + uvs[1].x * bcPoint.y + uvs[2].x * bcPoint.z;
			float v = uvs[0].y * bcPoint.x + uvs[1].y * bcPoint.y + uvs[2].y * bcPoint.z;
			v = 1 - v;
			//float u = (uvs[0].u + uvs[1].u + uvs[2].u) / 3;
			//float v = (uvs[0].v + uvs[1].v + uvs[2].v) / 3;

			TGAColor color = texture->get(u * texture->get_width(), v * texture->get_height());
			//TGAColor color = TGAColor(rand() % 255 * v, rand() % 255 * v, rand() % 255 * v, 255);
			//TGAColor color = TGAColor(rand() % 255 * u, rand() % 255 * u, rand() % 255 * u, 255);
			image.set(i, j, color);
		}
	}
}

//center_pos ：观察坐标 原点位置
void Graphics::LookAtLH(Vec3f eye_pos, Vec3f center_pos, Vec3f up)
{
	Matrix matrix = Matrix::identity();
	Vec3f& z = (center_pos - eye_pos).normalize();
	Vec3f& x = cross(z, up).normalize();
	Vec3f& y = cross(x, z).normalize();
	float eye_x = -(x * eye_pos);
	float eye_y = -(y * eye_pos);
	float eye_z = -(z * eye_pos);

	for (int i = 0; i < 3; i++)
	{
		matrix[i][0] = x[i];
		matrix[i][1] = y[i];
		matrix[i][2] = z[i];
	}
	matrix[3][0] = eye_x;
	matrix[3][1] = eye_y;
	matrix[3][2] = eye_z;
	world2view_matrix = matrix;
}

float rad2angle = 57.3;
float angle2rad = 0.01745;

void Graphics::Projection(float fov_y, float aspect, float z_near, float z_far)
{
	Matrix matrix = Matrix();
	float cot = 1 / std::tan(fov_y * angle2rad * 0.5f);
	matrix[0][0] = cot / aspect;
	matrix[1][1] = cot;
	matrix[2][2] = z_far / (z_far - z_near);
	matrix[2][3] = 1;
	matrix[3][2] = z_near * z_far / (z_near - z_far);
	projection_matrix = matrix;
}

void Graphics::Viewport(Vec2i pos, Vec2i size, int depth = 255)
{
	Matrix m = Matrix::identity();
	m[0][3] = pos.x + size.x / 2.f;
	m[1][3] = pos.y + size.y / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = size.x / 2.f;
	m[1][1] = size.y / 2.f;
	m[2][2] = depth / 2.f;
	ndc2viewport_matrix = m;
}


void DrawTriangle(Vec4f * pts, IShader & shader, TGAImage & image, TGAImage & zbuffer)
{
}