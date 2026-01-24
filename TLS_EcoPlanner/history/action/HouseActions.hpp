#pragma once

#include "history/action/HistoryAction.hpp"

class BuildHouseAction : public HistoryActionBase
{
public:
	BuildHouseAction(Stage& stage);

	// HistoryActionBase interface
public:
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	SerializedHistoryAction serialize() const override;
	void                    undo() override;

protected:
	StageError doImpl() override;
	void       postDoInit() override;

private:
	Stage::BuildingIndex m_idx;
};

class UpgradeHouseAction : public HistoryActionBase
{
public:
	UpgradeHouseAction(Stage& stage, Stage::BuildingIndex idx);

	// HistoryActionBase interface
public:
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	SerializedHistoryAction serialize() const override;
	void                    undo() override;

protected:
	StageError doImpl() override;
	void       postDoInit() override;

private:
	Stage::BuildingIndex const m_idx;
	unsigned                   m_upgradeLevel;
};
