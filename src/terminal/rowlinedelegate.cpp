// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: rowlinedelegate.cpp                      │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

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
