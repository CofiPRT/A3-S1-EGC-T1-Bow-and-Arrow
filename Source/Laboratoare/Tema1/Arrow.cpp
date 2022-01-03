#include "Arrow.h"
#include "Bow.h"

Arrow::Arrow() {}

Arrow::~Arrow() {}

void Arrow::UpdateFlight(double deltaTimeSeconds) {

	// make the tip follow the trajectory, but slower;
	// this is a direction, so normalize the result
	rotation = glm::normalize(rotation + glm::normalize(velocity));

	BaseObject::UpdateTrajectory(deltaTimeSeconds);

	float minY = ARROW_Y_MIN - ARROW_PENETRATION_LENGTH;
	if (position.y <= minY) {
		position.y = minY;
		StartStuckAnimation();
		return;
	}

	float maxY = ARROW_Y_MAX + ARROW_PENETRATION_LENGTH;
	if (position.y >= maxY) {
		position.y = maxY;
		StartStuckAnimation();
		return;
	}

	float maxX = ARROW_X_MAX + ARROW_PENETRATION_LENGTH;
	if (position.x >= maxX) {
		position.x = maxX;
		StartStuckAnimation();
		return;
	}

}

void Arrow::UpdateStuckAnimation(double deltaTimeSeconds) {

	timeStuck += deltaTimeSeconds;

	if (timeStuck >= stuckAnimationLength) {
		currState = ArrowState::IDLE;
		rotation = initialRotation;

		return;
	}

	// depending on impact velocity
	float controlFunc = timeStuck + stuckAnimationLength;
	float mainFunc = stuckAnimationMagnitude * sin(stuckAnimationFrequency * controlFunc);
	float limitFunc = fmax(0.0, stuckAnimationLength - timeStuck);

	float deviation = mainFunc / controlFunc * limitFunc;

	float originalAngle = atan2(initialRotation.y, initialRotation.x);
	rotation = glm::normalize(glm::vec2(cos(originalAngle + deviation), sin(originalAngle + deviation)));

}

void Arrow::StartStuckAnimation() {

	currState = ArrowState::STUCK;

	float g = GetGravitationalAcceleration();
	float impactForce = log(velocity.x * velocity.x + velocity.y * velocity.y) / log(g);
	stuckAnimationMagnitude = impactForce * ARROW_STUCK_ANIMATION_MAGNITUDE_FACTOR;
	stuckAnimationFrequency = impactForce * ARROW_STUCK_ANIMATION_FREQUENCY_FACTOR;
	stuckAnimationLength = impactForce * ARROW_STUCK_ANIMATION_LENGTH_FACTOR;

	initialRotation = rotation;
	timeStuck = 0;
	velocity = glm::vec2(0, 0);

}

void Arrow::UpdateAlive(double deltaTimeSeconds) {

	switch (currState) {
	case ArrowState::LOADED:
		UpdateLoadPosition();
		break;
	case ArrowState::STUCK:
		UpdateStuckAnimation(deltaTimeSeconds);
		break;
	case ArrowState::FLYING:
		UpdateFlight(deltaTimeSeconds);
		break;
	default:
		break;
	}
}

void Arrow::Launch(glm::vec2 velocity) {

	if (!IsLoaded())
		return;

	this->velocity = velocity;
	currState = ArrowState::FLYING;

}

void Arrow::UpdateLoadPosition() {

	rotation = bow->GetRotation();
	position = bow->GetPosition() + rotation * glm::vec2(ARROW_BODY_LENGTH);

}

void Arrow::LoadBy(Bow *bow) {
	
	this->bow = bow;
	currState = ArrowState::LOADED;

}

bool Arrow::IsLoaded() {
	return currState == ArrowState::LOADED;
}

bool Arrow::IsFlying() {
	return currState == ArrowState::FLYING;
}

bool Arrow::IsStuck() {
	return currState == ArrowState::STUCK;
}

bool Arrow::IsIdle() {
	return currState == ArrowState::IDLE;
}

bool Arrow::IsInBounds() {
	return true;
}

std::vector<RenderData> Arrow::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// ARROW BODY
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(-ARROW_TIP_SHADOW_LENGTH,	ARROW_BODY_WIDTH / 2,	0), ARROW_COLOR_BODY),
			VertexFormat(glm::vec3(-ARROW_BODY_LENGTH,			ARROW_BODY_WIDTH / 2,	0), ARROW_COLOR_BODY),
			VertexFormat(glm::vec3(-ARROW_BODY_LENGTH,			-ARROW_BODY_WIDTH / 2,	0), ARROW_COLOR_BODY),
			VertexFormat(glm::vec3(-ARROW_TIP_SHADOW_LENGTH,	-ARROW_BODY_WIDTH / 2,	0), ARROW_COLOR_BODY),
		};

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = ARROW_MESH_BODY;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// ARROW TAIL
	{
		float tailStartX = ARROW_BODY_LENGTH;
		float tailStartY = ARROW_TAIL_START_WIDTH / 2;

		float tailEndX = ARROW_BODY_LENGTH + ARROW_TAIL_LENGTH;
		float tailEndY = ARROW_TAIL_END_WIDTH / 2;

		float tailMiddleX = ARROW_BODY_LENGTH + ARROW_TAIL_LENGTH * 3 / 4;

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(-tailStartX,		0,				0), ARROW_COLOR_TAIL),
			VertexFormat(glm::vec3(-tailStartX,		tailStartY,		0), ARROW_COLOR_TAIL),
			VertexFormat(glm::vec3(-tailEndX,		tailEndY,		0), ARROW_COLOR_TAIL),
			VertexFormat(glm::vec3(-tailMiddleX,	0,				0), ARROW_COLOR_TAIL),
			VertexFormat(glm::vec3(-tailEndX,		-tailEndY,		0), ARROW_COLOR_TAIL),
			VertexFormat(glm::vec3(-tailStartX,		-tailStartY,	0), ARROW_COLOR_TAIL),
		};

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = ARROW_MESH_TAIL;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// ARROW TIP
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(-ARROW_TIP_SHADOW_LENGTH, 0, 0), ARROW_COLOR_TIP),
		};

		std::vector<unsigned short> indices;

		for (int i = 0; i <= ARROW_TIP_ARC_TRIANGLE_COUNT; i++) {
			float t = -M_PI_2 + (float)i / ARROW_TIP_ARC_TRIANGLE_COUNT * M_PI;

			float x = -ARROW_TIP_ARC_OFFSET_X + ARROW_TIP_ARC_RADIUS_X * cos(t);
			float y = (ARROW_TIP_SHADOW_OUTER_WIDTH + ARROW_TIP_SHADOW_INNER_WIDTH) / 4 * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), ARROW_COLOR_TIP));
		}

		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = ARROW_MESH_TIP;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// ARROW TIP SHADOW
	{
		float tipShadowEndX = ARROW_TIP_ARC_OFFSET_X;
		float tipShadowMiddleX = ARROW_TIP_SHADOW_LENGTH;

		float tipShadowEndOuterY = ARROW_TIP_SHADOW_OUTER_WIDTH / 2;
		float tipShadowEndInnerY = ARROW_TIP_SHADOW_INNER_WIDTH / 2;

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0,					0,						0), ARROW_COLOR_TIP_SHADOW),
			VertexFormat(glm::vec3(-tipShadowEndX,		-tipShadowEndOuterY,	0), ARROW_COLOR_TIP_SHADOW),
			VertexFormat(glm::vec3(-tipShadowEndX,		-tipShadowEndInnerY,	0), ARROW_COLOR_TIP_SHADOW),
			VertexFormat(glm::vec3(-tipShadowMiddleX,	0,						0), ARROW_COLOR_TIP_SHADOW),
			VertexFormat(glm::vec3(-tipShadowEndX,		tipShadowEndInnerY,		0), ARROW_COLOR_TIP_SHADOW),
			VertexFormat(glm::vec3(-tipShadowEndX,		tipShadowEndOuterY,		0), ARROW_COLOR_TIP_SHADOW)
		};

		std::vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = ARROW_MESH_TIP_SHADOW;

		unsigned int primitive = GL_TRIANGLE_FAN;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	return renderDatas;

}

std::vector<std::string> Arrow::GetMeshNames() {
	return {
		ARROW_MESH_TIP_SHADOW,
		ARROW_MESH_TIP,
		ARROW_MESH_TAIL,
		ARROW_MESH_BODY,
	};
}