// SPDX-License-Identifier: GPL-3.0-only

#include "terminalinputwidget.h"

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

TerminalInputWidget::TerminalInputWidget(QWidget* parent)
	: QWidget(parent)
    , m_input_field(new QLineEdit(this))
    , m_input_format(new QComboBox(this))
    , m_send_button(new QPushButton("Send", this))
    , m_rts_button(new QPushButton("RTS", this))
    , m_dtr_button(new QPushButton("DTR", this))
{
    m_rts_button->setCheckable(true);
    m_dtr_button->setCheckable(true);

    m_input_format->addItems({"ASCII", "HEX"});

    QHBoxLayout* input_layout = new QHBoxLayout;
    input_layout->addWidget(m_input_field);
    input_layout->addWidget(m_input_format);
    QVBoxLayout* led_input_layout = new QVBoxLayout;
    led_input_layout->addLayout(input_layout);

    QHBoxLayout* dtr_rts_layout = new QHBoxLayout;
    dtr_rts_layout->addWidget(m_rts_button);
    dtr_rts_layout->addWidget(m_dtr_button);
    QVBoxLayout* button_layout = new QVBoxLayout;
    button_layout->addLayout(dtr_rts_layout);
    button_layout->addWidget(m_send_button);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addLayout(led_input_layout);
    layout->addLayout(button_layout);
	setLayout(layout);

	connect(m_send_button, &QPushButton::clicked, this,
			&TerminalInputWidget::onSendClicked);
	connect(m_input_format, &QComboBox::currentTextChanged, this,
			&TerminalInputWidget::onFormatChanged);
	connect(m_input_field, &QLineEdit::returnPressed, this,
			&TerminalInputWidget::onReturnPressed);
    connect(m_dtr_button, &QPushButton::clicked, this, &TerminalInputWidget::setDTR);
    connect(m_rts_button, &QPushButton::clicked, this, &TerminalInputWidget::setRTS);
}

void TerminalInputWidget::onSendClicked()
{
	QString text = m_input_field->text();
	if (text.isEmpty()) return;

	bool isHex = m_input_format->currentText() == "HEX";

	QByteArray data;
	if (isHex) {
		data = parseHexInput(text);
		if (data.isEmpty()) {
			return;
		}
	}
	else {
		data = text.toUtf8();
	}

	emit sendData(data);
	m_input_field->clear();
}

QByteArray TerminalInputWidget::parseHexInput(const QString& raw_text)
{
	QString cleaned =
        raw_text.toUpper().remove(QRegularExpression("[^0-9A-F\\s]"));

	QStringList hexGroups;
    QString temp = cleaned.simplified();

	if (temp.contains(' ')) {
		hexGroups = temp.split(' ', Qt::SkipEmptyParts);
	}
	else {
		for (int i = 0; i < temp.length(); i += 2) {
			if (i + 2 <= temp.length())
				hexGroups << temp.mid(i, 2);
			else
                return QByteArray();
		}
	}

	for (const QString& group : hexGroups) {
        if (group.length() != 2) return QByteArray();
	}

	QByteArray result;
	for (const QString& group : hexGroups) {
		result.append(static_cast<char>(group.toUInt(nullptr, 16)));
	}

	return result;
}

void TerminalInputWidget::onFormatChanged(const QString& format)
{
	if (format == "HEX") {
		m_input_field->setToolTip(
			"Enter hex bytes using 2-digit pairs. Examples:\n"
			"→ AB02CD\n"
			"→ AB 02 CD\n"
			"Only digits 0–9 and letters A–F are allowed.");
	}
	else {
		m_input_field->setToolTip(
			"Enter ASCII text normally. Example:\n→ Hello World!");
	}
}

void TerminalInputWidget::onReturnPressed()
{
	onSendClicked();
}
