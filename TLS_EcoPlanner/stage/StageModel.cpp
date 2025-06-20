#include "StageModel.hpp"

StageModel::StageModel(Stage& stage) : m_stage(stage)
{
}

void StageModel::update()
{
	emit layoutAboutToBeChanged();

	// update data
	m_rowTexts.clear();

	if (m_stage.getShop() != nullptr) {
		m_rowTexts.append(m_stage.getShop()->getDescription());
	}

	for (auto i = 0u; i < m_stage.getRuins().size(); ++i) {
		m_rowTexts.append(m_stage.getRuins()[i].getDescription(i));
	}

	for (auto i = 0u; i < m_stage.getHouses().size(); ++i) {
		m_rowTexts.append(m_stage.getHouses()[i].getDescription(i));
	}

	for (auto i = 0u; i < m_stage.getGoldMines().size(); ++i) {
		m_rowTexts.append(m_stage.getGoldMines()[i].getDescription(i));
	}

	// changePersistentIndex();
	emit layoutChanged();
}

int StageModel::rowCount(QModelIndex const& parent) const
{
	Q_UNUSED(parent)
	return static_cast<int>(m_rowTexts.size());
}

QVariant StageModel::data(QModelIndex const& index, int role) const
{
	assert(index.row() < m_rowTexts.size());

	switch (role) {
	case Qt::DisplayRole:
		return m_rowTexts.at(index.row());
	default:
		return {};
	}
}

QModelIndex StageModel::index(int row, int column, QModelIndex const& parent) const
{
	Q_UNUSED(parent)
	return createIndex(row, column, nullptr);
}
