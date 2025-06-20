#pragma once

#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QWidget>

class HistoryResetColumnDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	HistoryResetColumnDelegate(QObject* parent);

	// QAbstractItemDelegate interface
public:
	void
	paint(QPainter* painter, QStyleOptionViewItem const& option,
	      QModelIndex const& index) const override;

	QWidget* createEditor(
	        QWidget* parent, QStyleOptionViewItem const& option,
	        QModelIndex const& index) const override;

	void setEditorData(QWidget* editor, QModelIndex const& index) const override;

	void setModelData(
	        QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const override;

	void updateEditorGeometry(
	        QWidget* editor, QStyleOptionViewItem const& option,
	        QModelIndex const& index) const override;

signals:
	void resetHistoryToIndexRequested(int rowIdx) const;

public slots:
	void cellEntered(QModelIndex const& index);

private:
	QTableView*           myWidget;
	QPushButton*          resetButton;
	bool                  isOneCellInEditMode;
	QPersistentModelIndex currentEditedCellIndex;
};
