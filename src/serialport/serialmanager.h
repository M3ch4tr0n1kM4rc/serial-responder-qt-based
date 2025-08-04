#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>

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

   signals:
	void dataReceived(const QByteArray &chunk);
	void errorOccurred(const QString &errorString);

   private slots:
	void onReadyRead();
	void onError(QSerialPort::SerialPortError error);

   private:
	QSerialPort m_serial_port;
};

#endif	// SERIALMANAGER_H
