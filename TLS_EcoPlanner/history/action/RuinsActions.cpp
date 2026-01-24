#include "RuinsActions.hpp"
#include "history/HistorySerialization.hpp"

namespace
{

QString const ScavengeRuinsText = "Scavenge size %1 ruins";

}

ScavengeRuinsAction::ScavengeRuinsAction(Stage& stage, unsigned size) :
        HistoryActionBase(stage), m_size(size)
{
}

int ScavengeRuinsAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	return -applyModifiers(m_value, priceData, modifiers);
}

QString ScavengeRuinsAction::getDescription() const
{
	return ScavengeRuinsText.arg(m_size);
}

SerializedHistoryAction ScavengeRuinsAction::serialize() const
{
	return { ActionElement::SCAVENGE_RUINS, { { RuinsSizeAttribute, m_size } } };
};

StageError ScavengeRuinsAction::doImpl()
{
	return m_stage.scavengeRuins(m_size);
}

void ScavengeRuinsAction::undo()
{
	m_stage.undoScavengeRuins(m_size);
}

void ScavengeRuinsAction::postDoInit()
{
	m_value = m_stage.getRuinsValue(m_size);
}
