// SPDX-License-Identifier: GPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "configmanager.h"
#include "datatablemodel.h"
#include "loggerwidget.h"
#include "responderwidget.h"
#include "serialmanager.h"
#include "serialwidget.h"
#include "terminalsplitwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

   public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

   private slots:
	// UI-Slots
	void menuActionLoadConfig();
	void menuActionSaveConfig();

	// Callback-Slots
    void handleConnectionState(bool value, const QString &portname);
	void handleSendData(const QByteArray &data);
	void handleSerialError(const QString &err);
	void handleIncomingData(const QByteArray &chunk);
	void handleResponse(const QByteArray &response);

	void print(const QString& msg);
	void warning(const QString& msg);
	void error(const QString& msg);

   private:
    void writeLog(const QString &level, const QString &msg);

	Ui::MainWindow *m_ui;
	DataTableModel *m_serial_data_model;
	ConfigManager *m_config_manager;
	SerialManager *m_serial_manager;
	SerialWidget* m_serial_widget;
	TerminalSplitWidget *m_terminal_widget;
	ResponderWidget* m_responder_widget;
    LoggerWidget* m_logger_widget;

};

#endif	// MAINWINDOW_H
