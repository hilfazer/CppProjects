#include "game_data/GameData.hpp"

#include <cassert>
#include <map>

GameData createDefaultGameData()
{
	using PR = PanicRank;

	static const std::vector<std::map<PanicRank, unsigned>> PanicRewards = {
		{{ PR::S, 110 }, { PR::A, 100 },  { PR::B, 90 },  { PR::C, 80 },  { PR::D, 70 }},
		{{ PR::S, 120 }, { PR::A, 110 }, { PR::B, 100 },  { PR::C, 90 },  { PR::D, 80 }},
		{{ PR::S, 130 }, { PR::A, 120 }, { PR::B, 110 }, { PR::C, 100 },  { PR::D, 90 }},
		{{ PR::S, 140 }, { PR::A, 130 }, { PR::B, 120 }, { PR::C, 110 }, { PR::D, 100 }},
	};

	static const std::vector<unsigned> ItemRewardPrices = { 20, 25, 30, 35 };

	assert(PanicRewards.size() == ItemRewardPrices.size());

	GameData data;
	data.night.panicRewards       = PanicRewards;
	data.night.itemValues         = ItemRewardPrices;
	data.goldMineUpgrades         = createDefaultGoldMineUpgrades();
	data.startingBuildings.house  = 2;
	data.startingBuildings.shop   = 1;
	data.startingBuildings.ruins1 = 2;
	data.startingBuildings.ruins2 = 2;
	return data;
}

std::string rankToString(PanicRank rank)
{
	static std::map<PanicRank, std::string> const RankMap{
		{PanicRank::S, "S"},
        {PanicRank::A, "A"},
        {PanicRank::B, "B"},
		{PanicRank::C, "C"},
        {PanicRank::D, "D"},
	};

	assert(RankMap.find(rank) != RankMap.end());

	return RankMap.at(rank);
}

PanicRank stringToRank(std::string const& rank)
{
	static std::map<std::string, PanicRank> const RankMap{
		{"S", PanicRank::S},
        {"A", PanicRank::A},
        {"B", PanicRank::B},
		{"C", PanicRank::C},
        {"D", PanicRank::D},
	};

	auto const it = RankMap.find(rank);
	return it != RankMap.end() ? it->second : PanicRank::S;
}

GoldMineUpgrades createDefaultGoldMineUpgrades()
{
	using P = GoldMineParam;

	GoldMineUpgrades maps;
	maps.resize(4);
	maps[0][P::GOLD]        = 10;
	maps[0][P::PRODUCTION]  = 10;
	maps[0][P::USES]        = 1;
	maps[0][P::WORKER_COST] = 2;
	maps[1][P::GOLD]        = 25;
	maps[1][P::WORKER_COST] = 1;
	maps[2][P::PRODUCTION]  = 20;
	maps[2][P::USES]        = 2;
	maps[3][P::GOLD]        = 55;

	return maps;
}
