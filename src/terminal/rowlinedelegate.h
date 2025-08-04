#ifndef ROWLINEDELEGATE_H
#define ROWLINEDELEGATE_H

#include <QStyledItemDelegate>

class RowLineDelegate : public QStyledItemDelegate {
   public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
			   const QModelIndex& index) const override;

};
#endif	// ROWLINEDELEGATE_H
