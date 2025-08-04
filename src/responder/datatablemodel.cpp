#include "datatablemodel.h"

#include <QJsonArray>
#include <QJsonObject>

#include "matcher.h"

DataTableModel::DataTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

int DataTableModel::rowCount(const QModelIndex &) const
{
	return m_entries.size();
}

int DataTableModel::columnCount(const QModelIndex &) const
{
	return 5;
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) return {};

	const Entry &e = m_entries.at(index.row());
	switch (index.column()) {
		case 0:	 // Eingabedaten
			return e.inputIsHex
					   ? QString::fromUtf8(e.inputData.toHex(' ').toUpper())
					   : QString::fromUtf8(e.inputData);

		case 1:	 // Eingabeformat
			return e.inputIsHex ? QStringLiteral("HEX")
								: QStringLiteral("ASCII");

		case 2:	 // Ausgabedaten
			return e.outputIsHex
					   ? QString::fromUtf8(e.outputData.toHex(' ').toUpper())
					   : QString::fromUtf8(e.outputData);

		case 3:	 // Ausgabeformat
			return e.outputIsHex ? QStringLiteral("HEX")
								 : QStringLiteral("ASCII");
		case 4:	 // Count
			return QString::number(e.counter);

		default:
			return {};
	}
}

QVariant DataTableModel::headerData(int section, Qt::Orientation orientation,
									int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return {};

	switch (section) {
		case 0:
            return QStringLiteral("Incoming pattern");
		case 1:
            return QStringLiteral("Input data format");
		case 2:
            return QStringLiteral("Output data");
		case 3:
            return QStringLiteral("Output data format");
		case 4:
            return QStringLiteral("Detection count");
		default:
			return {};
	}
}

Qt::ItemFlags DataTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool DataTableModel::setData(const QModelIndex &index, const QVariant &value,
							 int role)
{
	if (!index.isValid() || role != Qt::EditRole) return false;

	Entry &e = m_entries[index.row()];
	QByteArray raw = value.toString().toUtf8();

	switch (index.column()) {
        case 0:
			e.inputData = e.inputIsHex ? QByteArray::fromHex(raw) : raw;
			emit dataChanged(index, index);
			return true;

        case 1: {
			bool newHex = (value.toString().toUpper() == "HEX");
			if (newHex != e.inputIsHex) {
				e.inputIsHex = newHex;
				auto top = this->index(index.row(), 0);
				auto bottom = this->index(index.row(), 1);
				emit dataChanged(top, bottom);
			}
			return true;
		}

        case 2:
			e.outputData = e.outputIsHex ? QByteArray::fromHex(raw) : raw;
			emit dataChanged(index, index);
			return true;

        case 3: {
			bool newHex = (value.toString().toUpper() == "HEX");
			if (newHex != e.outputIsHex) {
				e.outputIsHex = newHex;
				auto top = this->index(index.row(), 2);
				auto bottom = this->index(index.row(), 3);
				emit dataChanged(top, bottom);
			}
			return true;
		}

		default:
			return false;
	}
}

void DataTableModel::clear()
{
	beginResetModel();
	m_entries.clear();
	endResetModel();
}

void DataTableModel::remove(int index)
{
	if (index < 0 || index >= m_entries.size()) return;

	beginRemoveRows(QModelIndex(), index, index);
	m_entries.removeAt(index);
	endRemoveRows();
}

void DataTableModel::addEntry(const QByteArray &in_data, bool in_hex,
                              const QByteArray &out_data, bool out_hex)
{
	int newRow = m_entries.size();
	beginInsertRows(QModelIndex(), newRow, newRow);
    m_entries.append({in_data, out_data, in_hex, out_hex});
	endInsertRows();
}

QByteArray DataTableModel::findResponse(const QByteArray &input_data,
										bool *found_p)
{
	bool found = false;
	QByteArray response_data;

	for (int i = 0; i < m_entries.size(); ++i) {
		if (Matcher::kmpSearch(input_data, m_entries[i].inputData) >= 0) {
			response_data.append(m_entries[i].outputData);
			m_entries[i].counter++;

            QModelIndex changedIndex = this->index(i, 4);
			emit dataChanged(changedIndex, changedIndex);

			found = true;
            break;
		}
	}

	if (nullptr != found_p) {
		*found_p = found;
	}
	return response_data;
}

QJsonArray DataTableModel::toJsonArray() const
{
	QJsonArray array;
	for (const Entry &e : m_entries) {
		QJsonObject obj;
		obj["input"] = QString::fromUtf8(e.inputData.toHex());
		obj["inputIsHex"] = e.inputIsHex;
		obj["output"] = QString::fromUtf8(e.outputData.toHex());
		obj["outputIsHex"] = e.outputIsHex;
		obj["count"] = QString::number(e.counter);
		array.append(obj);
	}
	return array;
}
