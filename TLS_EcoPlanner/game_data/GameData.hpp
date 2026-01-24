#pragma once

#include <map>
#include <string>
#include <vector>

enum class UpgradePath { PRODUCTION, WORKERS };

struct HousePriceData {
	unsigned build    = 30;
	unsigned upgrade1 = 40;
	unsigned upgrade2 = 80;
};

struct MinePriceData {
	unsigned              build              = 45;
	std::vector<unsigned> productionUpgrades = { 40, 90, 200 };
	std::vector<unsigned> workerUpgrades     = { 30, 100 };
};

struct ShopPriceData {
	unsigned build    = 60;
	unsigned upgrade1 = 40;
	unsigned upgrade2 = 70;
};

struct PriceData {
	float easyDifficultyModifier = 0.8f;
	float apocDifficultyModifier = 1.2f;

	HousePriceData house;
	MinePriceData  mine;
	ShopPriceData  shop;
};

struct MaxBuildingsData {
	unsigned mine  = 3;
	unsigned house = 5;
};

struct StartingBuildingsData {
	unsigned house  = 2;
	unsigned mine   = 0;
	unsigned shop   = 1;
	unsigned ruins1 = 3;
	unsigned ruins2 = 2;
	unsigned ruins3 = 2;
	unsigned ruins4 = 1;
};

enum class GoldMineParam { GOLD, PRODUCTION, WORKER_COST, USES };

using GoldMineUpgrades = std::vector<std::map<GoldMineParam, unsigned>>;

struct ShopUpgradesData {
	unsigned percentage0 = 30;
	unsigned percentage1 = 40;
	unsigned percentage2 = 50;
};

struct RuinsData {
	unsigned size1gold = 12;
	unsigned size2gold = 30;
	unsigned size3gold = 65;
	unsigned size4gold = 90;
};

enum class PanicRank : char { S, A, B, C, D };

struct NightRewardData {
	std::vector<std::map<PanicRank, unsigned>> panicRewards;
	std::vector<unsigned>                      itemValues;
	PanicRank                                  minimumRankForItem = PanicRank::A;
};

struct GameData {
	PriceData             prices;
	MaxBuildingsData      maxBuildings;
	NightRewardData       night;
	GoldMineUpgrades      goldMineUpgrades;
	ShopUpgradesData      shopUpgrades;
	RuinsData             ruinsValues;
	StartingBuildingsData startingBuildings;
};

GoldMineUpgrades createDefaultGoldMineUpgrades();
std::string      rankToString(PanicRank rank);
PanicRank        stringToRank(std::string const& rank);
