//#pragma once
//#ifndef _X_GL_H_
//#define _X_GL_H_
//
//#include "tgaimage.h"
//#include "geometry.h"
//
//extern Matrix projection_matrix;
//extern Matrix world2view_matrix;
//extern Matrix ndc2viewport_matrix;
//
//void LookAtLH(Vec3f eye_pos, Vec3f target_pos, Vec3f up);
//void Projection(float fov_y, float aspect, float z_near, float z_far);
//void Viewport(Vec2i pos, Vec2i size, int depth);
//
//struct IShader {
//	virtual ~IShader();
//	virtual Vec3i vertex(int iface, int nthvert) = 0;
//	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
//};
//
//void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer);
//
//#endif