// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: rowlinedelegate.h                        │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

#ifndef ROWLINEDELEGATE_H
#define ROWLINEDELEGATE_H

#include <QStyledItemDelegate>

class RowLineDelegate : public QStyledItemDelegate {
   public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
			   const QModelIndex& index) const override;

};
#endif	// ROWLINEDELEGATE_H
