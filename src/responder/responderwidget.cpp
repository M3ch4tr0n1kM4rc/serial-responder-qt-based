// SPDX-License-Identifier: GPL-3.0-only

#include "responderwidget.h"

#include "ui_responderwidget.h"

ResponderWidget::ResponderWidget(DataTableModel *serial_data_model,
								 QWidget *parent)
	: QWidget(parent),
	  m_ui(new Ui::ResponderWidget),
	  m_serial_data_model(serial_data_model),
	  m_packet_handler(new ResponderPacketHandler(m_serial_data_model, this))
{
	m_ui->setupUi(this);

	m_ui->tvInput->setModel(m_serial_data_model);
	m_ui->tvInput->setColumnWidth(4, 60);

    initDesign();
	populateFormatCombo();

	// UI-Events
	connect(m_ui->btnAddInput, &QPushButton::clicked, this,
			&ResponderWidget::addInputClicked);
	connect(m_ui->btnClearSelection, &QPushButton::clicked, this,
			&ResponderWidget::clearSelectedDataMapping);
	connect(m_ui->btnClearAll, &QPushButton::clicked, this,
			&ResponderWidget::clearAllDataMapping);

	// Packet Handler
	connect(m_packet_handler, &ResponderPacketHandler::packetReceived, this,
			&ResponderWidget::handlePacket);
	connect(m_packet_handler, &ResponderPacketHandler::responseReady,
			this, &ResponderWidget::responseReady);
}

ResponderWidget::~ResponderWidget()
{
	delete m_ui;
}

void ResponderWidget::initDesign()
{
    m_ui->tvInput->setStyleSheet(R"(
        QWidget {
            background-color: #404244;
        }
        /* Base table styling */
        QTableView {
            background-color: #2e2f30;
            color: #ffffff;
            gridline-color: transparent;
            selection-background-color: #3c3d3e;
            selection-color: #ffffff;
        }
        /* Corner widget between headers */
        QTableCornerButton::section {
            background-color: #404244;
            border: none;
        }
        /* Horizontal and vertical headers */
        QHeaderView::section {
            background-color: #404244;
            color: white;
            padding: 4px;
            border: none;
        }
        /* Vertical scrollbar */
        QScrollBar:vertical {
            background-color: #2e2f30;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: #FFCC00;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #FFD633;
        }
        /* Horizontal scrollbar */
        QScrollBar:horizontal {
            background-color: #2e2f30;
            height: 10px;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: #FFCC00;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #FFD633;
        }
        /* Hide scrollbar buttons */
        QScrollBar::add-line, QScrollBar::sub-line {
            background: none;
            border: none;
            height: 0px;
            width: 0px;
        }
    )");

    initHeader();
}

void ResponderWidget::initHeader()
{
    QHeaderView *header = m_ui->tvInput->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}

void ResponderWidget::populateFormatCombo()
{
	QStringList items({"ASCII", "HEX"});
	m_ui->cmbFormatIn->addItems(items);
	m_ui->cmbFormatOut->addItems(items);
}

void ResponderWidget::setHexMode(bool hex_mode)
{
	m_ui->cmbFormatIn->setCurrentIndex(hex_mode ? 0 : 1);
	m_ui->cmbFormatOut->setCurrentIndex(hex_mode ? 0 : 1);
}

void ResponderWidget::handleIncomingData(const QByteArray &chunk)
{
	if (m_ui->checkBoxEnableResponder->isChecked()) {
		m_packet_handler->processData(chunk);
	}
}

void ResponderWidget::handlePacket(const QByteArray &packet)
{
	m_serial_data_model->addEntry(packet, true, packet, false);
}

bool ResponderWidget::getHexMode() const
{
	if (m_ui->cmbFormatIn->currentText() == "HEX" ||
		m_ui->cmbFormatOut->currentText() == "HEX)") {
		return true;
	}
	return false;
}

void ResponderWidget::addInputClicked()
{
	if (m_ui->leDataInput->text().isEmpty()) {
		return;
	}
	bool input_is_hex = m_ui->cmbFormatIn->currentText() == "HEX";
	QByteArray input_data =
		input_is_hex ? QByteArray::fromHex(m_ui->leDataInput->text().toUtf8())
					 : m_ui->leDataInput->text().toUtf8();

	if (m_ui->leDataOutput->text().isEmpty()) {
		m_serial_data_model->addEntry(input_data, input_is_hex, input_data,
									  input_is_hex);
		m_ui->leDataInput->clear();
		return;
	}
	bool output_is_hex = m_ui->cmbFormatOut->currentText() == "HEX";
	QByteArray output_data =
		output_is_hex ? QByteArray::fromHex(m_ui->leDataOutput->text().toUtf8())
					  : m_ui->leDataOutput->text().toUtf8();

	m_serial_data_model->addEntry(input_data, input_is_hex, output_data,
								  output_is_hex);
	m_ui->leDataInput->clear();
	m_ui->leDataOutput->clear();
}

void ResponderWidget::clearSelectedDataMapping()
{
	QModelIndex idx = m_ui->tvInput->currentIndex();
	if (!idx.isValid()) return;

	int row = idx.row();
	m_serial_data_model->remove(row);
}

void ResponderWidget::clearAllDataMapping()
{
	m_serial_data_model->clear();
}
