#include "HouseActions.hpp"

namespace
{

QString const HouseBuiltText    = "Build house #%1";
QString const HouseUpgradedText = "Upgraded house #%1 to lvl %2";

} // namespace

BuildHouseAction::BuildHouseAction(Stage& stage) : HistoryActionBase(stage)
{
}

StageError BuildHouseAction::doImpl()
{
	auto houseIdx = m_stage.buildHouse();

	if (!houseIdx) {
		return StageError::BUILDING_LIMIT_REACHED;
	}

	m_idx = *houseIdx;
	return StageError::OK;
}

void BuildHouseAction::undo()
{
	m_stage.undoBuildHouse(m_idx);
}

void BuildHouseAction::postDoInit()
{
	// empty
}

int BuildHouseAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	auto cost = priceData.house.build;
	return applyModifiers(cost, priceData, modifiers);
}

QString BuildHouseAction::getDescription() const
{
	return HouseBuiltText.arg(m_idx);
}

SerializedHistoryAction BuildHouseAction::serialize() const
{
	return { ActionElement::BUILD_HOUSE, {} };
}

UpgradeHouseAction::UpgradeHouseAction(Stage& stage, Stage::BuildingIndex idx) :
        HistoryActionBase(stage), m_idx(idx)
{
}

StageError UpgradeHouseAction::doImpl()
{
	return m_stage.upgradeHouse(m_idx);
}

void UpgradeHouseAction::undo()
{
	m_stage.downgradeHouse(m_idx);
}

void UpgradeHouseAction::postDoInit()
{
	m_upgradeLevel = m_stage.getHouses()[m_idx].getUpgradeLevel();
}

int UpgradeHouseAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	if (m_upgradeLevel == 1)
		return applyModifiers(priceData.house.upgrade1, priceData, modifiers);
	else if (m_upgradeLevel == 2)
		return applyModifiers(priceData.house.upgrade2, priceData, modifiers);
	else {
		assert(false);
		return 0;
	}
}

QString UpgradeHouseAction::getDescription() const
{
	return HouseUpgradedText.arg(m_idx).arg(m_upgradeLevel);
}

SerializedHistoryAction UpgradeHouseAction::serialize() const
{
	return { ActionElement::UPGRADE_HOUSE, { { HouseIndexAttribute, m_idx } } };
}
