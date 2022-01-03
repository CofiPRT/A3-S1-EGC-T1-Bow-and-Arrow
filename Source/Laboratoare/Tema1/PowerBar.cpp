#include "PowerBar.h"

PowerBar::PowerBar() {}

PowerBar::~PowerBar() {
	this->hider->~PowerBarHider();
}

void PowerBar::RevealAt(glm::vec2 position, float percentage) {
	this->position = position;
	hider->RevealAt(position, percentage);
}

std::vector<RenderData> PowerBar::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// POWER BAR MAIN
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0,						0,						0), POWER_BAR_MED_COLOR),
			VertexFormat(glm::vec3(-POWER_BAR_LENGTH / 2,	-POWER_BAR_WIDTH / 2,	0), POWER_BAR_MIN_COLOR),
			VertexFormat(glm::vec3(-POWER_BAR_LENGTH / 2,	POWER_BAR_WIDTH / 2,	0), POWER_BAR_MIN_COLOR),
			VertexFormat(glm::vec3(POWER_BAR_LENGTH / 2,	POWER_BAR_WIDTH / 2,	0), POWER_BAR_MAX_COLOR),
			VertexFormat(glm::vec3(POWER_BAR_LENGTH / 2,	-POWER_BAR_WIDTH / 2,	0), POWER_BAR_MAX_COLOR),
		};

		std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 1 };

		std::string name = POWER_BAR_MESH_MAIN;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	RenderData::Add(renderDatas, PowerBarHider::GetRenderData());

	return renderDatas;

}

std::vector<std::string> PowerBar::GetMeshNames() {
	return { POWER_BAR_MESH_MAIN };
}

std::vector<BaseObject*> PowerBar::GetObjectsToRender() {
	return { hider };
}