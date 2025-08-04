#ifndef ASCIIHEXDELEGATE_H
#define ASCIIHEXDELEGATE_H

#include <QLineEdit>
#include <QPainter>
#include <QStyledItemDelegate>

class AsciiHexDelegate : public QStyledItemDelegate {
	Q_OBJECT

   public:
	explicit AsciiHexDelegate(QObject* parent = nullptr);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
						  const QModelIndex& index) const override;

	void setEditorData(QWidget* editor,
					   const QModelIndex& index) const override;

	void setModelData(QWidget* editor, QAbstractItemModel* model,
					  const QModelIndex& index) const override;

	void updateEditorGeometry(QWidget* editor,
							  const QStyleOptionViewItem& option,
							  const QModelIndex& index) const override;

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
			   const QModelIndex& index) const override;
};

#endif	// ASCIIHEXDELEGATE_H
