#pragma once

#include "history/action/HistoryAction.hpp"

class BuildGoldMineAction : public HistoryActionBase
{
public:
	BuildGoldMineAction(Stage& stage);

	// HistoryActionBase interface
public:
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	void    undo() override;
	SerializedHistoryAction serialize() const override;

public:
	Stage::BuildingIndex getIndex() const;

protected:
	StageError doImpl() override;
	void       postDoInit() override;

private:
	Stage::BuildingIndex m_idx;
};

class UpgradeGoldMineAction : public HistoryActionBase
{
public:
	UpgradeGoldMineAction(Stage& stage, Stage::BuildingIndex idx, UpgradePath path);

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
	UpgradePath const          m_upgradePath;
	unsigned                   m_upgradeLevel;
};

class WorkGoldMineAction : public HistoryActionBase
{
public:
	WorkGoldMineAction(Stage& stage, Stage::BuildingIndex mineIdx);

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
	unsigned             m_goldIncome;
};
