#include "rowlinedelegate.h"

#include <QPainter>

void RowLineDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
		   const QModelIndex& index) const {
	QStyledItemDelegate::paint(painter, option, index);

	// Add line under every second row pair
	if ((index.row() % 2) == 1) {
		painter->save();
		painter->setPen(QColor("#404244"));
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
		painter->restore();
	}
}
