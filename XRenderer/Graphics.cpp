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

void Graphics::DrawTriangleFrame(Vec2f * pts, TGAImage & image, TGAColor color)
{
	DrawLine(pts[0].x, pts[0].y, pts[1].x, pts[1].y, image, color);
	DrawLine(pts[1].x, pts[1].y, pts[2].x, pts[2].y, image, color);
	DrawLine(pts[2].x, pts[2].y, pts[3].x, pts[3].y, image, color);
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
	Vec3f v1 = Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]);
	Vec3f v2 = Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);
	Vec3f u = cross(v1,v2);

	/* `pts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u[2]) < 1) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]);
}

void Graphics::DrawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color, float *zBuffer)
{
	int width = image.get_width();
	int height = image.get_height();

	int xMin = min(v1[0], v2[0], v3[0]);
	xMin = std::max(0, xMin);
	int xMax = max(v1[0], v2[0], v3[0]);
	xMax = std::min(width, xMax);
	int yMin = min(v1[1], v2[1], v3[1]);
	yMin = std::max(0, yMin);
	int yMax = max(v1[1], v2[1], v3[1]);
	yMax = std::min(height, yMax);
	Vec2i verts[3] = {Vec2i(v1[0], v1[1]),Vec2i(v2[0], v2[1]),Vec2i(v3[0], v3[1])};
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);
			Vec3f bc = barycentric(verts, p);
			if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue;
			int idx = i + j * width;
			float zValue = v1[2] * bc[0] + v2[2] * bc[1] + v3[2] * bc[2];
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

	int xMin = min(v1[0], v2[0], v3[0]);
	xMin = std::max(0, xMin);
	int xMax = max(v1[0], v2[0], v3[0]);
	xMax = std::min(width, xMax);
	int yMin = min(v1[1], v2[1], v3[1]);
	yMin = std::max(0, yMin);
	int yMax = max(v1[1], v2[1], v3[1]);
	yMax = std::min(height, yMax);

	Vec2i verts[3] = { Vec2i(v1[0], v1[1]),Vec2i(v2[0], v2[1]),Vec2i(v3[0], v3[1]) };
	for (int i = xMin; i <= xMax; i++){
		for (int j = yMin; j <= yMax; j++){
			Vec2i p(i, j);
			Vec3f bc = barycentric(verts, p);
			if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue;
			int idx = i + j * width;
			float zValue = v1[2] * bc[0] + v2[2] * bc[1] + v3[2] * bc[2];
			if (zBuffer[idx] > zValue) continue;
			zBuffer[idx] = zValue;
			float u = uvs[0][0] * bc[0] + uvs[1][0] * bc[1] + uvs[2][0] * bc[2];
			float v = uvs[0][1] * bc[0] + uvs[1][1] * bc[1] + uvs[2][1] * bc[2];
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

//center_pos ：相机朝向的点
Matrix Graphics::LookAtLH(Vec3f eye_pos, Vec3f center_pos, Vec3f up)
{
	Matrix matrix = Matrix::identity();
	Vec3f& z = (eye_pos - center_pos).normalize();
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
	matrix[0][3] = eye_x;
	matrix[1][3] = eye_y;
	matrix[2][3] = eye_z;
	matrix[3][3] = 1;
	return matrix;
}


Matrix Graphics::OrthogonalProj(float width, float height, float z_near, float z_far)
{
	Matrix matrix = Matrix();
	matrix[0][0] = 2 / width;
	matrix[1][1] = 2 / height;
	matrix[2][2] = 2 / (z_near - z_far);
	matrix[2][3] = (z_near + z_far) / (z_near - z_far);
	matrix[3][3] = 1;

	return matrix;
}

float rad2angle = 57.3;
float angle2rad = 0.01745;

///fov_y:视野在y轴角度， aspect：纵横比
Matrix Graphics::Projection(float fov_y, float aspect, float z_near, float z_far)
{
	Matrix matrix = Matrix();
	float tan = 1.5 * std::tan(fov_y * 0.5f * angle2rad);
	matrix[0][0] = aspect / tan;
	matrix[1][1] = 1 / tan;
	matrix[2][2] = (z_near + z_far) / (z_far - z_near);
	matrix[2][3] = 2 * z_near * z_far / (z_near - z_far);
	matrix[3][2] = -1;
	return matrix;
}

Matrix Graphics::Viewport(Vec2i pos, Vec2i size, int depth = 255)
{
	/*Matrix m = Matrix::identity();
	m[0][3] = pos[0] + size[0] / 2.f;
	m[1][3] = pos[1] + size[1] / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = size[0] / 2.f;
	m[1][1] = size[1] / 2.f;
	m[2][2] = depth / 2.f;
	return m;*/
	Matrix m = Matrix::identity();
	m[0][3] = pos[0] + size[0] / 2.f;
	m[1][3] = pos[1] + size[1] / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = size[0] / 2.f;
	m[1][1] = size[1] / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}

Matrix Graphics::Object2World(Vec3f o, Vec3f x, Vec3f y, Vec3f z)
{
	Matrix m = Matrix::identity();
	for (int i = 0; i <= 2; i++)
	{
		m[i][0] = x[i];
		m[i][1] = y[i];
		m[i][2] = z[i];
		m[i][3] = o[i];
	}
	return m;
}

void Graphics::DrawTriangle(Vec4f * pts, IShader & shader, TGAImage & image, TGAImage & zbuffer)
{
	int width = image.get_width();
	int height = image.get_height();
	//因为已经乘以viewport矩阵，所以此时齐次除法后的坐标xy就是屏幕坐标
	Vec4f v1 = pts[0] / pts[0].w;
	Vec4f v2 = pts[1] / pts[1].w;
 	Vec4f v3 = pts[2] / pts[2].w;
	

	int xMin = min(v1[0], v2[0], v3[0]);
	xMin = std::max(0, xMin);
	int xMax = max(v1[0], v2[0], v3[0]);
	xMax = std::min(width, xMax);
	int yMin = min(v1[1], v2[1], v3[1]);
	yMin = std::max(0, yMin);
	int yMax = max(v1[1], v2[1], v3[1]);
	yMax = std::min(height, yMax);


	Vec2i verts[3] = {Vec2i(v1[0], v1[1]),Vec2i(v2[0], v2[1]),Vec2i(v3[0], v3[1])};
	Vec2f uv;
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);

			Vec3f bc = barycentric(verts, p); 
			float vz = 1 / (bc.x + bc.y + bc.z);

			if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
			float z = v1[2] * bc.x + v2[2] * bc.y + v3[2] * bc.z;
			float w = v1[3] * bc.x + v2[3] * bc.y + v3[3] * bc.z; 

			if (zbuffer.get(i,j).r > z) continue;//深度测试
			zbuffer.set(i,j, TGAColor(z, 1));

			/*uv.x = uvs[0].x * bc[0] + uvs[1].x * bc[1] + uvs[2].x * bc[2];
			uv.y = uvs[0].y * bc[0] + uvs[1].y * bc[1] + uvs[2].y * bc[2];
			uv.y = 1 - uv.y;*/

			Vec4f colorWeight;
			shader.fragment(bc, colorWeight, vz);
			TGAColor color = TGAColor(
				(int)(255 * colorWeight.x) % 255, 
				(int)(255 * colorWeight.y) % 255,
				(int)(255 * colorWeight.z) % 255,
				(int)(255 * colorWeight.w) % 255);
			image.set(i, j, color);
		}
	}

}

void Graphics::DrawTriangle(Vec4f * pts, IShader & shader, TGAImage & image, float *zBuffer)
{
	int width = image.get_width();
	int height = image.get_height();
	//if (!(pts[0].w > 2.2)) return;
	Vec4f v1 = pts[0] / pts[0].w;
	Vec4f v2 = pts[1] / pts[1].w;
	Vec4f v3 = pts[2] / pts[2].w;
	
	Matrix viewport_matrix = Graphics::Viewport(Vec2i(0, 0), Vec2i(width, height), 255);
	v1 = viewport_matrix * v1;
	v2 = viewport_matrix * v2;
	v3 = viewport_matrix * v3;

	//v1.x = v1.x * (width / 2) + (width / 2);
	//v1.y = v1.y * (height / 2) + (height / 2);
	//v2.x = v2.x * (width / 2) + (width / 2);
	//v2.y = v2.y * (height / 2) + (height / 2);
	//v3.x = v3.x * (width / 2) + (width / 2);
	//v3.y = v3.y * (height / 2) + (height / 2);
	/*TGAColor white = TGAColor(255, 255, 255, 255);

	Graphics::DrawLine(v1.x, v1.y, v2.x, v2.y, image, white);
	Graphics::DrawLine(v2.x, v2.y, v3.x, v3.y, image, white);
	Graphics::DrawLine(v1.x, v1.y, v3.x, v3.y, image, white);

	return;*/


	float rhw0 = 1 / pts[0].w;
	float rhw1 = 1 / pts[1].w;
	float rhw2 = 1 / pts[2].w;

	int xMin = min(v1[0], v2[0], v3[0]);
	xMin = std::max(0, xMin);
	int xMax = max(v1[0], v2[0], v3[0]);
	xMax = std::min(width, xMax);
	int yMin = min(v1[1], v2[1], v3[1]);
	yMin = std::max(0, yMin);
	int yMax = max(v1[1], v2[1], v3[1]);
	yMax = std::min(height, yMax);
	Vec2i verts[3] = { Vec2i(v1[0], v1[1]),Vec2i(v2[0], v2[1]),Vec2i(v3[0], v3[1]) };
	Vec2f uv;
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);

			Vec3f bc = barycentric(verts, p);
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;

		
			float rhw = rhw0 * bc.x + rhw1 * bc.y + rhw2 * bc.z;
			float w = 1 / ((rhw != 0.0f) ? rhw : 1.0f);
			
			bc.x = rhw0 * bc.x * w;
			bc.y = rhw1 * bc.y * w;
			bc.z = rhw2 * bc.z * w;

			int idx = i + j * width;
			
			if (zBuffer[idx] < w) continue;
			zBuffer[idx] = w;

			Vec4f colorWeight;
			shader.fragment(bc, colorWeight, 1);
			TGAColor color = TGAColor(
				(int)(255 * colorWeight.x) % 255,
				(int)(255 * colorWeight.y) % 255,
				(int)(255 * colorWeight.z) % 255,
				(int)(255 * colorWeight.w) % 255);
			image.set(i, j, color);
			
		}
	}

}


void Graphics::DrawTriangle(Vec4f *pts, IShader &shader, TGAImage &image)
{
	int width = image.get_width();
	int height = image.get_height();
	Vec4f v1 = pts[0] / pts[0].w;
	Vec4f v2 = pts[1] / pts[1].w;
	Vec4f v3 = pts[2] / pts[2].w;


		
	return;
	int xMin = min(v1[0], v2[0], v3[0]);
	xMin = std::max(0, xMin);
	int xMax = max(v1[0], v2[0], v3[0]);
	xMax = std::min(width, xMax);
	int yMin = min(v1[1], v2[1], v3[1]);
	yMin = std::max(0, yMin);
	int yMax = max(v1[1], v2[1], v3[1]);
	yMax = std::min(height, yMax);

	Vec2i verts[3] = { Vec2i(v1[0], v1[1]),Vec2i(v2[0], v2[1]),Vec2i(v3[0], v3[1]) };
	Vec2f uv;
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			Vec2i p(i, j);

			Vec3f bc = barycentric(verts, p);
			bc.x = bc.x / pts[0].w;
			bc.y = bc.y / pts[1].w;
			bc.z = bc.z / pts[2].w;
			float vz = 1 / (bc.x + bc.y + bc.z);

			if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
			float z = v1[2] * bc.x + v2[2] * bc.y + v3[2] * bc.z;
			float w = v1[3] * bc.x + v2[3] * bc.y + v3[3] * bc.z;

			Vec4f colorWeight;
			shader.fragment(bc, colorWeight, 1);
			TGAColor color = TGAColor(
				(int)(255 * colorWeight.x) % 255,
				(int)(255 * colorWeight.y) % 255,
				(int)(255 * colorWeight.z) % 255,
				(int)(255 * colorWeight.w) % 255);
			image.set(i, j, color);
		}
	}
}