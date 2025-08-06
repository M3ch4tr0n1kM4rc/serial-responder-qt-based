// SPDX-License-Identifier: GPL-3.0-only

#include "serialwidget.h"

#include <QSerialPortInfo>

#include "configmanager.h"

SerialWidget::SerialWidget(SerialManager* serial_manager, QWidget* parent)
	: QWidget(parent),
	  m_serial_manager(serial_manager),
	  m_led_frame(new QFrame(this)),
	  m_port_combo_box(new QComboBox(this)),
	  m_baudrate_combo_box(new QComboBox(this)),
	  m_data_bits_combo_box(new QComboBox(this)),
	  m_parity_combo_box(new QComboBox(this)),
	  m_stop_bits_combo_box(new QComboBox(this)),
	  m_flow_ctrl_combo_box(new QComboBox(this)),
      m_connect_button(new QPushButton("Connect", this)),
	  m_refresh_button(new QPushButton("R", this))
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_led_frame);
	layout->addWidget(m_connect_button);
	layout->addWidget(m_refresh_button);
	layout->addWidget(m_port_combo_box);
	layout->addWidget(m_baudrate_combo_box);
	layout->addWidget(m_data_bits_combo_box);
	layout->addWidget(m_parity_combo_box);
	layout->addWidget(m_stop_bits_combo_box);
	layout->addWidget(m_flow_ctrl_combo_box);
	setLayout(layout);

	initWidgets();
	populateSerialCombos();

	connect(m_connect_button, &QPushButton::clicked, this,
			&SerialWidget::connectClicked);

	connect(m_refresh_button, &QPushButton::clicked, this,
			&SerialWidget::populateSerialCombos);

	updateLed(LedState::Disconnected);
}

void SerialWidget::initWidgets()
{
	m_led_frame->setMinimumSize({20, 20});
	m_led_frame->setMaximumSize({20, 20});
	m_connect_button->setMaximumWidth(100);
	m_refresh_button->setMaximumWidth(30);
	m_refresh_button->setToolTip("Refresh the list of serial devices");
	m_port_combo_box->setMaximumWidth(500);
	m_baudrate_combo_box->setMaximumWidth(120);
	m_data_bits_combo_box->setMaximumWidth(90);
	m_parity_combo_box->setMaximumWidth(120);
	m_stop_bits_combo_box->setMaximumWidth(120);
	m_flow_ctrl_combo_box->setMaximumWidth(150);
}

void SerialWidget::setConfiguration(const ConfigManager* config_manager)
{
	m_port_combo_box->setCurrentText(config_manager->portName);
	m_baudrate_combo_box->setCurrentText(QString::number(config_manager->baudRate));
	m_data_bits_combo_box->setCurrentText(QString::number(int(config_manager->dataBits)));
	m_parity_combo_box->setCurrentIndex(int(config_manager->parity));
	m_stop_bits_combo_box->setCurrentIndex(int(config_manager->stopBits));
	m_flow_ctrl_combo_box->setCurrentIndex(int(config_manager->flowControl));
}

void SerialWidget::getConfiguration(ConfigManager* config_manager)
{
	if (nullptr == config_manager) {
		return;
	}
	config_manager->portName = getSelectedPortName();
	config_manager->baudRate = m_baudrate_combo_box->currentText().toInt();
	config_manager->dataBits =
		QSerialPort::DataBits(m_data_bits_combo_box->currentText().toInt());
	config_manager->parity = QSerialPort::Parity(m_parity_combo_box->currentIndex());
	config_manager->stopBits =
		QSerialPort::StopBits(m_stop_bits_combo_box->currentIndex());
	config_manager->flowControl =
		QSerialPort::FlowControl(m_flow_ctrl_combo_box->currentIndex());
}

void SerialWidget::populateSerialCombos()
{
	m_port_combo_box->clear();
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
		m_port_combo_box->addItem(createPortDropDownItem(info));
    }
    if (m_baudrate_combo_box->count() == 0) {
		m_baudrate_combo_box->addItems({"9600", "19200", "38400", "57600", "115200"});
        m_baudrate_combo_box->setCurrentIndex(4);
    }
    if (m_data_bits_combo_box->count() == 0) {
        m_data_bits_combo_box->addItems({"5", "6", "7", "8"});
        m_data_bits_combo_box->setCurrentIndex(3);
    }
    if (m_parity_combo_box->count() == 0) {
		m_parity_combo_box->addItems({"NoParity", "EvenParity", "OddParity"});
        m_parity_combo_box->setCurrentIndex(0);
    }
    if (m_stop_bits_combo_box->count() == 0) {
		m_stop_bits_combo_box->addItems({"OneStop", "OneAndHalfStop", "TwoStop"});
        m_stop_bits_combo_box->setCurrentIndex(0);
    }
    if (m_flow_ctrl_combo_box->count() == 0) {
		m_flow_ctrl_combo_box->addItems(
			{"NoFlowControl", "HardwareControl", "SoftwareControl"});
        m_flow_ctrl_combo_box->setCurrentIndex(0);
    }
}

QString SerialWidget::createPortDropDownItem(
	const QSerialPortInfo& port_info) const
{
	if (port_info.manufacturer().isEmpty()) {
		return port_info.portName();
	}
	return port_info.portName() + " <" + port_info.manufacturer() + ">";
}

QString SerialWidget::getSelectedPortName() const
{
	QString port_name = m_port_combo_box->currentText();
	if (port_name.contains("<")) {
		port_name = port_name.split("<").front().trimmed();
	}
	return port_name;
}

// Getters
QString SerialWidget::selectedPort() const
{
	return getSelectedPortName();
}
int SerialWidget::selectedBaudRate() const
{
	return m_baudrate_combo_box->currentText().toInt();
}
QSerialPort::DataBits SerialWidget::selectedDataBits() const
{
	return static_cast<QSerialPort::DataBits>(
		m_data_bits_combo_box->currentText().toInt());
}
QSerialPort::Parity SerialWidget::selectedParity() const
{
	return static_cast<QSerialPort::Parity>(m_parity_combo_box->currentIndex());
}
QSerialPort::StopBits SerialWidget::selectedStopBits() const
{
	return static_cast<QSerialPort::StopBits>(m_stop_bits_combo_box->currentIndex());
}
QSerialPort::FlowControl SerialWidget::selectedFlowControl() const
{
	return static_cast<QSerialPort::FlowControl>(
		m_flow_ctrl_combo_box->currentIndex());
}

void SerialWidget::updateLed(LedState state)
{
	QString style;
	switch (state) {
		case LedState::Connected:
			style = "background-color:limegreen;";
			break;
		case LedState::Disconnected:
			style = "background-color:lightgray;";
			break;
		case LedState::Error:
			style = "background-color:red;";
			break;
	}
	m_led_frame->setStyleSheet("border-radius:8px; " + style);
}

void SerialWidget::connectClicked()
{
	if (m_serial_manager->isOpen()) {
		m_serial_manager->closePort();
        m_connect_button->setText("Connect");
		updateLed(LedState::Disconnected);
		return;
	}

	bool ok = m_serial_manager->openPort(
		getSelectedPortName(), m_baudrate_combo_box->currentText().toInt(),
		static_cast<QSerialPort::DataBits>(m_data_bits_combo_box->currentText().toInt()),
		static_cast<QSerialPort::Parity>(m_parity_combo_box->currentIndex()),
		static_cast<QSerialPort::StopBits>(m_stop_bits_combo_box->currentIndex()),
		static_cast<QSerialPort::FlowControl>(m_flow_ctrl_combo_box->currentIndex()));
	if (ok) {
        m_connect_button->setText("Disconnect");
		updateLed(LedState::Connected);
	}
	else {
		updateLed(LedState::Error);
	}
}
