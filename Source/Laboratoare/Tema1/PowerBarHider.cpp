#include "PowerBarHider.h"

PowerBarHider::PowerBarHider() {}

PowerBarHider::~PowerBarHider() {}

void PowerBarHider::RevealAt(glm::vec2 position, float percentage) {
	this->position = position + glm::vec2(percentage * POWER_BAR_LENGTH / 2, 0);
	this->scale = glm::vec2(1.0 - percentage, 1.0);
}

std::vector<RenderData> PowerBarHider::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// POWER BAR MAIN
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(-POWER_BAR_LENGTH / 2,	-POWER_BAR_WIDTH / 2,	0), POWER_BAR_HIDER_COLOR),
			VertexFormat(glm::vec3(-POWER_BAR_LENGTH / 2,	POWER_BAR_WIDTH / 2,	0), POWER_BAR_HIDER_COLOR),
			VertexFormat(glm::vec3(POWER_BAR_LENGTH / 2,	POWER_BAR_WIDTH / 2,	0), POWER_BAR_HIDER_COLOR),
			VertexFormat(glm::vec3(POWER_BAR_LENGTH / 2,	-POWER_BAR_WIDTH / 2,	0), POWER_BAR_HIDER_COLOR),
		};

		std::vector<unsigned short> indices = { 0, 1, 2, 3 };

		std::string name = POWER_BAR_HIDER_MESH_MAIN;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	return renderDatas;

}

std::vector<std::string> PowerBarHider::GetMeshNames() {
	return { POWER_BAR_HIDER_MESH_MAIN };
}