// XRenderer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
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
Model *model = NULL;
Graphics *graphic = NULL;

int main(int argc, char** argv)
{
 //   std::cout << "Hello World!\n"; 
	Test test;
	test.DOTest();
	//TGAImage image(100, 100, TGAImage::RGB);
	//image.set(52, 41, red);
	//image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	//image.write_tga_file("output.tga");
	
	return 0;
	graphic = new Graphics();
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);
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
			graphic->DrawLine(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;

	return 0;
}

