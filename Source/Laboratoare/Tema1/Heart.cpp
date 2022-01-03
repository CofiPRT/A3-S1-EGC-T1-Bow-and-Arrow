#include "Heart.h"

Heart::Heart(glm::vec2 position) {

	decayJumpDist = std::uniform_real_distribution<float>(
		HEART_JUMP_MIN_ANGLE, HEART_JUMP_MAX_ANGLE
	);

	this->position = position;

}

Heart::~Heart() {}

float Heart::GetGravitationalAcceleration() {
	return !IsDecaying() ? 0 : BaseObject::GetGravitationalAcceleration();
}

void Heart::StartWiggle() {

	if (!IsWiggling())
		initialPosition = position;

	wiggling = true;
	wiggleTime = 0;

}

void Heart::StartDecay() {

	float deviation = decayJumpDist(RandomUtils::mt);
	glm::vec3 velocity3 = glm::vec3(0, HEART_JUMP_VELOCITY, 0) * Transform2D::Rotate(deviation);
	velocity = glm::vec2(velocity3);

	BaseObject::StartDecay();

}

void Heart::UpdateWiggle(double deltaTimeSeconds) {

	wiggleTime += deltaTimeSeconds;

	// stop wiggling
	if (wiggleTime >= HEART_WIGGLE_LENGTH) {
		wiggling = false;
		position = initialPosition;
		return;
	}

	// calculate wiggle deviation
	float deviation = HEART_WIGGLE_MAGNITUDE * asin(sin(HEART_WIGGLE_FREQUENCY * wiggleTime));
	position = initialPosition + glm::vec2(deviation, 0);

}

void Heart::UpdateAlive(double deltaTimeSeconds) {

	if (IsWiggling())
		UpdateWiggle(deltaTimeSeconds);

}

void Heart::UpdateDecay(double deltaTimeSeconds) {

	BaseObject::UpdateTrajectory(deltaTimeSeconds);

	glm::vec3 rotation3 = glm::vec3(rotation, 0);
	rotation = glm::normalize(rotation3 * Transform2D::Rotate(deltaTimeSeconds));

}

bool Heart::IsInBounds() {
	return position.y >= GAME_AREA_MIN_Y - HEART_HEIGHT / 2;
}

std::vector<RenderData> Heart::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// HEART BODY AND STROKE
	{
		std::vector<VertexFormat> verticesStroke;
		std::vector<VertexFormat> verticesBody = {
			VertexFormat(glm::vec3(0), HEART_COLOR_BODY)
		};

		std::vector<unsigned short> indicesStroke;
		std::vector<unsigned short> indicesBody = { 0 };

		for (int i = 0; i <= HEART_BODY_TRIANGLE_COUNT; i++) {
			float t = (float)i / HEART_BODY_TRIANGLE_COUNT * 2 * M_PI;

			// don't ask... this is the equation of a beautiful heart
			float x = HEART_WIDTH / 2.0 * sin(t) * sin(t) * sin(t);
			float y = HEART_HEIGHT / 34.0 * (13.0 * cos(t) - 5.0 * cos(2.0 * t) - 2.0 * cos(3.0 * t) - cos(4.0 * t));

			verticesStroke.push_back(VertexFormat(glm::vec3(x, y, 0), HEART_COLOR_STROKE));
			verticesBody.push_back(VertexFormat(glm::vec3(x, y, 0), HEART_COLOR_BODY));

			indicesStroke.push_back(indicesStroke.size());
			indicesBody.push_back(indicesBody.size());
		}

		std::string nameStroke = HEART_MESH_STROKE;
		std::string nameBody = HEART_MESH_BODY;

		unsigned int primitiveStroke = GL_LINE_LOOP;
		unsigned int primitiveBody = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(verticesStroke, indicesStroke, nameStroke, primitiveStroke));
		renderDatas.push_back(RenderData(verticesBody, indicesBody, nameBody, primitiveBody));
	}

	return renderDatas;

}

std::vector<std::string> Heart::GetMeshNames() {
	return {
		HEART_MESH_STROKE,
		HEART_MESH_BODY
	};
}

bool Heart::IsWiggling() {
	return wiggling;
}