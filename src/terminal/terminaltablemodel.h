#ifndef TERMINALTABLEMODEL_H
#define TERMINALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <QDateTime>
#include <QList>

struct TerminalByte {
	char value;
	QDateTime timestamp;
	bool isReceived;
};

struct TerminalEntry {
	QList<TerminalByte> bytes;
};

class TerminalTableModel : public QAbstractTableModel {
	Q_OBJECT

   public:
	explicit TerminalTableModel(QObject* parent = nullptr);
	void appendEntry(const QByteArray& data, const QDateTime& timestamp,
                     bool is_received);

	void setWrapOnNewline(bool enabled);
	void setMaxColumns(int count);

	void setAsciiColor(const QColor& foreground, const QColor& background);
	void setHexColor(const QColor& foreground, const QColor& background);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void clearData();

   private:
	QList<TerminalEntry> m_data_entries;
	bool m_wrap_on_new_line;
    int m_max_columns;

	QColor m_ascii_foreground;
	QColor m_ascii_background;

	QColor m_hex_foreground;
	QColor m_hex_background;

	void rewrapData(const QList<TerminalEntry>& entries, int max_columns);
};

#endif	// TERMINALTABLEMODEL_H
