#include "Tema1.h"
#include "Transform2D.h"
#include "Colors.h"

#include <vector>
#include <math.h>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema1::Tema1() {}
Tema1::~Tema1() {}

void Tema1::Init() {
	
	// 2D camera
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x		= GAME_AREA_MIN_X;						// logic x
	logicSpace.y		= GAME_AREA_MIN_Y;						// logic y
	logicSpace.width	= GAME_AREA_MAX_X - GAME_AREA_MIN_X;	// logic width
	logicSpace.height	= GAME_AREA_MAX_Y - GAME_AREA_MIN_Y;	// logic height

	glLineWidth(5);

	CreateMeshes(Game::GetRenderData());

	glm::ivec2 resolution = window->GetResolution();
	Text = new TextRenderer(resolution.x, resolution.y);

	Text->Load("Source/TextRenderer/Fonts/Arial.ttf", 18);

}

void Tema1::FrameStart() {

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	ViewportSpace viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, COLOR_WASHED_GREEN);

	// Compute the 2D visualization matrix
	visMatrix = VisualizationTransf2DUnif(logicSpace, viewSpace);

}

void Tema1::Update(float deltaTimeSeconds) {

	game->Update(deltaTimeSeconds);
	RenderObjects(game->GetObjectsToRender());

}

void Tema1::RenderObjects(std::vector<BaseObject*> objects) {

	for (auto object : objects) {
		
		RenderObjects(object->GetObjectsToRender());

		glm::vec2 position = object->GetPosition();
		glm::vec2 rotation = object->GetRotation();
		glm::vec2 scale = object->GetScale();

		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= visMatrix;
		modelMatrix *= Transform2D::Translate(position.x, position.y);
		modelMatrix *= Transform2D::Rotate(atan2(rotation.y, rotation.x));
		modelMatrix *= Transform2D::Scale(scale.x, scale.y);

		for (auto meshName : object->GetMeshNames())
			RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);

	}

}

void Tema1::FrameEnd() {
	drawHud();
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {

	if (window->KeyHold(GLFW_KEY_W))
		game->MoveBowUp(deltaTime);

	if (window->KeyHold(GLFW_KEY_S))
		game->MoveBowDown(deltaTime);

}

void Tema1::OnKeyPress(int key, int mods) {
	
	switch (key) {
	case GLFW_KEY_P:
		game->TogglePause();
		break;
	case GLFW_KEY_R:
		game->Restart();
		break;
	case GLFW_KEY_LEFT_BRACKET:
		game->DecreaseGameTick();
		break;
	case GLFW_KEY_RIGHT_BRACKET:
		game->IncreaseGameTick();
		break;
	default:
		break;
	}

}

void Tema1::OnKeyRelease(int key, int mods) {}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

	// from viewport coords back to logic
	glm::vec3 target = glm::inverse(visMatrix) * glm::vec3(mouseX, mouseY, 1);
	game->SetBowTarget(glm::vec2(target.x, GAME_AREA_MAX_Y - target.y));

}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

	if (button == GLFW_MOUSE_BUTTON_2)
		game->DrawBow();

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

	if (button == GLFW_MOUSE_BUTTON_2)
		game->FireBow();

}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema1::OnWindowResize(int width, int height) {}

void Tema1::CreateMeshes(std::vector<RenderData>& renderDatas) {

	for (RenderData renderData : renderDatas)
		CreateMesh(renderData);

}

void Tema1::CreateMesh(RenderData renderData) {

	std::vector<VertexFormat> &vertices = renderData.vertices;
	std::vector<unsigned short> &indices = renderData.indices;
	std::string& name = renderData.name;
	unsigned int& primitive = renderData.primitive;

	unsigned int VAO_ID = 0;
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	unsigned int VBO_ID = 0;
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO_ID = 0;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh *newMesh = new Mesh(name);

	meshes[name] = newMesh;
	newMesh->InitFromBuffer(VAO_ID, static_cast<unsigned short>(indices.size()));
	newMesh->SetDrawMode(primitive);

}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 clearColor)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Tema1::drawHud() {

	glm::vec3 vec = glm::vec3(GAME_AREA_MAX_X - 200, 50, 1.0f);
	vec = vec * visMatrix;

	Text->RenderText("Score: " + std::to_string(game->GetScore()), vec.x, vec.y, vec.z, glm::vec3(1.0, 1.0, 1.0));

	if (game->IsPaused()) {
		vec = glm::vec3((GAME_AREA_MIN_X + GAME_AREA_MAX_X) / 2, (GAME_AREA_MIN_Y + GAME_AREA_MAX_Y) / 2, 4.0f);
		vec = vec * visMatrix;

		Text->RenderText("PAUSED", vec.x, vec.y, vec.z, glm::vec3(1.0, 1.0, 1.0));
	} else if (game->IsEnded()) {
		vec = glm::vec3((GAME_AREA_MIN_X + GAME_AREA_MAX_X) / 2, (GAME_AREA_MIN_Y + GAME_AREA_MAX_Y) / 2, 4.0f);
		vec = vec * visMatrix;

		Text->RenderText("GAME OVER", vec.x, vec.y, vec.z, glm::vec3(1.0, 1.0, 1.0));
	}

}