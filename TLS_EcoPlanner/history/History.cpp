#include "History.hpp"
#include "history/action/DayStartAction.hpp"
#include "history/action/HistoryAction.hpp"

History::History()
{
}

void History::addAction(std::unique_ptr<IHistoryAction> historyAction, PriceData const& priceData)
{
	assert(historyAction->isExecuted());

	int previousGold = m_postActionGold.empty() ? m_startingGold : m_postActionGold.back();
	m_postActionGold.push_back(previousGold - historyAction->getCost(priceData, m_priceModifiers));
	m_historyActions.push_back(std::move(historyAction));

	if (dynamic_cast<DayStartAction*>(m_historyActions.back().get()) != nullptr) {
		m_dayStartIndexes.push_back(m_historyActions.size() - 1);
	}

	assert(m_historyActions.size() == m_postActionGold.size());
}

void History::resetToIndex(int idx)
{
	while (static_cast<int>(m_historyActions.size()) - 1 > idx) {
		m_historyActions.back()->undo();
		m_historyActions.pop_back();
	}

	while (static_cast<int>(m_postActionGold.size()) - 1 > idx) {
		m_postActionGold.pop_back();
	}

	if (m_dayStartIndexes.size() > 0) {
		while (static_cast<int>(m_dayStartIndexes.back()) > idx) {
			m_dayStartIndexes.pop_back();
		}
	}

	assert(m_historyActions.size() == m_postActionGold.size());
}

void History::updateRemainingGold(PriceData const& priceData, PriceModsFlags modifiers)
{
	if (m_historyActions.size() == 0u)
		return;

	m_postActionGold.clear();
	m_postActionGold.push_back(
	        m_startingGold - m_historyActions.front()->getCost(priceData, modifiers));

	for (std::size_t i = 1; i < m_historyActions.size(); ++i) {
		m_postActionGold.push_back(
		        m_postActionGold[i - 1] - m_historyActions[i]->getCost(priceData, modifiers));
	}

	assert(m_historyActions.size() == m_postActionGold.size());
}

void History::setStartingGold(int gold)
{
	int diff       = gold - m_startingGold;
	m_startingGold = gold;
	std::for_each(m_postActionGold.begin(), m_postActionGold.end(), [diff](auto& a) { a += diff; });
}

void History::setPriceModifiers(PriceModsFlags modifiers, PriceData const& priceData)
{
	m_priceModifiers = modifiers;
	updateRemainingGold(priceData, modifiers);
}

std::vector<IHistoryAction const*> History::getActions() const
{
	std::vector<IHistoryAction const*> actions;
	actions.reserve(m_historyActions.size());

	for (auto const& action : m_historyActions)
		actions.push_back(action.get());

	return actions;
}
