﻿// XRenderer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <limits>
#include <iostream>
#include<vector>
#include "Math.h"
#include "Test.h"
#include "tgaimage.h"
#include "model.h"
#include "Graphics.h"
constexpr int width = 800; // output image size
constexpr int height = 800;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

template<class T>
int length(T& arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}

Model *model = NULL;
TGAImage *texture = NULL;

void DrawFlatTriangle(Model *model, TGAImage &image);
void DrawWireframe(Model *model, TGAImage &image);
void DrawModelWithSimpleLight(Model *model, TGAImage &image);
void DrawModelWithTexture(Model *model, TGAImage &image, TGAImage *texture);
void DrawModelWithShader(Model * model, IShader &shader, TGAImage &image, TGAImage &zbuffer, Matrix &viewport_matrix);

struct TestShader : IShader
{
public :
	Model *model_;
	Matrix object2world_matrix;
	Matrix projection_matrix;
	Matrix world2view_matrix;
	
	virtual Vec4f vertex(int iface, int nthvert) 
	{
		std::vector<int> face = model_->face(iface);
		int vertex_idx = face[nthvert];
		Vec3f vertex = model_->vert(vertex_idx);
		Vec4f v4 = Vec4f(vertex.x, vertex.y, vertex.z, 1);
		Vec2f uv = model_->uv(vertex_idx);
		Vec3f normal = model_->normal(vertex_idx);
		//return projection_matrix * world2view_matrix * object2world_matrix * v4;
		Vec4f world_pos = object2world_matrix * v4;
		Vec4f view_pos = world2view_matrix * world_pos;
		Vec4f projection_pos = projection_matrix * view_pos;
		return projection_pos;
	}
	virtual bool fragment(Vec3f bar, TGAColor & color)
	{
		return false;
	}
};


int main(int argc, char** argv)
{
	Test test;
	test.DOTest();

	if (argc >= 2) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}

	if (argc >= 3)
	{
		texture = new TGAImage();
		texture->read_tga_file(argv[1]);
	}
	else
	{
		texture = new TGAImage();
		texture->read_tga_file("textures/african_head_diffuse.tga");
	}

	TGAImage image(width, height, TGAImage::RGB);
	TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
	//DrawWireframe(model, image);
	//DrawFlatTriangle(model, image); 
	//DrawModelWithSimpleLight(model, image);
	//DrawModelWithTexture(model, image, texture);
	TestShader shader = TestShader();
	shader.model_ = model;
	shader.object2world_matrix = Graphics::Object2World(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1));
	shader.world2view_matrix = Graphics::LookAtLH(Vec3f(0, 0, -10), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	shader.projection_matrix = Graphics::Projection(60, 1, 1, 100);
	Matrix viewport_matrix = Graphics::Viewport(Vec2i(0, 0), Vec2i(800, 800), 255);
	DrawModelWithShader(model, shader, image, zbuffer, viewport_matrix);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	delete texture;
	return 0;
}

Vec3f World2Screen(Vec3f v)
{
	return Vec3f((v.x + 1) * width / 2, (v.y + 1) * height / 2, v.z);
}

float* CreateZBuffer(int width, int height)
{
	float* zBuffer = new float[width * height];
	for (int i = width * height - 1; i >= 0; i--)
	{
		zBuffer[i] = -std::numeric_limits<float>::max();
	}
	return zBuffer;
}

//线框
void DrawWireframe(Model *model, TGAImage &image)
{
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			// obj保存的坐标范围是[-1,1],需要转为[0,1]
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			Graphics::DrawLine(x0, y0, x1, y1, image, white);
		}
	}
}

//平面填充
void DrawFlatTriangle(Model *model, TGAImage &image)
{
	float* zBuffer = CreateZBuffer(width, height);
	for (int i = 0; i < model->nfaces(); i++) {
		auto color = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
		std::vector<int> face = model->face(i);
		Vec3f v0 = World2Screen(model->vert(face[0]));
		Vec3f v1 = World2Screen(model->vert(face[1]));
		Vec3f v2 = World2Screen(model->vert(face[2]));
		Graphics::DrawTriangle(v0, v1, v2, image, color, zBuffer);
	}
}

//简单光照
void DrawModelWithSimpleLight(Model *model, TGAImage &image)
{
	float* zBuffer = CreateZBuffer(width, height);
	Vec3f lightDir = Vec3f(0, 0, 1);
	lightDir.normalize();
	Vec3f screen_coords[3];
	Vec3f world_coords[3];
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v = model->vert(face[j]);
			world_coords[j] = v;
			screen_coords[j] = World2Screen(v);
		}
		auto a = world_coords[1] - world_coords[0];
		auto b = world_coords[2] - world_coords[0];
		Vec3f& normal = cross(a,b).normalize();
		auto intensity = normal * lightDir;
		if (intensity > 0)
		{
			TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);
			Graphics::DrawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, color, zBuffer);
		}
	}
}

//带贴图
void DrawModelWithTexture(Model *model, TGAImage &image, TGAImage *texture)
{
	float* zBuffer = CreateZBuffer(width, height);
	Vec3f lightDir = Vec3f(0, 0, 1);
	lightDir.normalize();
	Vec3f screen_coords[3];
	Vec3f world_coords[3];
	Vec2f uvs[3];
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		std::vector<int> uvidxs = model->uvidx(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v = model->vert(face[j]);
			world_coords[j] = v;
			screen_coords[j] = World2Screen(v);
			uvs[j] = model->uv(uvidxs[j]);
		}
		auto a = world_coords[1] - world_coords[0];
		auto b = world_coords[2] - world_coords[0];
		Vec3f& normal = cross(a, b).normalize();
		auto intensity = normal * lightDir;
		if (intensity > 0)
		{
			//TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);
			Graphics::DrawTriangleWithTexture(screen_coords, uvs, image, zBuffer, texture);
		}
	}
}

void DrawModelWithShader(Model * model, IShader &shader, TGAImage &image, TGAImage &zbuffer, Matrix &viewport_matrix)
{
	Vec3f lightDir = Vec3f(0, 0, 1);
	lightDir.normalize();
	for (int i = 0; i < model->nfaces(); i++) 
	{
		Vec4f screen_coords[3];
		for (int j = 0; j < 3; j++)
		{
			screen_coords[j] = shader.vertex(i, j);
		}
		Graphics::DrawTriangle(screen_coords, shader, image, zbuffer, viewport_matrix);
	}
}
