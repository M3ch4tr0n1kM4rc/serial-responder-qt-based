// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: serialmanager.h                                    │
// │    Description: definition of SerialManager class                  │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialManager : public QObject {
	Q_OBJECT

   public:
	explicit SerialManager(QObject *parent = nullptr);
	~SerialManager();

    bool openPort(const QString &portname, qint32 baudrate,
                  QSerialPort::DataBits databits, QSerialPort::Parity parity,
                  QSerialPort::StopBits stopbits,
                  QSerialPort::FlowControl flowcontrol);

	void closePort();
	bool isOpen() const;

   public slots:
	void sendData(const QByteArray &data);
    void handleRTS(bool value);
    void handleDTR(bool value);

   signals:
    void setPinouts(bool ri, bool dcd, bool cts, bool dsr);
    void connectionEstablished(bool connected, const QString& port_name);
	void dataReceived(const QByteArray &chunk);
    void errorOccurred(const QString &error_string);

   private slots:
	void onReadyRead();
	void onError(QSerialPort::SerialPortError error);
    void updatePinouts();

private:
	QSerialPort m_serial_port;
    QTimer m_pinout_timer;
};

#endif	// SERIALMANAGER_H
