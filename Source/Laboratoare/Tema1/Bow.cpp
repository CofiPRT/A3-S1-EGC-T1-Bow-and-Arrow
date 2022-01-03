#include "Bow.h"

Bow::Bow() {
	position = glm::vec2(BOW_START_X, BOW_START_Y);
	LoadArrow();
}

Bow::~Bow() {
	this->arrow->~Arrow();
	this->powerBar->~PowerBar();
}

void Bow::UpdateDraw(double deltaTimeSeconds) {

	if (drawFactor == 1.0)
		return;

	drawFactor = fmin(1.0, drawFactor + BOW_DRAW_SPEED * deltaTimeSeconds);

}

void Bow::UpdateDirection() {
	rotation = glm::normalize(target - position);
}

void Bow::UpdatePowerBar() {

	float totalOffset = BOW_ARC_HEIGHT / 2 + BOW_POWER_BAR_OFFSET;

	// place the power bar wherever there is room for it (ideally below the bow)
	if (position.y - totalOffset - POWER_BAR_WIDTH > 0)
		powerBar->RevealAt(position - glm::vec2(0, totalOffset), drawFactor);
	else
		powerBar->RevealAt(position + glm::vec2(0, totalOffset), drawFactor);

}

void Bow::UpdateAlive(double deltaTimeSeconds) {

	UpdateDirection();

	switch (currState) {
	case BowState::DRAWING:
		UpdateDraw(deltaTimeSeconds);
		UpdatePowerBar();
		break;
	default:
		break;
	}

}

void Bow::SetTarget(glm::vec2 target) {

	// keep the target in game boundaries
	if (target.x < position.x || target.x > GAME_AREA_MAX_X)
		return;

	if (target.y < GAME_AREA_MIN_Y || target.y > GAME_AREA_MAX_Y)
		return;

	this->target = target;
	UpdateDirection();

}

void Bow::Draw() {
	
	if (!IsLoaded())
		return;

	drawFactor = 0;
	currState = BowState::DRAWING;
}

void Bow::Fire() {
	
	if (!IsDrawing())
		return;

	glm::vec2 velocity = rotation * glm::vec2(drawFactor * BOW_POWER);

	arrow->Launch(velocity);
	currState = BowState::UNLOADED;

}

void Bow::LoadArrow() {
	
	arrow->LoadBy(this);
	currState = BowState::LOADED;

}

void Bow::MoveUp(double deltaTime) {
	
	float maxY = GAME_AREA_MAX_Y - BOW_ARC_HEIGHT / 2;

	position.y = fmin(maxY, position.y + deltaTime * BOW_MOVEMENT_SPEED);

}

void Bow::MoveDown(double deltaTime) {

	float minY = GAME_AREA_MIN_Y + BOW_ARC_HEIGHT / 2;

	position.y = fmax(minY, position.y - deltaTime * BOW_MOVEMENT_SPEED);

}

bool Bow::IsUnloaded() {
	return currState == BowState::UNLOADED;
}

bool Bow::IsLoaded() {
	return currState == BowState::LOADED;
}

bool Bow::IsDrawing() {
	return currState == BowState::DRAWING;
}

bool Bow::IsInBounds() {
	return position.y >= GAME_AREA_MIN_Y - BOW_ARC_HEIGHT;
}

Arrow* Bow::GetArrow() {
	return arrow;
}

std::vector<RenderData> Bow::GetRenderData() {

	std::vector<RenderData> renderDatas;

	// BOW STRING
	{
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, BOW_ARC_HEIGHT / 2, 0), BOW_STRING_COLOR),
			VertexFormat(glm::vec3(0, -BOW_ARC_HEIGHT / 2, 0), BOW_STRING_COLOR)
		};

		std::vector<unsigned short> indices = { 0, 1 };

		std::string name = BOW_MESH_STRING;

		unsigned int primitive = GL_LINES;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	// BOW ARC
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;

		for (int i = 0; i <= BOW_ARC_TRIANGLE_COUNT; i++) {
			float t = -M_PI_2 + (float)i / BOW_ARC_TRIANGLE_COUNT * M_PI;

			float x = BOW_ARC_WIDTH / 2 * cos(t);
			float y = BOW_ARC_HEIGHT / 2 * sin(t);

			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), BOW_ARC_COLOR));
		}

		for (int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		std::string name = BOW_MESH_ARC;

		unsigned int primitive = GL_LINE_STRIP;

		renderDatas.push_back(RenderData(vertices, indices, name, primitive));
	}

	RenderData::Add(renderDatas, PowerBar::GetRenderData());

	return renderDatas;

}

std::vector<std::string> Bow::GetMeshNames() {
	return { BOW_MESH_ARC, BOW_MESH_STRING };
}

std::vector<BaseObject*> Bow::GetObjectsToRender() {

	std::vector<BaseObject*> objects = { arrow };

	if (IsDrawing() && IsAlive())
		objects.push_back(powerBar);

	return objects;
}