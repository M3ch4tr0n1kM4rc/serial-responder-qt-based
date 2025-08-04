#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QComboBox>
#include <QPushButton>
#include <QSerialPort>
#include <QVBoxLayout>
#include <QWidget>

#include "configmanager.h"
#include "serialmanager.h"

class SerialWidget : public QWidget {
	Q_OBJECT

   public:
	enum class LedState { Disconnected, Connected, Error };

	explicit SerialWidget(SerialManager* serial_manager,
						  QWidget* parent = nullptr);

	QString selectedPort() const;
	int selectedBaudRate() const;
	QSerialPort::DataBits selectedDataBits() const;
	QSerialPort::Parity selectedParity() const;
	QSerialPort::StopBits selectedStopBits() const;
	QSerialPort::FlowControl selectedFlowControl() const;

	void setConfiguration(const ConfigManager* config_manager);
	void getConfiguration(ConfigManager* config_manager);
   public slots:
	void connectClicked();
	void updateLed(LedState state);

   signals:
	void connectRequested();

   private:
	SerialManager* m_serial_manager;

	QFrame* m_led_frame;
	QComboBox* m_port_combo_box;
	QComboBox* m_baudrate_combo_box;
	QComboBox* m_data_bits_combo_box;
	QComboBox* m_parity_combo_box;
	QComboBox* m_stop_bits_combo_box;
	QComboBox* m_flow_ctrl_combo_box;
	QPushButton* m_connect_button;
	QPushButton* m_refresh_button;

	void initWidgets();

	void populateSerialCombos();
	QString createPortDropDownItem(const QSerialPortInfo& port_info) const;
	QString getSelectedPortName() const;
};

#endif	// SERIALWIDGET_H
