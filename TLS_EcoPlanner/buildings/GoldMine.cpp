#include "GoldMine.hpp"


namespace
{

QString const  GoldMineText = "Gold Mine #%1 [prod lvl: %2, worker lvl: %3]";
unsigned const ParamCount   = 4;

} // namespace

GoldMine::GoldMine(GoldMineUpgrades const& upgrades) : m_params(upgradesToParams(upgrades))
{
	assert(m_params.gold.size() == m_params.production.size());
	assert(m_params.workerCost.size() == m_params.workerUses.size());
}

bool GoldMine::upgradeProduction()
{
	if (m_prodLevel == m_params.gold.size() - 1)
		return false;

	++m_prodLevel;
	return true;
}

bool GoldMine::downgradeProduction()
{
	if (m_prodLevel == 0)
		return false;

	--m_prodLevel;
	return true;
}

bool GoldMine::upgradeWorkers()
{
	if (m_workerLevel == m_params.workerCost.size() - 1)
		return false;

	++m_workerLevel;
	return true;
}

bool GoldMine::downgradeWorkers()
{
	if (m_workerLevel == 0)
		return false;

	--m_workerLevel;
	return true;
}

GoldMine::MineError GoldMine::useMine(unsigned availableWorkers)
{
	if (m_timesUsed >= m_params.workerUses[m_workerLevel])
		return MineError::NOT_ENOUGH_USES;

	if (availableWorkers < m_params.workerCost[m_workerLevel])
		return MineError::NOT_ENOUGH_WORKERS;

	++m_timesUsed;
	return MineError::OK;
}

void GoldMine::undoUseMine()
{
	assert(m_timesUsed > 0);

	--m_timesUsed;
}

void GoldMine::regenerate()
{
	m_timesUsed = 0;
}

void GoldMine::undoRegenerate(unsigned timesUsed)
{
	m_timesUsed = timesUsed;
}

unsigned GoldMine::getDailyIncome() const
{
	return (m_params.production[m_prodLevel] / 10) * m_params.gold[m_prodLevel];
}

QString GoldMine::getDescription(unsigned index) const
{
	return GoldMineText.arg(index).arg(m_prodLevel).arg(m_workerLevel);
}

GoldMine::MineParams GoldMine::upgradesToParams(GoldMineUpgrades const& upgradeMaps)
{
	assert(upgradeMaps.size() > 0);
	assert(upgradeMaps[0].size() == ParamCount);

	MineParams params;

	for (std::size_t lvl = 0; lvl < upgradeMaps.size(); ++lvl) {

		using P = GoldMineParam;
		if (upgradeMaps[lvl].find(P::GOLD) != upgradeMaps[lvl].end() ||
		    upgradeMaps[lvl].find(P::PRODUCTION) != upgradeMaps[lvl].end()) {

			params.gold.resize(lvl + 1);
			params.production.resize(lvl + 1);
		}
		if (upgradeMaps[lvl].find(P::WORKER_COST) != upgradeMaps[lvl].end() ||
		    upgradeMaps[lvl].find(P::USES) != upgradeMaps[lvl].end()) {

			params.workerCost.resize(lvl + 1);
			params.workerUses.resize(lvl + 1);
		}
	}

	for (std::size_t lvl = 0; lvl < upgradeMaps.size(); ++lvl) {
		if (lvl > 0) {
			(params.gold.size() > lvl) && (params.gold[lvl] = params.gold[lvl - 1]);
			(params.production.size() > lvl) &&
			        (params.production[lvl] = params.production[lvl - 1]);
			(params.workerCost.size() > lvl) &&
			        (params.workerCost[lvl] = params.workerCost[lvl - 1]);
			(params.workerUses.size() > lvl) &&
			        (params.workerUses[lvl] = params.workerUses[lvl - 1]);
		}

		std::map<GoldMineParam, unsigned> const& params2Values = upgradeMaps[lvl];

		for (auto const& keyValue : params2Values) {
			switch (keyValue.first) {
			case GoldMineParam::GOLD:
				params.gold[lvl] = keyValue.second;
				break;
			case GoldMineParam::PRODUCTION:
				params.production[lvl] = keyValue.second;
				break;
			case GoldMineParam::WORKER_COST:
				params.workerCost[lvl] = keyValue.second;
				break;
			case GoldMineParam::USES:
				params.workerUses[lvl] = keyValue.second;
				break;
			}
		}
	}

	return params;
}
