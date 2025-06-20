#pragma once

#include "game_data/GameData.hpp"
#include "source/Misc.hpp"

#include <memory>
#include <vector>

class IHistoryAction;

class History
{
public:
	History();
	~History() = default;

	void addAction(std::unique_ptr<IHistoryAction> historyAction, PriceData const& priceData);

	void resetToIndex(int idx);
	void updateRemainingGold(PriceData const& priceData, PriceModsFlags modifiers);

	void setStartingGold(int gold);
	void setPriceModifiers(PriceModsFlags modifiers, PriceData const& priceData);

	std::vector<IHistoryAction const*> getActions() const;
	std::vector<int> const&            getRemainingGold() const { return m_postActionGold; }
	std::size_t                        getNumberOfDays() const { return m_dayStartIndexes.size(); }

private:
	int                                          m_startingGold = 0;
	std::vector<std::unique_ptr<IHistoryAction>> m_historyActions;
	std::vector<int>                             m_postActionGold;
	std::vector<std::size_t>                     m_dayStartIndexes;
	PriceModsFlags                               m_priceModifiers;
};
