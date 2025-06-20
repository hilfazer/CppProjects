#include "HistoryModel.hpp"
#include "history/History.hpp"
#include "history/action/HistoryAction.hpp"

#include <QtGui/QColor>

namespace
{

int const ColumnCount = 3;
QColor    InvalidBackground(155, 0, 0, 127);

} // namespace

HistoryModel::HistoryModel(History const& history) : m_history(history)
{
}

void HistoryModel::update()
{
	emit layoutAboutToBeChanged();
	auto actions = m_history.getActions();
	m_actionTexts.clear();
	m_actionTexts.reserve(static_cast<int>(actions.size()));
	m_actionTooltips.clear();
	m_actionTooltips.reserve(static_cast<int>(actions.size()));

	for (auto action : actions) {
		m_actionTexts.append(action->getDescription());
		m_actionTooltips.append(action->getTooltip());
	}

	// changePersistentIndex();
	emit layoutChanged();
}

int HistoryModel::rowCount(QModelIndex const& parent) const
{
	if (parent.isValid())
		return 0;

	return static_cast<int>(m_actionTexts.size());
}

int HistoryModel::columnCount(QModelIndex const& parent) const
{
	if (parent.isValid())
		return 0;

	return ColumnCount;
}

QVariant HistoryModel::data(QModelIndex const& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch (index.column()) {
	case ColumnIdx::Text:
		switch (role) {
		case Qt::DisplayRole:
			return m_actionTexts.at(index.row());
		case Qt::ToolTipRole:
			return m_actionTooltips.at(index.row());
		default:
			return QVariant();
		}

	case ColumnIdx::Gold:
		switch (role) {
		case Qt::DisplayRole:
			return m_history.getRemainingGold().at(static_cast<std::size_t>(index.row()));
		case Qt::BackgroundRole:
			return m_history.getRemainingGold().at(static_cast<std::size_t>(index.row())) < 0
			               ? InvalidBackground
			               : QVariant();
		default:
			return QVariant();
		}

	default:
		return QVariant();
	}
}
