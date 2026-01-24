#include "DayStartAction.hpp"

namespace
{

QString const NewDayText = "Day %1 dawns; income: %2";
QString const DayStartTooltip =
        "panic reward: %1\n"
        "item reward: %2\n"
        "gold mine income: %3\n"
        "item production income: %4";

} // namespace

DayStartAction::DayStartAction(
        Stage& stage, unsigned dayNumber, PanicRank rank, GameData const& gameData) :
        HistoryActionBase(stage), m_dayNumber(dayNumber), m_panicRank(rank)
{
	assert(dayNumber > 0);

	unsigned const DayIndex = dayNumber - 1;
	assert(DayIndex < gameData.night.panicRewards.size());

	m_nightItemValue  = gameData.night.itemValues.at(DayIndex);
	m_panicGoldReward = gameData.night.panicRewards[DayIndex].at(rank);
}

StageError DayStartAction::doImpl()
{
	m_previousDayStageUses = m_stage.regenerate();
	return StageError::OK;
}

void DayStartAction::undo()
{
	m_stage.undoRegenerate(m_previousDayStageUses);
}

void DayStartAction::postDoInit()
{
	m_dailyGoldIncome       = m_stage.dailyGoldIncome();
	m_dailyProductionIncome = m_stage.dailyProductionIncome();
}

int DayStartAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	Q_UNUSED(priceData)
	Q_UNUSED(modifiers)
	return -getReward();
}

QString DayStartAction::getDescription() const
{
	return NewDayText.arg(m_dayNumber).arg(getReward());
}

QString DayStartAction::getTooltip() const
{
	unsigned const itemPrice =
	        static_cast<unsigned>(m_nightItemValue * (m_stage.getShopPricePercentage() / 100.0));
	return DayStartTooltip.arg(m_panicGoldReward)
	        .arg(itemPrice)
	        .arg(m_dailyGoldIncome)
	        .arg(m_dailyProductionIncome);
}

SerializedHistoryAction DayStartAction::serialize() const
{
	return { ActionElement::DAY_START,
		     { { PanicRankAttribute, QString::fromStdString(rankToString(m_panicRank)) } } };
}

int DayStartAction::getReward() const
{
	unsigned const itemPrice =
	        static_cast<unsigned>(m_nightItemValue * (m_stage.getShopPricePercentage() / 100.0));
	return static_cast<int>(
	        m_panicGoldReward + itemPrice + m_dailyGoldIncome + m_dailyProductionIncome);
}
