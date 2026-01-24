#include "Stage.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>

using Err = StageError;

Stage::Stage(StageParams params) :
        m_maxBuildings(params.maxBuildings),
        m_startingBuildings(params.startingBuildings),
        m_mineCosts(params.mineCosts),
        m_goldMineUpgrades(params.mineUpgrades),
        m_shopUpgrades(params.shopUpgrades),
        m_ruinsValues(params.ruinsValues)
{
}

void Stage::constructStartingBuildings(StartingBuildingsData const& startBuildingsData)
{
	for (unsigned i = 0; i < startBuildingsData.house; ++i) {
		buildHouse();
	}

	for (unsigned i = 0; i < startBuildingsData.mine; ++i) {
		buildMine();
	}

	for (unsigned i = 0; i < startBuildingsData.ruins1; ++i) {
		buildRuins(1);
	}

	for (unsigned i = 0; i < startBuildingsData.ruins2; ++i) {
		buildRuins(2);
	}

	for (unsigned i = 0; i < startBuildingsData.ruins3; ++i) {
		buildRuins(3);
	}

	for (unsigned i = 0; i < startBuildingsData.ruins4; ++i) {
		buildRuins(4);
	}

	if (startBuildingsData.shop > 0) {
		buildShop();
	}
}

std::optional<Stage::BuildingIndex> Stage::buildMine()
{
	if (m_goldMines.size() == m_maxBuildings.mine)
		return {};

	m_goldMines.emplace_back(m_goldMineUpgrades);
	return static_cast<Stage::BuildingIndex>(m_goldMines.size()) - 1;
}

void Stage::undoBuildMine(BuildingIndex idx)
{
	assert(idx == getGoldMines().size() - 1);

	GoldMine const& mine = m_goldMines.back();
	assert(mine.getProdLevel() == 0u);
	assert(mine.getWorkLevel() == 0u);

	m_goldMines.pop_back();
}

StageError Stage::upgradeGoldMineProduction(BuildingIndex idx)
{
	return getMine(idx).upgradeProduction() ? Err::OK : Err::UPGRADE_LIMIT_REACHED;
}

void Stage::undoUpgradeGoldMineProduction(BuildingIndex idx)
{
	auto result = getMine(idx).downgradeProduction();
	assert(result);
}

StageError Stage::upgradeGoldMineWorkers(BuildingIndex idx)
{
	return getMine(idx).upgradeWorkers() ? Err::OK : Err::UPGRADE_LIMIT_REACHED;
}

void Stage::undoUpgradeGoldMineWorkers(BuildingIndex idx)
{
	auto result = getMine(idx).downgradeWorkers();
	assert(result);
}

StageError Stage::workMine(BuildingIndex idx)
{
	if (idx >= m_goldMines.size()) {
		return StageError::NO_BUILDING_WITH_IDX;
	}

	switch (m_goldMines[idx].useMine(getRemainingWorkers())) {
	case GoldMine::MineError::NOT_ENOUGH_WORKERS: {
		return Err::NOT_ENOUGH_WORKERS;
	}
	case GoldMine::MineError::NOT_ENOUGH_USES: {
		return Err::NOT_ENOUGH_BUILDING_USES;
	}
	case GoldMine::MineError::OK: {
		m_usedWorkers += m_goldMines[idx].getRequiredWorkers();
		return StageError::OK;
	}
	case GoldMine::MineError::FAIL: {
		assert(!"unknown error");
	}
	}

	return Err::FAIL;
}

void Stage::undoWorkMine(BuildingIndex idx)
{
	auto const requiredWorkers = m_goldMines[idx].getRequiredWorkers();

	assert(m_goldMines.size() > idx);
	assert(m_usedWorkers >= requiredWorkers);

	m_goldMines[idx].undoUseMine();
	m_usedWorkers -= requiredWorkers;
}

StageError Stage::buildShop()
{
	if (m_shop)
		return Err::BUILDING_LIMIT_REACHED;

	m_shop = Shop(m_shopUpgrades);
	return Err::OK;
}

void Stage::undoBuildShop()
{
	m_shop.reset();
}

StageError Stage::upgradeShopSellingPrices()
{
	if (m_shop)
		return m_shop->upgradePrices() ? Err::OK : Err::UPGRADE_LIMIT_REACHED;
	else
		return Err::BUILDING_DOESNT_EXIST;
}

StageError Stage::downgradeShopSellingPrices()
{
	if (m_shop)
		return m_shop->downgradePrices() ? Err::OK : Err::FAIL;
	else
		return Err::BUILDING_DOESNT_EXIST;
}

void Stage::buildRuins(unsigned size)
{
	m_ruins.emplace_back(size);
}

StageError Stage::scavengeRuins(unsigned size)
{
	auto it = std::find_if(m_ruins.begin(), m_ruins.end(), [&](Ruins const& ruins) {
		return ruins.getSize() == size;
	});

	if (it == m_ruins.end()) {
		return StageError::BUILDING_DOESNT_EXIST;
	}

	if (getRemainingWorkers() < size) {
		return StageError::NOT_ENOUGH_WORKERS;
	}

	m_ruins.erase(it);
	m_usedWorkers += size;
	return StageError::OK;
}

void Stage::undoScavengeRuins(unsigned size)
{
	assert(m_usedWorkers >= size);

	m_ruins.emplace_back(size);
	m_usedWorkers -= size;
}

std::optional<Stage::BuildingIndex> Stage::buildHouse()
{
	if (m_houses.size() >= m_maxBuildings.house)
		return {};

	m_houses.emplace_back();
	return static_cast<Stage::BuildingIndex>(m_houses.size()) - 1;
}

void Stage::undoBuildHouse(BuildingIndex idx)
{
	assert(idx == getHouses().size() - 1);

	House const& house = m_houses.back();
	assert(house.getWorkerCount() == 1u);

	m_houses.pop_back();
}

StageError Stage::upgradeHouse(BuildingIndex idx)
{
	assert(m_houses.size() > idx);

	return m_houses[idx].upgradeWorkers() ? Err::OK : Err::UPGRADE_LIMIT_REACHED;
}

void Stage::downgradeHouse(BuildingIndex idx)
{
	assert(m_houses.size() > idx);

	m_houses[idx].downgradeWorkers();
}

StageUses Stage::regenerate()
{
	StageUses uses;
	uses.workers  = m_usedWorkers;
	m_usedWorkers = 0;

	for (auto& mine : m_goldMines) {
		uses.mines.push_back(mine.getTimesUsed());
		mine.regenerate();
	}
	return uses;
}

void Stage::undoRegenerate(StageUses const& uses)
{
	m_usedWorkers = uses.workers;

	assert(m_goldMines.size() == uses.mines.size());
	for (unsigned i = 0; i < uses.mines.size(); ++i) {
		m_goldMines[i].undoRegenerate(uses.mines[i]);
	}
}

unsigned Stage::dailyGoldIncome() const
{
	unsigned totalIncome = std::accumulate(
	        m_goldMines.begin(), m_goldMines.end(), 0u, [](unsigned total, GoldMine const& mine) {
		        return total + mine.getDailyIncome();
	        });
	return totalIncome;
}

unsigned Stage::dailyProductionIncome() const
{
	return 0u; // TODO: implement
}

unsigned Stage::getShopPricePercentage() const
{
	if (m_shop)
		return m_shop->getPricePercentage();
	else
		return Shop::DefaultPercentage;
}

unsigned Stage::getMaxWorkers() const
{
	unsigned count = 0;
	for (auto const& house : m_houses) {
		count += house.getWorkerCount();
	}
	return count;
}

unsigned Stage::getRuinsValue(unsigned size) const
{
	static const std::map<unsigned, unsigned> sizeToValue = {
		{1, m_ruinsValues.size1gold},
		{2, m_ruinsValues.size2gold},
		{3, m_ruinsValues.size3gold},
		{4, m_ruinsValues.size4gold},
	};

	return sizeToValue.at(size);
}

Shop const* Stage::getShop() const
{
	if (m_shop)
		return &*m_shop;
	else
		return nullptr;
}

GoldMine& Stage::getMine(BuildingIndex idx)
{
	assert(static_cast<std::size_t>(idx) < m_goldMines.size());

	return m_goldMines.at(idx);
}
