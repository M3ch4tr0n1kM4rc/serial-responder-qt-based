// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: configmanager.h                                    │
// │    Description: definition of ConfigManager class                  │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QJsonArray>
#include <QSerialPort>
#include <QString>

#include "datatablemodel.h"

class ConfigManager {
   public:
    explicit ConfigManager(DataTableModel* data_model);

	bool load(const QString& filePath);
	bool save(const QString& filePath) const;

	QString portName;
	qint32 baudRate = QSerialPort::Baud115200;
	QSerialPort::DataBits dataBits = QSerialPort::Data8;
	QSerialPort::Parity parity = QSerialPort::NoParity;
	QSerialPort::StopBits stopBits = QSerialPort::OneStop;
	QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;

	bool defaultHexMode = false;

   private:
	DataTableModel* m_responder_data_model;
};

#endif	// CONFIGMANAGER_H
