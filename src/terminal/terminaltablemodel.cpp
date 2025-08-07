// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: terminaltablemodel.cpp                             │
// │    Description: implementation of TerminalTableModel class         │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#include "terminaltablemodel.h"

#include <QBrush>
#include <QString>

TerminalTableModel::TerminalTableModel(QObject* parent)
	: QAbstractTableModel(parent),
	  m_data_entries(),
	  m_wrap_on_new_line(false),
	  m_max_columns(40),
	  m_ascii_foreground("#f0f5f5"),
	  m_ascii_background("#6aa4a4"),
	  m_hex_foreground("#f6f3f3"),
      m_hex_background("#7878b2"),
      m_selected_byte({0, QDateTime(), false})
{
}

void TerminalTableModel::setAsciiColor(const QColor& foreground,
									   const QColor& background)
{
	m_ascii_foreground = foreground;
	m_ascii_background = background;
}

void TerminalTableModel::setHexColor(const QColor& foreground,
									 const QColor& background)
{
	m_hex_foreground = foreground;
	m_hex_background = background;
}

void TerminalTableModel::appendEntry(const QByteArray& data,
                                     const QDateTime& timestamp,
                                     bool is_received)
{
	QList<TerminalByte> byteList;
	for (char byteChar : data) {
		byteList.append({.value = byteChar,
						 .timestamp = timestamp,
                         .isReceived = is_received});
	}
	m_data_entries.append(TerminalEntry{byteList});
	rewrapData(m_data_entries, m_max_columns);
	beginResetModel();
	endResetModel();
}

void TerminalTableModel::setWrapOnNewline(bool enabled)
{
	m_wrap_on_new_line = enabled;
	rewrapData(m_data_entries, m_max_columns);
}

void TerminalTableModel::setMaxColumns(int count)
{
	if (m_max_columns != count) {
		m_max_columns = count;
		rewrapData(m_data_entries, m_max_columns);
	}
}

int TerminalTableModel::rowCount(const QModelIndex&) const
{
	return qMax(2, m_data_entries.size() * 2);
}

int TerminalTableModel::columnCount(const QModelIndex&) const
{
	if (!m_wrap_on_new_line) {
		return m_max_columns +
			   1;  // Include dummy column when wrapping is disabled
	}

	// Optional: respect newline-based wrapping
	int maxLen = 0;
	for (const auto& entry : m_data_entries) {
		int currentLen = entry.bytes.length();
		if (currentLen > maxLen) {
			maxLen = currentLen;
		}
	}
	return maxLen + 1;	// Add dummy column when wrapping is enabled
}

void TerminalTableModel::rewrapData(const QList<TerminalEntry>& entries,
									int max_columns)
{
	QList<TerminalByte> allBytes;
	// Flatten all byte info into a single list
	for (const auto& entry : entries) {
		for (const TerminalByte& c : entry.bytes) {
			allBytes.append(c);
		}
	}

	m_data_entries.clear();

	if (m_wrap_on_new_line) {
		// Wrap on line breaks (\n or \r)
		TerminalEntry currentEntry;
		int columnCount = 0;

		for (int i = 0; i < allBytes.size(); ++i) {
			const TerminalByte& byte = allBytes[i];
			bool shouldSplit = false;

			// Check if current byte is newline
			if (byte.value == '\n' || byte.value == '\r') {
				// Look ahead to check for combination
				if (i + 1 < allBytes.size()) {
					char nextChar = allBytes[i + 1].value;
					if ((byte.value == '\n' && nextChar == '\r') ||
						(byte.value == '\r' && nextChar == '\n')) {
						// Treat combo as one split marker
						currentEntry.bytes.append(byte);
						currentEntry.bytes.append(allBytes[i + 1]);
						++i;
					}
					else {
						currentEntry.bytes.append(byte);
					}
				}
				else {
					currentEntry.bytes.append(byte);
				}
				shouldSplit = true;
			}
			else {
				currentEntry.bytes.append(byte);
				columnCount++;
				// Split by max column count
				if (columnCount >= max_columns) {
					shouldSplit = true;
				}
			}

			if (shouldSplit && !currentEntry.bytes.isEmpty()) {
				m_data_entries.append(currentEntry);
				currentEntry.bytes.clear();
				columnCount = 0;
			}
		}
		if (!currentEntry.bytes.isEmpty()) {
			m_data_entries.append(currentEntry);
		}
	}
	else {
		// Wrap by column count
		int pos = 0;
		while (pos < allBytes.size()) {
			TerminalEntry entry;
			int chunkSize = qMin(max_columns, allBytes.size() - pos);
			entry.bytes = allBytes.mid(pos, chunkSize);
			m_data_entries.append(entry);
			pos += chunkSize;
		}
	}

	beginResetModel();
	endResetModel();
}

QVariant TerminalTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) return {};

	int entryIndex = index.row() / 2;
	bool isAsciiRow = (index.row() % 2 == 0);

	if (m_data_entries.isEmpty()) {
		if (index.column() == columnCount() - 1) {
			if (role == Qt::DisplayRole) return QString();	// Empty
			if (role == Qt::ToolTipRole)
				return "Dummy column for layout alignment";
			if (role == Qt::TextAlignmentRole) return Qt::AlignCenter;
			return {};
		}
	}

	if (entryIndex >= m_data_entries.size()) {
		return {};
	}

	const TerminalEntry& entry = m_data_entries.at(entryIndex);
	int dataSize = entry.bytes.size();

	// Dummy column
	if (index.column() == columnCount() - 1) {
		if (role == Qt::DisplayRole) return QString();	// Empty
		if (role == Qt::ToolTipRole) return "Dummy column for layout alignment";
		if (role == Qt::TextAlignmentRole) return Qt::AlignCenter;
		return {};
	}

	// Out-of-range data
	if (index.column() >= dataSize) {
		return {};
	}

	TerminalByte byte = entry.bytes.at(index.column());
	switch (role) {
		case Qt::DisplayRole:
			if (isAsciiRow) {
				if (byte.value == '\n') return QStringLiteral("\\n");
				if (byte.value == '\r') return QStringLiteral("\\r");
				return QString(QChar(byte.value));
			}
			else {
				return QString("%1")
                    .arg(static_cast<unsigned int>(byte.value), 2, 16,
						 QLatin1Char('0'))
					.toUpper();
			}
		case Qt::ForegroundRole:
			return isAsciiRow ? QBrush(m_ascii_foreground)
							  : QBrush(m_hex_foreground);
		case Qt::BackgroundRole:
            if (byte.timestamp == m_selected_byte.timestamp) {
                return isAsciiRow ? QBrush(QColor(200, 100, 0))
                                  : QBrush(QColor(125, 0, 0));
            }
			return isAsciiRow ? QBrush(m_ascii_background)
							  : QBrush(m_hex_background);
		case Qt::ToolTipRole:
            return QString("time: %1\nreceived: %2")
				.arg(byte.timestamp.toString("HH:mm:ss.zzz"))
                .arg(byte.isReceived ? "Yes" : "No");
		case Qt::TextAlignmentRole:
			return Qt::AlignCenter;
	}

	return {};
}

QVariant TerminalTableModel::headerData(int section,
										Qt::Orientation orientation,
										int role) const
{
	if (role != Qt::DisplayRole) return {};

	if (orientation == Qt::Vertical) {
		if (m_data_entries.isEmpty()) {
			return (section % 2 == 0) ? "ASCII (0)" : "HEX (0)";
		}

		int entryIndex = section / 2;
		QString format = (section % 2 == 0) ? "ASCII" : "HEX";
		return QString("%1 (%2)").arg(format).arg(entryIndex);
	}

	// Hide header for dummy column
	if (section == columnCount() - 1) return "";

	return QString::number(section + 1);
}

Qt::ItemFlags TerminalTableModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return {};

	if (index.column() == columnCount() - 1) return Qt::NoItemFlags;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TerminalTableModel::handleSelection(const QModelIndex &index)
{
    m_selected_byte = findSelection(index);
    beginResetModel();
    endResetModel();
}

TerminalByte TerminalTableModel::findSelection(const QModelIndex &index) const
{
    int entryIndex = index.row() / 2;
    if (m_data_entries.isEmpty()) {
        return {};

    }
    if (entryIndex >= m_data_entries.size()) {
        return {};
    }
    int column = index.column();
    if (column == columnCount() - 1) {
        return {};
    }
    const TerminalEntry& entry = m_data_entries.at(entryIndex);
    int dataSize = entry.bytes.size();
    if (column >= dataSize) {
        return {};
    }
    return entry.bytes.at(column);
}

void TerminalTableModel::clearData()
{
    m_selected_byte = {};
	m_data_entries.clear();
	beginResetModel();
	endResetModel();
}
