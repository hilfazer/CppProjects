#pragma once

#include "game_data/GameData.hpp"

#include <QString>

#include <map>
#include <vector>

class GoldMine
{
public:
	enum class MineError {
		OK = 0,
		FAIL,
		NOT_ENOUGH_WORKERS,
		NOT_ENOUGH_USES,
	};

public:
	GoldMine(GoldMineUpgrades const& upgrades);

	bool      upgradeProduction();
	bool      downgradeProduction();
	bool      upgradeWorkers();
	bool      downgradeWorkers();
	MineError useMine(unsigned availableWorkers);
	void      undoUseMine();
	void      regenerate();
	void      undoRegenerate(unsigned timesUsed);

	unsigned getProdLevel() const { return m_prodLevel; }
	unsigned getWorkLevel() const { return m_workerLevel; }
	unsigned getRequiredWorkers() const { return m_params.workerCost[m_workerLevel]; }
	unsigned getDailyIncome() const;
	QString  getDescription(unsigned index) const;
	unsigned getTimesUsed() const { return m_timesUsed; }

private:
	struct MineParams {
		std::vector<unsigned> production;
		std::vector<unsigned> gold;
		std::vector<unsigned> workerCost;
		std::vector<unsigned> workerUses;
	} const m_params;

	unsigned m_prodLevel   = 0;
	unsigned m_workerLevel = 0;
	unsigned m_timesUsed   = 0;

private:
	static MineParams upgradesToParams(GoldMineUpgrades const& upgradeMaps);
};
