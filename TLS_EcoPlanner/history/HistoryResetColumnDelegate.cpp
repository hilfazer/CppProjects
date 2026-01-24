#include "HistoryResetColumnDelegate.hpp"
#include "history/HistoryModel.hpp"

#include <QPainter>

namespace
{

QPushButton* createResetButtonOnWidget(QWidget* parent)
{
	auto button = new QPushButton("reset", parent);
	return button;
}

} // namespace

HistoryResetColumnDelegate::HistoryResetColumnDelegate(QObject* parent) :
        QStyledItemDelegate(parent)
{
	if (QTableView* tableView = qobject_cast<QTableView*>(parent)) {
		myWidget = tableView;
		resetButton = createResetButtonOnWidget(myWidget);
		resetButton->hide();
		myWidget->setMouseTracking(true);
		connect(myWidget, &QTableView::entered, this, &HistoryResetColumnDelegate::cellEntered);
		isOneCellInEditMode = false;
	}
}

void HistoryResetColumnDelegate::paint(
        QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
	assert(index.column() == HistoryModel::ColumnIdx::Reset);

	resetButton->setGeometry(option.rect);
	resetButton->setText("reset");
	if (option.state == QStyle::State_Selected)
		painter->fillRect(option.rect, option.palette.highlight());

	painter->drawPixmap(option.rect.x(), option.rect.y(), resetButton->grab());
}

QWidget* HistoryResetColumnDelegate::createEditor(
        QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
	assert(index.column() == HistoryModel::ColumnIdx::Reset);
	Q_UNUSED(option)

	auto closure = [this, index]() { emit resetHistoryToIndexRequested(index.row()); };

	QPushButton* btn = createResetButtonOnWidget(parent);
	connect(btn, &QPushButton::clicked, this, closure);

	return btn;
}

void HistoryResetColumnDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const
{
	assert(index.column() == HistoryModel::ColumnIdx::Reset);

	QPushButton* btn = qobject_cast<QPushButton*>(editor);
	btn->setProperty("data_value", index.data());
}

void HistoryResetColumnDelegate::setModelData(
        QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const
{
	assert(index.column() == HistoryModel::ColumnIdx::Reset);

	QPushButton* btn = qobject_cast<QPushButton*>(editor);
	model->setData(index, btn->property("data_value"));
}

void HistoryResetColumnDelegate::updateEditorGeometry(
        QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

void HistoryResetColumnDelegate::cellEntered(QModelIndex const& index)
{
	if (index.column() == HistoryModel::ColumnIdx::Reset) {
		if (isOneCellInEditMode) {
			myWidget->closePersistentEditor(currentEditedCellIndex);
		}
		myWidget->openPersistentEditor(index);
		isOneCellInEditMode    = true;
		currentEditedCellIndex = index;
	}
	else {
		if (isOneCellInEditMode) {
			isOneCellInEditMode = false;
			myWidget->closePersistentEditor(currentEditedCellIndex);
		}
	}
}
