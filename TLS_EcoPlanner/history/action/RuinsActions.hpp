#pragma once

#include "history/action/HistoryAction.hpp"

class ScavengeRuinsAction : public HistoryActionBase
{

	// IHistoryAction interface
public:
	ScavengeRuinsAction(Stage& stage, unsigned size);

	void                    undo();
	int                     getCost(PriceData const& priceData, PriceModsFlags modifiers) const;
	QString                 getDescription() const;
	SerializedHistoryAction serialize() const;

	// HistoryActionBase interface
protected:
	StageError doImpl();
	void       postDoInit();

private:
	unsigned const m_size = 1;
	unsigned       m_value;
};
