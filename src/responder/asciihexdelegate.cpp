#include "asciihexdelegate.h"

#include <QApplication>
#include <QLineEdit>

AsciiHexDelegate::AsciiHexDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

QWidget* AsciiHexDelegate::createEditor(QWidget* parent,
										const QStyleOptionViewItem&,
										const QModelIndex&) const
{
	return new QLineEdit(parent);
}

void AsciiHexDelegate::setEditorData(QWidget* editor,
									 const QModelIndex& index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
	if (lineEdit) lineEdit->setText(value);
}

void AsciiHexDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
									const QModelIndex& index) const
{
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
	if (lineEdit) model->setData(index, lineEdit->text(), Qt::EditRole);
}

void AsciiHexDelegate::updateEditorGeometry(QWidget* editor,
											const QStyleOptionViewItem& option,
											const QModelIndex&) const
{
	editor->setGeometry(option.rect);
}

void AsciiHexDelegate::paint(QPainter* painter,
							 const QStyleOptionViewItem& option,
							 const QModelIndex& index) const
{
	QStyleOptionViewItem opt(option);
	initStyleOption(&opt, index);

	bool isAsciiRow = (index.row() % 2 == 0);
	opt.palette.setColor(QPalette::Text,
						 isAsciiRow ? Qt::darkGreen : Qt::darkBlue);

	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
}
