#ifndef RESPONDERPACKETHANDLER_H
#define RESPONDERPACKETHANDLER_H

#include <QByteArray>
#include <QObject>

#include "datatablemodel.h"

class ResponderPacketHandler : public QObject {
	Q_OBJECT

   public:
	explicit ResponderPacketHandler(DataTableModel *model,
									QObject *parent = nullptr);

	void processData(const QByteArray &chunk);

   signals:
	void packetReceived(const QByteArray &packet);
	void responseReady(const QByteArray &response);

   private:
	bool isPacketComplete() const;
	QByteArray extractPacket();
	QByteArray createResponse(const QByteArray &packet);

   private:
	QByteArray m_buffer;
	DataTableModel *m_data_model;
};

#endif	// RESPONDERPACKETHANDLER_H
