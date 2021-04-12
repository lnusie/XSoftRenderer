#include "Test.h"
#include <iostream>
#include <string>
#include "geometry.h"
#include "graphics.h"

void Test::DOTest(void)
{
	Vec4f obj_pos = Vec4f();
	obj_pos.x = 1;
	obj_pos.y = 0;
	obj_pos.z = 3;
	obj_pos.w = 1;

	Matrix obj2world = Graphics::Object2World(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1));
	Vec4f world_pos = obj2world * obj_pos;
	std::cout << "world_pos : " << world_pos << std::endl;
	Matrix world2view = Graphics::LookAtLH(Vec3f(0, 0, -1), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	Vec4f view_pos = world2view * world_pos;
	std::cout << "view_pos : " << view_pos << std::endl;

}
