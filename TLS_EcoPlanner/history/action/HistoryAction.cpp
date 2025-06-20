#include "HistoryAction.hpp"

int applyModifiers(unsigned cost, PriceData const& priceData, PriceModsFlags modifiers)
{
	float fCost = static_cast<float>(cost);
	fCost *= (modifiers.easy ? priceData.easyDifficultyModifier : 1.0f);
	fCost *= (modifiers.apoc ? priceData.apocDifficultyModifier : 1.0f);
	return static_cast<int>(fCost);
}

IHistoryAction::~IHistoryAction()
{
}

HistoryActionBase::HistoryActionBase(Stage& stage) : m_stage(stage)
{
}

StageError HistoryActionBase::execute()
{
	assert(m_executed == false);

	auto error = doImpl();
	if (error == StageError::OK) {
		postDoInit();
		m_executed = true;
	}

	return error;
}
