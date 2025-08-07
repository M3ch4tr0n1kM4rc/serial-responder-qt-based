// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: terminalviewwidget.h                     │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

#ifndef TERMINALVIEWWIDGET_H
#define TERMINALVIEWWIDGET_H

#include <QCheckBox>
#include <QDateTime>
#include <QSpinBox>
#include <QTableView>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

#include "terminaltablemodel.h"

class TerminalViewWidget : public QWidget {
	Q_OBJECT

   public:
	explicit TerminalViewWidget(bool top_buttons = true,
								QWidget* parent = nullptr);

	void appendData(const QByteArray& data, const QDateTime& timestamp,
                    bool is_received);

   private slots:
	void updateDisplayFormat();
	void updateColumnCount(int count);
	void updateNewlineBehavior(bool enabled);
    void setSelection(const QModelIndex &index);
	void clearTable();

   private:
	void initializeTable();
	void initializeWidgets();
	void applyFormatFilter(const QString& mode);
	void applyCellWidth(const QString& mode);

	QTableView* m_table_view;
	TerminalTableModel* m_terminal_data_model;

	QToolBar* m_toolbar;

	QToolButton* m_clear_button;
	QToolButton* m_ascii_enable_tool_button;
	QToolButton* m_hex_enable_tool_button;
	QSpinBox* m_column_count_spin_box;
	QCheckBox* m_newline_check_box;

	QString m_display_mode;
};

#endif	// TERMINALVIEWWIDGET_H
