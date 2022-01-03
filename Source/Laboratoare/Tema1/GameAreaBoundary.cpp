#include "GameAreaBoundary.h"

GameAreaBoundary::GameAreaBoundary() {}

GameAreaBoundary::~GameAreaBoundary() {}

std::vector<RenderData> GameAreaBoundary::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// BOUNDS
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(GAME_AREA_MIN_X, GAME_AREA_MIN_Y, 0), BOUNDARY_COLOR),
			VertexFormat(glm::vec3(GAME_AREA_MIN_X, GAME_AREA_MAX_Y, 0), BOUNDARY_COLOR),
			VertexFormat(glm::vec3(GAME_AREA_MAX_X, GAME_AREA_MAX_Y, 0), BOUNDARY_COLOR),
			VertexFormat(glm::vec3(GAME_AREA_MAX_X, GAME_AREA_MIN_Y, 0), BOUNDARY_COLOR)
		};

		std::vector<unsigned short> indices = { 0, 1, 2, 3 };

		std::string name = GAME_AREA_BOUNDARY_MESH_MAIN;

		unsigned int primitive = GL_LINE_LOOP;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	return renderDatas;

}

std::vector<std::string> GameAreaBoundary::GetMeshNames() {
	return { GAME_AREA_BOUNDARY_MESH_MAIN };
}