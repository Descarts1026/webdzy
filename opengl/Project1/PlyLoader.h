#pragma once

#include <string>
#include <vector>

class Vec3 {
public:
	Vec3() :x(0), y(0), z(0) {};
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	float x, y, z;
};

class PlyLoader {
public:
	PlyLoader(const std::string& path);
	
	std::vector<Vec3> vertices;
	std::vector<Vec3> colors;
	std::vector<Vec3> normals;
	
};