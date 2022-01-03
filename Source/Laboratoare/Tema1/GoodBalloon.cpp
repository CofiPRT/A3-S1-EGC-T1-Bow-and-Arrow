#include "GoodBalloon.h"

GoodBalloon::GoodBalloon(glm::vec2 position) {
	this->position = position;
}

GoodBalloon::~GoodBalloon() {}

std::vector<RenderData> GoodBalloon::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// GOOD BALLOON BODY
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0), GOOD_BALLOON_COLOR_BODY)
		};

		for (int i = 0; i <= BALLOON_BODY_TRIANGLE_COUNT; i++) {
			float t = (float)i / BALLOON_BODY_TRIANGLE_COUNT * 2 * M_PI;

			float x = BALLOON_WIDTH / 2 * cos(t);
			float y = BALLOON_HEIGHT / 2 * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), GOOD_BALLOON_COLOR_BODY));
		}

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = GOOD_BALLOON_MESH_BODY;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// GOOD BALLOON SHINE
	{
		std::vector<VertexFormat> vertices;

		float midAngle = (BALLOON_SHINE_START_ANGLE + BALLOON_SHINE_END_ANGLE) / 2;
		float angleRange = BALLOON_SHINE_END_ANGLE - BALLOON_SHINE_START_ANGLE;
		float percentRange = BALLOON_SHINE_END_PERCENT - BALLOON_SHINE_START_PERCENT;

		for (int i = 0; i <= BALLOON_BODY_TRIANGLE_COUNT; i++) {
			float t = BALLOON_SHINE_START_ANGLE + (float)i / BALLOON_BODY_TRIANGLE_COUNT * angleRange;

			float innerX = BALLOON_WIDTH / 2 * BALLOON_SHINE_START_PERCENT * cos(t);
			float innerY = BALLOON_HEIGHT / 2 * BALLOON_SHINE_START_PERCENT * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(innerX, innerY, 0), GOOD_BALLOON_COLOR_BODY));

			float percentageFactor = 1.0 - abs(1.0 - t / midAngle);
			float currPercent = BALLOON_SHINE_START_PERCENT + percentageFactor * percentRange;

			float outerX = BALLOON_WIDTH / 2 * currPercent * cos(t);
			float outerY = BALLOON_HEIGHT / 2 * currPercent * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(outerX, outerY, 0), GOOD_BALLOON_COLOR_SHINE));
		}

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = GOOD_BALLOON_MESH_SHINE;

		unsigned int primitive = GL_TRIANGLE_STRIP;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// GOOD BALLOON KNOT
	{
		float maxY = -BALLOON_HEIGHT * (1.0 / 2.0 - BALLOON_KNOT_HEIGHT_FACTOR);
		float minY = -BALLOON_HEIGHT * (1.0 / 2.0 + BALLOON_KNOT_HEIGHT_FACTOR);
		float x = BALLOON_WIDTH * BALLOON_KNOT_WIDTH_FACTOR;

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0,	maxY, 0), GOOD_BALLOON_COLOR_KNOT),
			VertexFormat(glm::vec3(-x,	minY, 0), GOOD_BALLOON_COLOR_KNOT),
			VertexFormat(glm::vec3(x,	minY, 0), GOOD_BALLOON_COLOR_KNOT),
		};

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = GOOD_BALLOON_MESH_KNOT;

		unsigned int primitive = GL_TRIANGLES;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// GOOD BALLOON STRING
	{
		float minY = -BALLOON_HEIGHT * (1.0 / 2.0 + BALLOON_KNOT_HEIGHT_FACTOR);
		float width = BALLOON_WIDTH * BALLOON_STRING_WIDTH_FACTOR;
		float height = BALLOON_HEIGHT * BALLOON_STRING_HEIGHT_FACTOR;

		std::vector<glm::vec3> points = {
			glm::vec3(0, 0, 0),
			glm::vec3(0, -height, 0),
			glm::vec3(width, -height, 0),
			glm::vec3(width, -2 * height, 0)
		};

		std::vector<VertexFormat> vertices;

		for (int i = 0; i < points.size(); i++) {
			glm::vec3 vertex = points[i];
			vertex = vertex * Transform2D::Rotate(BALLOON_STRING_ROTATION);
			vertex = vertex + glm::vec3(0, minY, 0);

			vertices.push_back(VertexFormat(vertex, GOOD_BALLOON_COLOR_STRING));
		}

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = GOOD_BALLOON_MESH_STRING;

		unsigned int primitive = GL_LINE_STRIP;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	return renderDatas;

}

std::vector<std::string> GoodBalloon::GetMeshNames() {

	std::vector<std::string> meshNames;
	
	if (IsAlive()) {
		meshNames.push_back(GOOD_BALLOON_MESH_SHINE);
		meshNames.push_back(GOOD_BALLOON_MESH_BODY);
	}

	meshNames.push_back(GOOD_BALLOON_MESH_KNOT);
	meshNames.push_back(GOOD_BALLOON_MESH_STRING);

	return meshNames;

}

int GoodBalloon::GetScore() {
	return GOOD_BALLOON_SCORE;
}