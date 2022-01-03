#include "RandomUtils.h"

std::random_device RandomUtils::rd;
std::mt19937 RandomUtils::mt = std::mt19937(RandomUtils::rd());