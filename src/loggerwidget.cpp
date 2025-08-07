// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: loggerwidget.cpp                         │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

#include "loggerwidget.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QDateTime>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

LoggerWidget::LoggerWidget(QWidget *parent) : QWidget(parent) {
    m_export_button = new QPushButton("Export Logs",this);
    m_table_widget = new QTableWidget(this);
    m_table_widget->setColumnCount(6);
    m_table_widget->setHorizontalHeaderLabels({"Date", "Time", "Level",
                                      "Message", "Source", "Additional"});

    init();



    connect(m_export_button, &QPushButton::clicked,
            this, &LoggerWidget::handleExportClicked);
}

void LoggerWidget::init()
{
    QHeaderView *header = m_table_widget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::Stretch);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_table_widget);
    layout->addWidget(m_export_button);
    setLayout(layout);

    m_table_widget->setStyleSheet(R"(
        QWidget {
            background-color: #404244;
        }
        /* Base table styling */
        QTableView {
            background-color: #2e2f30;
            color: #ffffff;
            gridline-color: transparent;
            selection-background-color: #3c3d3e;
            selection-color: #ffffff;
        }
        /* Corner widget between headers */
        QTableCornerButton::section {
            background-color: #404244;
            border: none;
        }
        /* Horizontal and vertical headers */
        QHeaderView::section {
            background-color: #404244;
            color: white;
            padding: 4px;
            border: none;
        }
        /* Vertical scrollbar */
        QScrollBar:vertical {
            background-color: #2e2f30;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: #FFCC00;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #FFD633;
        }
        /* Horizontal scrollbar */
        QScrollBar:horizontal {
            background-color: #2e2f30;
            height: 10px;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: #FFCC00;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #FFD633;
        }
        /* Hide scrollbar buttons */
        QScrollBar::add-line, QScrollBar::sub-line {
            background: none;
            border: none;
            height: 0px;
            width: 0px;
        }
    )");
}

void LoggerWidget::addLogEntry(const QString &logLine) {
    // Entferne die Pipes und splitte den String
    QString cleaned = logLine;
    QStringList parts = cleaned.split("|", Qt::SkipEmptyParts);

    if (parts.size() < 3) return;

    QString timestampStr = parts[0].remove("|").trimmed();
    QString level = parts[1].trimmed();
    QString message = parts[2].trimmed();
    QString source = "";

    if (parts.size() == 4) {
        source = parts[3].trimmed();
    }

    QString additional = "";
    if (parts.size() >= 5) {
        additional = parts.mid(3).join(" ").trimmed().simplified();
    }

    addLogEntry(timestampStr, level, message, source, additional);
}

void LoggerWidget::addLogEntry(QString timestamp, QString level,
                               QString message, QString source,
                               QString additional) {

    QDateTime time_point = QDateTime::fromString(timestamp, Qt::ISODate);
    QString date = time_point.date().toString("yyyy-MM-dd");
    QString time = time_point.time().toString("HH:mm:ss.zzz");

    int row = m_table_widget->rowCount();
    m_table_widget->insertRow(row);

    m_table_widget->setItem(row, 0, new QTableWidgetItem(date));
    m_table_widget->setItem(row, 1, new QTableWidgetItem(time));
    m_table_widget->setItem(row, 2, new QTableWidgetItem(getEmojiForLevel(level)));
    m_table_widget->setItem(row, 3, new QTableWidgetItem(message));
    m_table_widget->setItem(row, 4, new QTableWidgetItem(source));
    m_table_widget->setItem(row, 4, new QTableWidgetItem(additional));
}

void LoggerWidget::handleExportClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Logs", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Optional: Fehlermeldung anzeigen
        return;
    }

    QTextStream out(&file);

    for (int row = 0; row < m_table_widget->rowCount(); ++row) {
        QStringList fields;
        for (int col = 0; col < m_table_widget->columnCount(); ++col) {
            QTableWidgetItem *item = m_table_widget->item(row, col);
            QString text = item ? item->text() : "";
            fields << text;
        }

        // Format: |timestamp|level|message|source|additional|
        QString formattedLine = QString("|%1|%2|%3|%4|%5|")
                                    .arg(fields.value(0) + "T" + fields.value(1)) // ISO timestamp
                                    .arg(getLevelForEmoji(fields.value(2)))                         // level
                                    .arg(fields.value(3))                         // message
                                    .arg(fields.value(4))                         // source
                                    .arg(fields.value(5));                        // additional

        out << formattedLine << "\n";
    }

    file.close();
}

QString LoggerWidget::getEmojiForLevel(const QString &level) const {
    if (level == "ERROR") return "(x)";
    if (level == "WARNING") return "(!)";
    if (level == "INFO" || level == "MESSAGE") return "(ℹ️)";
    if (level == "DEBUG") return "(d)";
    return "❓";
}

QString LoggerWidget::getLevelForEmoji(const QString &emoji) const {
    if (emoji == "(x)") return "ERROR";
    if (emoji == "(!)") return "WARNING";
    if (emoji == "(i)") return "INFO";
    if (emoji == "(d)") return "DEBUG";
    return "MESSAGE";
}

