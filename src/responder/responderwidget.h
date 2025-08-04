#ifndef RESPONDERWIDGET_H
#define RESPONDERWIDGET_H

#include <QWidget>

#include "datatablemodel.h"
#include "responderpackethandler.h"
#include "serialmanager.h"

namespace Ui {
class ResponderWidget;
}

class ResponderWidget : public QWidget {
	Q_OBJECT

   public:
	explicit ResponderWidget(DataTableModel *serial_data_model,
							 QWidget *parent = nullptr);
	~ResponderWidget();

	bool getHexMode() const;

   signals:
	void responseReady(const QByteArray &chunk);

   public slots:
	void setHexMode(bool hex_mode);
	void handleIncomingData(const QByteArray &chunk);

   private slots:
	void addInputClicked();
	void clearSelectedDataMapping();
	void clearAllDataMapping();
	void handlePacket(const QByteArray &packet);

   private:
	Ui::ResponderWidget *m_ui;
	DataTableModel *m_serial_data_model;
	SerialManager *m_serial_manager;
	ResponderPacketHandler *m_packet_handler;
	void populateFormatCombo();
};

#endif	// RESPONDERWIDGET_H
