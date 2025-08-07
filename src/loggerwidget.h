// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: loggerwidget.h                                     │
// │    Description: definition of LoggerWidget class                   │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class LoggerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);
    void addLogEntry(const QString &logLine);
    void addLogEntry(QString timestamp, QString level, QString message, QString source, QString additional);

private slots:
    void handleExportClicked();

private:
    void init();

    QPushButton *m_export_button;
    QTableWidget *m_table_widget;
    QString getEmojiForLevel(const QString &level) const;
    QString getLevelForEmoji(const QString &emoji) const;

};

#endif // LOGGERWIDGET_H
