#include "Shuriken.h"

Shuriken::Shuriken(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
}

Shuriken::~Shuriken() {}

bool Shuriken::TestCollision(Arrow* arrow) {

	if (!IsAlive())
		return false;

	// simply test if the arrow tip is inside the shuriken's bounds
	bool result = glm::distance(position, arrow->GetPosition()) <= SHURIKEN_BLADE_LENGTH;

	if (result) {
		velocity += arrow->GetVelocity() * glm::vec2(SHURIKEN_IMPACT_ARROW_FACTOR);
		currState = ObjectState::DECAYING;
	}

	return result;

}

bool Shuriken::TestCollision(Bow* bow) {

	if (!IsAlive())
		return false;

	// test shuriken center
	glm::vec2 direction = glm::normalize(position - bow->GetPosition());
	float relativeAngle = acos(glm::dot(direction, bow->GetRotation()));

	float x = BOW_ARC_WIDTH / 2 * cos(relativeAngle);
	float y = BOW_ARC_HEIGHT / 2 * sin(relativeAngle);

	float bowRadius = sqrt(x * x + y * y);

	bool result = glm::distance(position, bow->GetPosition()) <= bowRadius + SHURIKEN_BLADE_LENGTH;

	if (result) {
		velocity *= -direction * glm::vec2(SHURIKEN_IMPACT_BOW_FACTOR);
		currState = ObjectState::DECAYING;
	}

	return result;

}

void Shuriken::UpdateAlive(double deltaTimeSeconds) {

	BaseObject::UpdateTrajectory(deltaTimeSeconds);

	// update rotation
	glm::vec3 rotation3 = glm::vec3(rotation, 0) * Transform2D::Rotate(-deltaTimeSeconds * SHURIKEN_ROTATION_SPEED);
	rotation = glm::normalize(glm::vec2(rotation3));

}

float Shuriken::GetDecayLength() {
	return SHURIKEN_DECAY_LENGTH;
}

int Shuriken::GetScore() {
	return SHURIKEN_SCORE;
}

bool Shuriken::IsInBounds() {
	return position.x >= GAME_AREA_MIN_X - SHURIKEN_BLADE_LENGTH;
}

std::vector<RenderData> Shuriken::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// SHURIKEN CENTER CIRCLE
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0), SHURIKEN_CENTER_COLOR)
		};

		std::vector<unsigned short> indices;

		for (int i = 0; i <= SHURIKEN_CENTER_TRIANGLE_COUNT; i++) {
			float t = (float)i / SHURIKEN_CENTER_TRIANGLE_COUNT * 2 * M_PI;

			float x = SHURIKEN_CENTER_RADIUS * cos(t);
			float y = SHURIKEN_CENTER_RADIUS * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), SHURIKEN_CENTER_COLOR));
		}

		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = SHURIKEN_MESH_CENTER;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// SHURIKEN BLADES
	{
		float tipCoord = SHURIKEN_BLADE_LENGTH;
		float cornerCoord = SHURIKEN_BLADE_LENGTH * sqrt(2.0) / 4.0;

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0,				0,				0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(cornerCoord,		cornerCoord,	0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(0,				tipCoord,		0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(-cornerCoord,	cornerCoord,	0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(-tipCoord,		0,				0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(-cornerCoord,	-cornerCoord,	0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(0,				-tipCoord,		0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(cornerCoord,		-cornerCoord,	0), SHURIKEN_BLADE_COLOR),
			VertexFormat(glm::vec3(tipCoord,		0,				0), SHURIKEN_BLADE_COLOR),
		};

		std::vector<unsigned short> indices = {
			0, 1, 2,
			0, 3, 4,
			0, 5, 6,
			0, 7, 8
		};

		std::string name = SHURIKEN_MESH_BLADES;

		unsigned int primitive = GL_TRIANGLES;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	return renderDatas;

}

std::vector<std::string> Shuriken::GetMeshNames() {
	return { SHURIKEN_MESH_CENTER, SHURIKEN_MESH_BLADES };
}