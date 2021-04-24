#include "Test.h"
#include <iostream>
#include <string>
#include "geometry.h"
#include "graphics.h"

void Test::DOTest(void)
{
	Vec4f obj_pos = Vec4f();
	obj_pos.x = 1;
	obj_pos.y = 1;
	obj_pos.z = 0;
	obj_pos.w = 1;

	Matrix obj2world = Graphics::Object2World(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1));
	Vec4f world_pos = obj2world * obj_pos;
	std::cout << "world_pos : " << world_pos << std::endl;
	Matrix world2view = Graphics::LookAtRH(Vec3f(0, 0, -2), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	Vec4f view_pos = world2view * world_pos;
	std::cout << "view_pos  : " << view_pos << std::endl;
	Matrix projection_matrix = Graphics::Projection(120, 1, 1, 10);
	Vec4f proj_pos = projection_matrix * view_pos;
	std::cout << "proj_pos  : " << proj_pos << std::endl;
	proj_pos = proj_pos / proj_pos.w;
	Matrix orthogonal_matrix = Graphics::OrthogonalProj(2, 2, 1, 10);
	Vec4f orth_pos = orthogonal_matrix * view_pos;
	std::cout << "orth_pos  : " << proj_pos << std::endl;
	Matrix viewport_matrix = Graphics::Viewport(Vec2i(0, 0), Vec2i(1024, 1024), 255);
	Vec4f viewport_pos = viewport_matrix * proj_pos;
	std::cout << "viewport_pos  : " << viewport_pos << std::endl;


}
