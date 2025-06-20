#pragma once

#include "stage/Stage.hpp"

#include <QAbstractListModel>
#include <QtCore/QObject>

class StageModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit StageModel(Stage& stage);
	void update();

	// QAbstractItemModel interface
public:
	int         rowCount(QModelIndex const& parent) const override;
	QVariant    data(QModelIndex const& index, int role) const override;
	QModelIndex index(int row, int column, QModelIndex const& parent) const override;

private:
	Stage const& m_stage;
	QStringList  m_rowTexts;
};
