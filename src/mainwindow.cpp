#include "mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QStandardPaths>
#include <QVBoxLayout>

#include "./ui_mainwindow.h"
#include "terminalsplitwidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  m_serial_data_model(new DataTableModel(this)),
	  m_config_manager(new ConfigManager(m_serial_data_model)),
	  m_serial_manager(new SerialManager(this)),
	  m_serial_widget(new SerialWidget(m_serial_manager, this)),
	  m_terminal_widget(new TerminalSplitWidget(this)),
	  m_responder_widget(new ResponderWidget(m_serial_data_model, this)),
	  m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

	m_ui->serialLayout->addWidget(m_serial_widget);
	m_ui->terminalLayout->addWidget(m_terminal_widget);
	m_ui->responderLayout->addWidget(m_responder_widget);

	// UI-Events
    connect(m_ui->actionLoadConfig, &QAction::triggered,
            this, &MainWindow::menuActionLoadConfig);
    connect(m_ui->actionSaveConfig, &QAction::triggered,
            this, &MainWindow::menuActionSaveConfig);
    connect(m_ui->actionExit, &QAction::triggered,
            this, &MainWindow::close);
    connect(m_ui->actionAboutQt, &QAction::triggered,
            qApp, &QApplication::aboutQt);

	// m_serial_manager ↔ m_packet_handler ↔ MainWindow
    connect(m_serial_manager, &SerialManager::dataReceived,
            this, &MainWindow::handleIncomingData);
    connect(m_serial_manager, &SerialManager::errorOccurred,
            this, &MainWindow::handleSerialError);

	// Terminal widget
	connect(m_ui->actionTerminal_Vertikal, &QAction::triggered,
			m_terminal_widget, &TerminalSplitWidget::setVerticalSplitView);
    connect(m_terminal_widget, &TerminalSplitWidget::sendData,
            this, &MainWindow::handleSendData);

	// Responder Widget
    connect(m_responder_widget, &ResponderWidget::responseReady,
            this, &MainWindow::handleResponse);

	print("Application Startup");
}

MainWindow::~MainWindow()
{
	print("Application Shutdown");
	delete m_ui;
}

void MainWindow::handleIncomingData(const QByteArray &chunk)
{
	m_terminal_widget->handleReceivedData(chunk);
	QString message("Incoming Data (HEX): '");
    message += QString(chunk.toHex()) + "'";
	print(message);
	m_responder_widget->handleIncomingData(chunk);
}

void MainWindow::handleResponse(const QByteArray &response)
{
	handleSendData(response);
	QString message("Response Data (HEX): '");
	message += QString(response.toHex()) + "'";
	statusBar()->showMessage(message, 1000);
	print(message);
}

void MainWindow::menuActionLoadConfig()
{
	const QString cfgDir =
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	QDir().mkpath(cfgDir);

    QString filter = tr("Response configuration (*.json)");
	QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open configuration ..."),
        cfgDir,
        filter,
        &filter,
        QFileDialog::DontUseNativeDialog
	);

	if (fileName.isEmpty() || fileName.isNull()) {
		return;
	}

	if (!m_config_manager->load(fileName)) {
        QString msg = tr("File '%1' couldn't be loaded.")
						  .arg(QFileInfo(fileName).fileName());
        QMessageBox::warning(this, tr("File open error"),
							 msg);
		warning(msg);
		return;
	}

	m_serial_widget->setConfiguration(m_config_manager);
	m_responder_widget->setHexMode(m_config_manager->defaultHexMode);

	QString msg =
        tr("Configuration file '%1' loaded: successful.").arg(QFileInfo(fileName).fileName());

	statusBar()->showMessage(msg, 3000);

	QString title = QApplication::applicationName();
	title += QString(" - [%1]").arg(QFileInfo(fileName).fileName());
	setWindowTitle(title);

    print(msg);
}

void MainWindow::menuActionSaveConfig()
{
	const QString cfgDir =
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	QDir().mkpath(cfgDir);
	QString defaultFile = cfgDir + QDir::separator() + "config.json";

    QString filter = tr("Response configuration (*.json)");
	QString fileName =
        QFileDialog::getSaveFileName(this, tr("Save configuration ..."),
                                     defaultFile,
                                     filter,
                                     &filter,
                                     QFileDialog::DontUseNativeDialog
		);

	if (fileName.isEmpty() || fileName.isNull()) {
		return;
	}

	if (QFileInfo(fileName).suffix().isEmpty()) {
		fileName += ".json";
	}

	m_serial_widget->getConfiguration(m_config_manager);
	m_config_manager->defaultHexMode = m_responder_widget->getHexMode();

	if (!m_config_manager->save(fileName)) {
        QString msg = tr("File '%1' couldn't be saved..")
						  .arg(QFileInfo(fileName).fileName());
        QMessageBox::warning(this, tr("File save error"),
							 msg);
		warning(msg);
		return;
	}

    QString msg = tr("Configuration '%1' saved successful.")
					  .arg(QFileInfo(fileName).fileName());
	statusBar()->showMessage(msg,
							 3000);

	QString title = QApplication::applicationName();
	title += QString(" - [%1]").arg(QFileInfo(fileName).fileName());
	setWindowTitle(title);
}

void MainWindow::handleSendData(const QByteArray &data)
{
	if (m_serial_manager->isOpen()) {
		m_serial_manager->sendData(data);
		m_terminal_widget->updateSendData(data);
	}
}

void MainWindow::handleSerialError(const QString &err)
{
	QString msg = "Serial Error: " + err;
	QMessageBox::critical(this, "Serial Error", err);
	m_serial_widget->updateLed(SerialWidget::LedState::Error);
	error(msg);
}

void MainWindow::print(const QString &msg)
{
    m_ui->textEditLogger->append(msg);
}

void MainWindow::warning(const QString &msg)
{
    m_ui->textEditLogger->append(msg);
}

void MainWindow::error(const QString &msg)
{
    m_ui->textEditLogger->append(msg);
}
