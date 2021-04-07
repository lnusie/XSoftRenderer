#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> normals_;
	std::vector<std::vector<int> > faces_;
	std::vector<Vec2f> uvs_;
	std::vector<std::vector<int>> uvidxs_;

public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	std::vector<int> uvidx(int i);
	Vec3f vert(int i);
	Vec3f normal(int i);
	Vec2f uv(int i);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__
