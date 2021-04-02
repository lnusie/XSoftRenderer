#pragma once
#include "tgaimage.h"
#include "geometry.h"

extern Matrix projection_matrix;
extern Matrix world2view_matrix;

void LookAtLH(Vec3f eye_pos, Vec3f target_pos, Vec3f up);
void Projection(float fov_y, float aspect, float z_near, float z_far);
void Viewport(Vec2i pos, Vec2i size);





