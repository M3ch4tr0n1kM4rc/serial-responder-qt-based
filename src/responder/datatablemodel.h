// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: datatablemodel.h                                   │
// │    Description: definition of DataTableModel class                 │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef DATATABLEMODEL_H
#define DATATABLEMODEL_H

#include <QAbstractTableModel>
#include <QByteArray>
#include <QJsonArray>
#include <QVector>

class DataTableModel : public QAbstractTableModel {
	Q_OBJECT

   public:
	explicit DataTableModel(QObject *parent = nullptr);

	// Model-Overrides
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;

	void remove(int index);

    // Custom API
	void clear();
    void addEntry(const QByteArray &in_data, bool in_hex,
                  const QByteArray &out_data, bool out_hex);

	QJsonArray toJsonArray() const;
	QByteArray findResponse(const QByteArray &input_data, bool *found_p);

   private:
	struct Entry {
		QByteArray inputData;
		QByteArray outputData;
		bool inputIsHex;
		bool outputIsHex;
		uint64_t counter;
	};

	QVector<Entry> m_entries;
};

#endif	// DATATABLEMODEL_H
