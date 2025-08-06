// SPDX-License-Identifier: GPL-3.0-only

#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent)
	: QObject(parent),
	  m_serial_port(this)
{
	connect(&m_serial_port, &QSerialPort::readyRead, this,
			&SerialManager::onReadyRead);
	connect(&m_serial_port, &QSerialPort::errorOccurred, this,
			&SerialManager::onError);
}

SerialManager::~SerialManager()
{
	if (m_serial_port.isOpen()) m_serial_port.close();
}

bool SerialManager::openPort(const QString &portname, qint32 baudrate,
                             QSerialPort::DataBits databits,
                             QSerialPort::Parity parity,
                             QSerialPort::StopBits stopbits,
                             QSerialPort::FlowControl flowcontrol)
{
	if (m_serial_port.isOpen()) m_serial_port.close();

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
	return true;
}

void SerialManager::closePort()
{
	if (m_serial_port.isOpen()) m_serial_port.close();
}

bool SerialManager::isOpen() const
{
	return m_serial_port.isOpen();
}

void SerialManager::sendData(const QByteArray &data)
{
	if (m_serial_port.isOpen()) m_serial_port.write(data);
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
		const QString msg = m_serial_port.errorString() +
							" Linux Fix: Try to add user to 'dialout' group"
							" with:\n'sudo usermod -aG dialout $USER'.\n"
							" After that 'logout' is required!";
		emit errorOccurred(msg);
		return;
	}

	emit errorOccurred(m_serial_port.errorString());
}
