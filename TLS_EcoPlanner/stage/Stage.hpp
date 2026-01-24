#pragma once

#include "buildings/GoldMine.hpp"
#include "buildings/House.hpp"
#include "buildings/Ruins.hpp"
#include "buildings/Shop.hpp"

#include <optional>
#include <vector>

struct StageParams {
	MaxBuildingsData      maxBuildings;
	StartingBuildingsData startingBuildings;
	MinePriceData         mineCosts;
	GoldMineUpgrades      mineUpgrades;
	ShopUpgradesData      shopUpgrades;
	RuinsData             ruinsValues;
};

struct StageUses {
	unsigned              workers;
	std::vector<unsigned> mines;
	// TODO: production
};

enum class StageError {
	OK = 0,
	FAIL,
	NO_BUILDING_WITH_IDX,
	NOT_ENOUGH_BUILDING_USES,
	NOT_ENOUGH_WORKERS,
	UPGRADE_LIMIT_REACHED,
	BUILDING_LIMIT_REACHED,
	BUILDING_DOESNT_EXIST,
};

class Stage
{
public:
	using BuildingIndex = std::size_t;

public:
	Stage(StageParams params);
	Stage(Stage const&)            = delete;
	Stage& operator=(Stage const&) = delete;

	void constructStartingBuildings(StartingBuildingsData const& startBuildingsData);

	std::optional<BuildingIndex> buildMine();
	void                         undoBuildMine(BuildingIndex idx);
	StageError                   upgradeGoldMineProduction(BuildingIndex idx);
	void                         undoUpgradeGoldMineProduction(BuildingIndex idx);
	StageError                   upgradeGoldMineWorkers(BuildingIndex idx);
	void                         undoUpgradeGoldMineWorkers(BuildingIndex idx);
	StageError                   workMine(BuildingIndex idx);
	void                         undoWorkMine(BuildingIndex idx);

	StageError buildShop();
	void       undoBuildShop();
	StageError upgradeShopSellingPrices();
	StageError downgradeShopSellingPrices();

	void buildRuins(unsigned size);
	StageError scavengeRuins(unsigned size);
	void       undoScavengeRuins(unsigned size);

	std::optional<BuildingIndex> buildHouse();
	void                         undoBuildHouse(BuildingIndex idx);
	StageError                   upgradeHouse(BuildingIndex idx);
	void                         downgradeHouse(BuildingIndex idx);

	StageUses regenerate();
	void      undoRegenerate(StageUses const& uses);

	unsigned dailyGoldIncome() const;
	unsigned dailyProductionIncome() const;
	unsigned getShopPricePercentage() const;
	unsigned getMaxWorkers() const;
	unsigned getRemainingWorkers() const { return getMaxWorkers() - m_usedWorkers; }
	unsigned getRuinsValue(unsigned size) const;

	// data accessors
public:
	std::vector<GoldMine> const& getGoldMines() const { return m_goldMines; }
	Shop const*                  getShop() const;
	std::vector<House> const&    getHouses() const { return m_houses; }
	std::vector<Ruins> const&    getRuins() const { return m_ruins; }

private:
	GoldMine& getMine(BuildingIndex idx);

private:
	MaxBuildingsData const      m_maxBuildings;
	StartingBuildingsData const m_startingBuildings;
	MinePriceData const         m_mineCosts;
	GoldMineUpgrades const      m_goldMineUpgrades;
	ShopUpgradesData const      m_shopUpgrades;
	RuinsData const             m_ruinsValues;

	unsigned              m_usedWorkers = 0;
	std::vector<GoldMine> m_goldMines;
	std::optional<Shop>   m_shop;
	std::vector<House>    m_houses;
	std::vector<Ruins>    m_ruins;
};
