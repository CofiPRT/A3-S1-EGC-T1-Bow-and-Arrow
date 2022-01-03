#pragma once

#include "Bow.h"
#include "GameAreaBoundary.h"
#include "Arrow.h"
#include "GoodBalloon.h"
#include "BadBalloon.h"
#include "Shuriken.h"
#include "Heart.h"
#include "RandomUtils.h"

#include <map>

#define SPAWN_DELAY_MIN (0.f)
#define SPAWN_DELAY_MAX (2.f)

#define HEARTS_INITIAL_COUNT	(3)
#define HEARTS_SPACING			(10)
#define HEARTS_POSITION_X		(GAME_AREA_MIN_X + HEART_WIDTH + HEARTS_SPACING)
#define HEARTS_POSITION_Y		(GAME_AREA_MIN_Y + HEART_HEIGHT + HEARTS_SPACING)

#define MAX_SPAWNED_OBJECTS	(10)

#define GAME_TICK_CHANGE_STEP (0.1f)

class Game {

private:
	enum class GameState { PLAYING, PAUSED, ENDED };

	GameState currState = GameState::PLAYING;

	GameAreaBoundary* boundary = new GameAreaBoundary();
	Bow* bow = new Bow();
	Arrow* arrow = bow->GetArrow();

	std::vector<BaseBalloonObject*> balloons;
	std::vector<Shuriken*> shurikens;
	std::vector<Heart*> hearts;

	float spawnDelay = 0;

	int currLifePoints = HEARTS_INITIAL_COUNT; // actual life points left, not just visual objects like hearts
	int currScore = 0;

	float prevGameTick = 1.0;
	float currGameTick = 1.0;

	// random number generation
	std::uniform_real_distribution<float> spawnDelayDist;
	std::uniform_real_distribution<float> spawnWeightDist;

	std::uniform_real_distribution<float> balloonSpawnXDist;
	std::uniform_real_distribution<float> shurikenSpawnYDist;
	std::uniform_real_distribution<float> shurikenSpawnSpeedDist;

	std::vector<std::pair<std::function<void()>, float>> spawnPool;

	void InitDistributions();
	void InitSpawnPool();
	void InitHearts();

	void CheckShurikens();
	void CheckBalloons();
	void CheckHearts();

	void AttemptSpawn(double deltaTimeSeconds);
	void UpdateObjects(double deltaTimeSeconds);

	void LoseLifePoint();
	void EndGame();

public:
	Game();
	~Game();

	void Update(double deltaTimeSeconds);
	std::vector<BaseObject*> GetObjectsToRender();
	static std::vector<RenderData> GetRenderData();

	void SetBowTarget(glm::vec2 target);
	void MoveBowUp(double deltaTime);
	void MoveBowDown(double deltaTime);
	void DrawBow();
	void FireBow();

	void TogglePause();
	void Restart();
	void DecreaseGameTick();
	void IncreaseGameTick();

	bool IsPlaying();
	bool IsPaused();
	bool IsEnded();

	int GetScore();

};