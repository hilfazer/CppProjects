#pragma once

#include "history/action/HistoryAction.hpp"

class DayStartAction : public HistoryActionBase
{
public:
	DayStartAction(Stage& stage, unsigned dayNumber, PanicRank rank, GameData const& gameData);
	~DayStartAction() override = default;

	// HistoryActionBase interface
public:
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	SerializedHistoryAction serialize() const override;
	void                    undo() override;
	QString                 getTooltip() const override;

	// HistoryActionBase interface
protected:
	StageError doImpl() override;
	void       postDoInit() override;

private:
	int getReward() const;

private:
	unsigned const  m_dayNumber;
	PanicRank const m_panicRank;

	unsigned  m_nightItemValue;
	unsigned  m_panicGoldReward;
	unsigned  m_dailyGoldIncome       = 0;
	unsigned  m_dailyProductionIncome = 0;
	StageUses m_previousDayStageUses;
};
