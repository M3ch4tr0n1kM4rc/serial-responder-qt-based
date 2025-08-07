// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: responderpackethandler.h                           │
// │    Description: definition of ResponderPacketHandler class         │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

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
