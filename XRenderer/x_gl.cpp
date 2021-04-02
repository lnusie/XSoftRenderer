#include "x_gl.h"
#include <algorithm>
#include "math.h"

//center_pos ：观察坐标 原点位置
void LookAtLH(Vec3f eye_pos, Vec3f center_pos, Vec3f up)
{
	Matrix matrix = Matrix(4);
	Vec3f z = (center_pos - eye_pos).normalize;
	Vec3f x = (z ^ up).normalize;
	Vec3f y = (x ^ z).normalize;
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

void Projection(float fov_y, float aspect, float z_near, float z_far)
{
	Matrix matrix = Matrix(4);
	float cot = 1 /	std::tan(fov_y * Math::angle2rad * 0.5f);
	matrix[0][0] = cot / aspect;
	matrix[1][1] = cot;
	matrix[2][2] = z_far / (z_far - z_near);
	matrix[2][3] = 1;
	matrix[3][2] = z_near * z_far / (z_near - z_far);
	projection_matrix = matrix;
}

void Viewport(Vec2i pos, Vec2i size)
{


}
