#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), uvs_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
		//istringstream对象可以绑定一行字符串，然后以空格为分隔符把该行分隔开来。
        std::istringstream iss(line.c_str());
        char trash;
		// line.compare(0, 2, "v ")：从line的第0位开始2位与"v "比较，相等返回0，不相等返回1！
		//而c++ 中 ！非0的数 都为 1
        if (!line.compare(0, 2, "v ")) //v 0.000283538 -1 0.286843
		{
			// iss >> trash ：从 iss中取出当前的string
            iss >> trash; //弹出"v"
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } 
		else if (!line.compare(0, 2, "f ")) //f 24/1/24 25/2/25 26/3/26
		{
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
		else if (!line.compare(0, 2, "vt "))
		{
			Vec2f uv;
			iss >> trash;
			for (int i = 0; i < 2; i++) iss >> uv.raw[i];
			uvs_.push_back(uv);
		}
    }
    //std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

