#pragma once

#include "ViewportUtils.h"
#include "Game.h"

#include <map>

#include <TextRenderer/TextRenderer.h>
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class Tema1 : public SimpleScene {

public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void CreateMesh(RenderData renderData);
	void CreateMeshes(std::vector<RenderData>& renderDatas);

	void RenderObjects(std::vector<BaseObject*> objects);

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
	void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	// Draw the hud
	void drawHud();

	// The text renderer
	TextRenderer* Text;

	LogicSpace logicSpace;
	glm::mat3 visMatrix;

	Game* game = new Game();
};