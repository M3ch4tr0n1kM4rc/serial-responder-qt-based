#include "terminalviewwidget.h"

#include <QCheckBox>
#include <QDebug>
#include <QFontMetrics>
#include <QHeaderView>
#include <QLabel>
#include <QScrollBar>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>

#include "rowlinedelegate.h"

TerminalViewWidget::TerminalViewWidget(bool top_buttons, QWidget* parent)
	: QWidget(parent),
	  m_table_view(new QTableView(this)),
	  m_terminal_data_model(new TerminalTableModel(this)),
	  m_toolbar(new QToolBar(this)),
	  m_clear_button(new QToolButton(this)),
	  m_ascii_enable_tool_button(new QToolButton(this)),
	  m_hex_enable_tool_button(new QToolButton(this)),
	  m_column_count_spin_box(new QSpinBox(this)),
	  m_newline_check_box(new QCheckBox("Break on newline", this)),
	  m_display_mode("Both")
{
	initializeTable();
	initializeWidgets();

	QVBoxLayout* layout = new QVBoxLayout(this);
	if (top_buttons) {
		layout->addWidget(m_toolbar);
		layout->addWidget(m_table_view);
	}
	else {
		layout->addWidget(m_table_view);
		layout->addWidget(m_toolbar);
	}
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);

	updateDisplayFormat();
}

void TerminalViewWidget::initializeTable()
{
	m_table_view->setItemDelegate(new RowLineDelegate());

	m_table_view->setModel(m_terminal_data_model);

	m_table_view->horizontalHeader()->setStretchLastSection(true);
	m_table_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

	m_table_view->verticalHeader()->setVisible(true);
	m_table_view->verticalHeader()->setDefaultAlignment(Qt::AlignRight |
														Qt::AlignVCenter);
	m_table_view->verticalHeader()->setMinimumWidth(80);
	m_table_view->verticalHeader()->setMaximumWidth(80);

	m_table_view->setShowGrid(false);
	m_table_view->setAlternatingRowColors(
		false);	 // Optional: keep a uniform look

	m_table_view->setStyleSheet(R"(
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
}

void TerminalViewWidget::initializeWidgets()
{
	m_clear_button->setText("Clear");
	m_clear_button->setStyleSheet(R"(
		QToolButton {
			background-color: lightGray;
			border: 1px solid gray;
			padding: 4px 8px;
		}
	)");

	auto setupToggle = [](QToolButton* button, const QString& text) {
		button->setText(text);
		button->setCheckable(true);
		button->setChecked(true);
	};

	setupToggle(m_ascii_enable_tool_button, "ASCII View");
	m_ascii_enable_tool_button->setStyleSheet(R"(
		QToolButton {
			background-color: lightGray;
			border: 1px solid gray;
			padding: 4px 8px;
		}
		QToolButton:checked {
			background-color: #6aa4a4;
			color: white;
		}
	)");
	setupToggle(m_hex_enable_tool_button, "HEX View");
	m_hex_enable_tool_button->setStyleSheet(R"(
		QToolButton {
			background-color: lightGray;
			border: 1px solid gray;
			padding: 4px 8px;
		}
		QToolButton:checked {
			background-color: #7878b2;
			color: white;
		}
	)");

	m_toolbar->addWidget(m_clear_button);
	m_toolbar->addSeparator();
	m_toolbar->addWidget(m_ascii_enable_tool_button);
	m_toolbar->addWidget(m_hex_enable_tool_button);

	m_column_count_spin_box->setRange(1, 100);
	m_column_count_spin_box->setValue(40);

	m_toolbar->addSeparator();
	m_toolbar->addWidget(new QLabel("Bytes per row:", this));
	m_toolbar->addWidget(m_column_count_spin_box);
	m_toolbar->addSeparator();
	m_toolbar->addWidget(m_newline_check_box);

	connect(m_clear_button, &QToolButton::clicked, this,
			&TerminalViewWidget::clearTable);
	connect(m_ascii_enable_tool_button, &QToolButton::toggled, this,
			&TerminalViewWidget::updateDisplayFormat);
	connect(m_hex_enable_tool_button, &QToolButton::toggled, this,
			&TerminalViewWidget::updateDisplayFormat);
	connect(m_column_count_spin_box,
			QOverload<int>::of(&QSpinBox::valueChanged), this,
			&TerminalViewWidget::updateColumnCount);
	connect(m_newline_check_box, &QCheckBox::toggled, this,
			&TerminalViewWidget::updateNewlineBehavior);
}

void TerminalViewWidget::updateDisplayFormat()
{
	bool ascii = m_ascii_enable_tool_button->isChecked();
	bool hex = m_hex_enable_tool_button->isChecked();

	m_display_mode = (ascii && hex) ? "Both"
					 : (ascii)		? "ASCII"
					 : (hex)		? "HEX"
									: "None";

	applyFormatFilter(m_display_mode);
	applyCellWidth(m_display_mode);
}

void TerminalViewWidget::updateColumnCount(int count)
{
	m_terminal_data_model->setMaxColumns(count);
	updateDisplayFormat();
}

void TerminalViewWidget::updateNewlineBehavior(bool enabled)
{
	m_terminal_data_model->setWrapOnNewline(enabled);
	updateDisplayFormat();
}

void TerminalViewWidget::clearTable()
{
	m_terminal_data_model->clearData();
}

void TerminalViewWidget::appendData(const QByteArray& data,
                                    const QDateTime& timestamp, bool is_received)
{
    m_terminal_data_model->appendEntry(data, timestamp, is_received);
}

void TerminalViewWidget::applyFormatFilter(const QString& mode)
{
	int totalRows = m_terminal_data_model->rowCount();
	for (int row = 0; row < totalRows; ++row) {
		bool show = false;
		if (mode == "ASCII" && row % 2 == 0) show = true;
		if (mode == "HEX" && row % 2 == 1) show = true;
		if (mode == "Both") show = true;

		m_table_view->setRowHidden(row, !show);
	}
}

void TerminalViewWidget::applyCellWidth(const QString& mode)
{
	int charWidth = (mode == "HEX") ? 1 : 1;
	int pixelWidth = QFontMetrics(m_table_view->font())
						 .horizontalAdvance(QString(charWidth, 'W')) +
					 12;

	m_table_view->horizontalHeader()->setDefaultSectionSize(pixelWidth);
	m_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
