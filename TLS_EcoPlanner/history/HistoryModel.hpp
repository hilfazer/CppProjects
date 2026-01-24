#pragma once

#include <QtCore/QAbstractTableModel>

class History;

class HistoryModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum ColumnIdx { Text = 0, Gold, Reset };

public:
	explicit HistoryModel(History const& history);

	void update();

	int      rowCount(QModelIndex const& parent = QModelIndex()) const override;
	int      columnCount(QModelIndex const& parent = QModelIndex()) const override;
	QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;

private:
	History const& m_history;
	QStringList    m_actionTexts;
	QStringList    m_actionTooltips;
};
