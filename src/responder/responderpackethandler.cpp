// SPDX-License-Identifier: GPL-3.0-only

#include "responderpackethandler.h"

ResponderPacketHandler::ResponderPacketHandler(DataTableModel *model,
											   QObject *parent)
	: QObject(parent),
	  m_data_model(model)
{
}

void ResponderPacketHandler::processData(const QByteArray &chunk)
{
	m_buffer.append(chunk);
	bool found = false;
	auto response = m_data_model->findResponse(m_buffer, &found);
	if (found) {
		emit responseReady(response);
		m_buffer.clear();
	}
}

bool ResponderPacketHandler::isPacketComplete() const
{
	if (m_buffer.size() < 1) {
		return false;
	}
	quint8 payloadLength = static_cast<quint8>(m_buffer.at(0));
	return m_buffer.size() >= (1 + payloadLength);
}

QByteArray ResponderPacketHandler::extractPacket()
{
	quint8 payloadLength = static_cast<quint8>(m_buffer.at(0));
	QByteArray packet = m_buffer.mid(1, payloadLength);
	m_buffer.remove(0, 1 + payloadLength);
	return packet;
}

QByteArray ResponderPacketHandler::createResponse(const QByteArray &packet)
{
	for (int i = 0; i < m_data_model->rowCount(); ++i) {
		// input column
		QModelIndex inIndex = m_data_model->index(i, 0);
		QByteArray input = m_data_model->data(inIndex, Qt::UserRole).toByteArray();
		// output column
		QModelIndex outIndex = m_data_model->index(i, 2);
		QByteArray output = m_data_model->data(outIndex, Qt::UserRole).toByteArray();

		if (input == packet) {
			return output;
		}
	}
	return packet;
}
