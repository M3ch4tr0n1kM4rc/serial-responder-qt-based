// SPDX-License-Identifier: GPL-3.0-only

#include "terminalinputwidget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

TerminalInputWidget::TerminalInputWidget(QWidget* parent)
	: QWidget(parent)
{
	m_input_field = new QLineEdit(this);
	m_input_format = new QComboBox(this);
	m_input_format->addItems({"ASCII", "HEX"});
	m_send_button = new QPushButton("Send", this);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_input_field);
	layout->addWidget(m_input_format);
	layout->addWidget(m_send_button);
	setLayout(layout);

	connect(m_send_button, &QPushButton::clicked, this,
			&TerminalInputWidget::onSendClicked);
	connect(m_input_format, &QComboBox::currentTextChanged, this,
			&TerminalInputWidget::onFormatChanged);
	connect(m_input_field, &QLineEdit::returnPressed, this,
			&TerminalInputWidget::onReturnPressed);
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
