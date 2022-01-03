#pragma once

#include <vector>

#include <Core/Engine.h>

struct RenderData {

	RenderData(std::vector<VertexFormat> vertices,
				std::vector<unsigned short> indices,
				std::string name,
				unsigned int primitive)
		: vertices(vertices), indices(indices), name(name), primitive(primitive) { }

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	std::string name;
	unsigned int primitive;

	static inline void Add(std::vector<RenderData>& a, std::vector<RenderData>& b) {
		a.insert(a.end(), b.begin(), b.end());
	}

};

