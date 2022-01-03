#include "Game.h"

Game::Game() {

	InitSpawnPool();
	InitDistributions();

	Restart();

}

Game::~Game() {}

void Game::InitDistributions() {

	// calculate the possible delays between spawns
	spawnDelayDist = std::uniform_real_distribution<float>(SPAWN_DELAY_MIN, SPAWN_DELAY_MAX);

	// calculate the total weight of the spawn pool
	float totalWeight = 0;

	for (auto entry : spawnPool)
		totalWeight += entry.second;

	spawnWeightDist = std::uniform_real_distribution<float>(0, totalWeight);

	// calculate the possible X spawn coordinates for balloons
	float minBalloonSpawnX = GAME_AREA_MIN_X + BOW_ARC_HEIGHT + BALLOON_WIDTH;
	float maxBalloonSpawnX = GAME_AREA_MAX_X - BALLOON_HEIGHT;

	balloonSpawnXDist = std::uniform_real_distribution<float>(minBalloonSpawnX, maxBalloonSpawnX);

	// calculate the possible Y spawn coordinates for shurikens
	float minShurikenSpawnY = GAME_AREA_MIN_Y + SHURIKEN_BLADE_LENGTH;
	float maxShurikenSpawnY = GAME_AREA_MAX_Y - SHURIKEN_BLADE_LENGTH;

	shurikenSpawnYDist = std::uniform_real_distribution<float>(minShurikenSpawnY, maxShurikenSpawnY);

	// calculate the possible X velocity values for shurikens
	shurikenSpawnSpeedDist = std::uniform_real_distribution<float>(SHURIKEN_MIN_SPEED, SHURIKEN_MAX_SPEED);

}

void Game::InitSpawnPool() {

	// Good Balloon
	spawnPool.push_back({
		[this]() {
			float randomX = balloonSpawnXDist(RandomUtils::mt);
			float y = GAME_AREA_MIN_Y - BALLOON_HEIGHT / 2;

			balloons.push_back(new GoodBalloon(glm::vec2(randomX, y)));
		}, GOOD_BALLOON_SPAWN_WEIGHT
	});

	// Bad Balloon
	spawnPool.push_back({
		[this]() {
			float randomX = balloonSpawnXDist(RandomUtils::mt);
			float y = GAME_AREA_MIN_Y - BALLOON_HEIGHT / 2;

			balloons.push_back(new BadBalloon(glm::vec2(randomX, y)));
		}, BAD_BALLOON_SPAWN_WEIGHT
	});

	// Shuriken
	spawnPool.push_back({
		[this]() {
			float x = GAME_AREA_MAX_X + SHURIKEN_BLADE_LENGTH;
			float randomY = shurikenSpawnYDist(RandomUtils::mt);
			float randomVel = shurikenSpawnSpeedDist(RandomUtils::mt);
			
			shurikens.push_back(new Shuriken(glm::vec2(x, randomY), glm::vec2(-randomVel, 0)));
		}, SHURIKEN_SPAWN_WEIGHT
	});

}

void Game::InitHearts() {

	float spawnX = HEARTS_POSITION_X;
	float spawnY = HEARTS_POSITION_Y;

	for (int i = 0; i < HEARTS_INITIAL_COUNT; i++) {
		hearts.push_back(new Heart(glm::vec2(spawnX, spawnY)));
		spawnX += HEART_WIDTH + HEARTS_SPACING;
	}

}

std::vector<RenderData> Game::GetRenderData() {

	std::vector<RenderData> renderDatas;
	
	RenderData::Add(renderDatas, GameAreaBoundary::GetRenderData());
	RenderData::Add(renderDatas, Bow::GetRenderData());
	RenderData::Add(renderDatas, Arrow::GetRenderData());
	RenderData::Add(renderDatas, Shuriken::GetRenderData());
	RenderData::Add(renderDatas, BadBalloon::GetRenderData());
	RenderData::Add(renderDatas, GoodBalloon::GetRenderData());
	RenderData::Add(renderDatas, Heart::GetRenderData());

	return renderDatas;

}

std::vector<BaseObject*> Game::GetObjectsToRender() {

	std::vector<BaseObject*> objects = {
		boundary,
		bow,
		arrow
	};

	for (auto balloon : balloons)
		objects.push_back(balloon);

	for (auto shuriken : shurikens)
		objects.push_back(shuriken);

	for (auto heart : hearts)
		objects.push_back(heart);

	return objects;

}

void Game::AttemptSpawn(double deltaTimeSeconds) {

	spawnDelay -= deltaTimeSeconds;

	// don't spawn anything yet
	if (spawnDelay > 0)
		return;

	// set a random delay for the next spawn attempt
	spawnDelay = spawnDelayDist(RandomUtils::mt);

	// respect the limit
	if (shurikens.size() + balloons.size() == MAX_SPAWNED_OBJECTS)
		return;

	// attempt to spawn objects
	float randomWeight = spawnWeightDist(RandomUtils::mt);
	float currWeight = 0;

	for (auto& entry : spawnPool) {
		currWeight += entry.second;

		if (randomWeight < currWeight) {
			entry.first(); // spawn the object
			return;
		}
	}

}

void Game::LoseLifePoint() {

	// make the last heart decay
	hearts[currLifePoints - 1]->StartDecay();
	
	// make the other hearts wiggle
	for (int i = 0; i < currLifePoints - 1; i++)
		hearts[i]->StartWiggle();

	currLifePoints--;

	if (currLifePoints == 0) {
		currState = GameState::ENDED;
		EndGame();
	}

}

void Game::EndGame() {

	bow->StartDecay();

	for (auto balloon : balloons)
		balloon->StartDecay();

	for (auto shuriken : shurikens)
		shuriken->StartDecay();

	for (auto heart : hearts)
		heart->StartDecay();

}

void Game::CheckShurikens() {

	auto it = shurikens.begin(); 

	while (it != shurikens.end()) {
		Shuriken* shuriken = (*it);

		if (shuriken->IsDead()) {
			it = shurikens.erase(it);
			delete shuriken;
			continue;
		}

		if (shuriken->TestCollision(bow)) {
			LoseLifePoint();
			continue;
		}

		if (arrow->IsFlying() && shuriken->TestCollision(arrow)) {
			currScore += shuriken->GetScore();
		}

		it++;
	}

}

void Game::CheckBalloons() {

	auto it = balloons.begin();

	while (it != balloons.end()) {
		BaseBalloonObject* balloon = (*it);

		if (balloon->IsDead()) {
			it = balloons.erase(it);
			delete balloon;
			continue;
		}

		if (arrow->IsFlying() && balloon->TestCollision(arrow)) {
			currScore += balloon->GetScore();
		}

		it++;
	}

}

void Game::CheckHearts() {

	auto it = hearts.begin();

	while (it != hearts.end()) {
		Heart* heart = (*it);

		if (heart->IsDead()) {
			it = hearts.erase(it);
			delete heart;
			continue;
		}

		it++;
	}

}

void Game::UpdateObjects(double deltaTimeSeconds) {

	if (arrow->IsIdle())
		bow->LoadArrow();

	bow->Update(deltaTimeSeconds);
	arrow->Update(deltaTimeSeconds);

	for (auto balloon : balloons)
		balloon->Update(deltaTimeSeconds);

	for (auto shuriken : shurikens)
		shuriken->Update(deltaTimeSeconds);

	for (auto heart : hearts)
		heart->Update(deltaTimeSeconds);

}

void Game::Update(double deltaTimeSeconds) {

	deltaTimeSeconds *= currGameTick;
	if (currScore < 0)
		currScore = 0;

	if (IsPaused())
		return;

	CheckShurikens();
	CheckBalloons();
	CheckHearts();
	UpdateObjects(deltaTimeSeconds);
	
	if (!IsPlaying())
		return;

	AttemptSpawn(deltaTimeSeconds);

}

void Game::SetBowTarget(glm::vec2 target) {
	if (!IsPlaying())
		return;

	bow->SetTarget(target);
}

void Game::MoveBowUp(double deltaTime) {
	if (!IsPlaying())
		return;

	bow->MoveUp(deltaTime * currGameTick);
}

void Game::MoveBowDown(double deltaTime) {
	if (!IsPlaying())
		return;

	bow->MoveDown(deltaTime * currGameTick);
}

void Game::DrawBow() {
	if (!IsPlaying())
		return;

	bow->Draw();
}

void Game::FireBow() {
	if (!IsPlaying())
		return;

	bow->Fire();
}

bool Game::IsPlaying() {
	return currState == GameState::PLAYING;
}

bool Game::IsPaused() {
	return currState == GameState::PAUSED;
}

bool Game::IsEnded() {
	return currState == GameState::ENDED;
}

void Game::TogglePause() {

	if (IsPlaying()) {
		currState = GameState::PAUSED;
		prevGameTick = currGameTick;
		currGameTick = 0;
	} else if (IsPaused()) {
		currState = GameState::PLAYING;
		currGameTick = prevGameTick;
	}

}

void Game::Restart() {

	currState = GameState::PLAYING;

	delete boundary;
	boundary = new GameAreaBoundary();

	delete bow;
	bow = new Bow();
	arrow = bow->GetArrow();

	for (auto balloon : balloons)
		delete balloon;
	balloons.clear();

	for (auto shuriken : shurikens)
		delete shuriken;
	shurikens.clear();

	for (auto heart : hearts)
		delete heart;
	hearts.clear();

	InitHearts();

	spawnDelay = 0;
	currLifePoints = HEARTS_INITIAL_COUNT;
	currScore = 0;

	currGameTick = 1.0;

}

void Game::DecreaseGameTick() {
	currGameTick -= GAME_TICK_CHANGE_STEP;

	if (currGameTick <= 0)
		currGameTick = 0;
}

void Game::IncreaseGameTick() {
	currGameTick += GAME_TICK_CHANGE_STEP;
}

int Game::GetScore() {
	return currScore;
}