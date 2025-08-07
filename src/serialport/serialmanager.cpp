// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: serialmanager.cpp                        │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent)
	: QObject(parent),
	  m_serial_port(this)
{
	connect(&m_serial_port, &QSerialPort::readyRead, this,
			&SerialManager::onReadyRead);
	connect(&m_serial_port, &QSerialPort::errorOccurred, this,
			&SerialManager::onError);
    connect(&m_pinout_timer, &QTimer::timeout,
            this, &SerialManager::updatePinouts);
}

SerialManager::~SerialManager()
{
    closePort();
}

bool SerialManager::openPort(const QString &portname, qint32 baudrate,
                             QSerialPort::DataBits databits,
                             QSerialPort::Parity parity,
                             QSerialPort::StopBits stopbits,
                             QSerialPort::FlowControl flowcontrol)
{
    if (m_serial_port.isOpen()) {
        closePort();
    }

    m_serial_port.setPortName(portname);
    m_serial_port.setBaudRate(baudrate);
    m_serial_port.setDataBits(databits);
	m_serial_port.setParity(parity);
    m_serial_port.setStopBits(stopbits);
    m_serial_port.setFlowControl(flowcontrol);

	if (!m_serial_port.open(QIODevice::ReadWrite)) {
		emit errorOccurred(m_serial_port.errorString());
		return false;
	}
    emit connectionEstablished(true, m_serial_port.portName());
    m_pinout_timer.start(250);
	return true;
}

void SerialManager::closePort()
{
    if (m_serial_port.isOpen()) {
        m_pinout_timer.stop();
        m_serial_port.close();
    }
    emit updatePinouts();
    emit connectionEstablished(false, m_serial_port.portName());
}

bool SerialManager::isOpen() const
{
	return m_serial_port.isOpen();
}

void SerialManager::sendData(const QByteArray &data)
{
	if (m_serial_port.isOpen()) m_serial_port.write(data);
}

void SerialManager::handleRTS(bool value)
{
    if (m_serial_port.isOpen()) {
        m_serial_port.setRequestToSend(value);
    }
}

void SerialManager::handleDTR(bool value)
{
    if (m_serial_port.isOpen()) {
        m_serial_port.setDataTerminalReady(value);
    }
}

void SerialManager::updatePinouts() {
    if (!m_serial_port.isOpen()) {
        if (m_pinout_timer.isActive()) {
            m_pinout_timer.stop();
        }
        emit setPinouts(false, false, false, false);
        return;
    }
    QSerialPort::PinoutSignals pinout_signals = m_serial_port.pinoutSignals();
    emit setPinouts(pinout_signals & QSerialPort::RingIndicatorSignal,
                    pinout_signals & QSerialPort::DataCarrierDetectSignal,
                    pinout_signals & QSerialPort::ClearToSendSignal,
                    pinout_signals & QSerialPort::DataSetReadySignal);
}

void SerialManager::onReadyRead()
{
	QByteArray chunk = m_serial_port.readAll();
	emit dataReceived(chunk);
}

void SerialManager::onError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::NoError) {
		return;
	}
	if (error == QSerialPort::PermissionError) {
        const QString errorString = m_serial_port.errorString();
        if (!errorString.contains("locking")) {
            const QString msg = m_serial_port.errorString() +
                                " Linux Fix: Try to add user to 'dialout' group"
                                " with:\n'sudo usermod -aG dialout $USER'.\n"
                                " After that 'logout' is required!";
            emit errorOccurred(msg);
        }
		return;
	}

	emit errorOccurred(m_serial_port.errorString());
}
