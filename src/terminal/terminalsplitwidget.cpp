// SPDX-License-Identifier: GPL-3.0-only

#include "terminalsplitwidget.h"

#include <QDateTime>
#include <QVBoxLayout>

#include "terminalinputwidget.h"
#include "terminalviewwidget.h"

TerminalSplitWidget::TerminalSplitWidget(QWidget* parent)
	: QWidget(parent),
	  m_receive_group(new QGroupBox("Received data:", this)),
	  m_sent_group(new QGroupBox("Send data:", this)),
	  m_input_group(new QGroupBox("User input:", this)),
	  m_splitter(new QSplitter(Qt::Vertical, this)),
	  m_receiver_terminal(new TerminalViewWidget(true, this)),
	  m_sender_terminal(new TerminalViewWidget(false, this)),
	  m_sender_input(new TerminalInputWidget(this))
{
	QVBoxLayout* rec_layout = new QVBoxLayout(m_receive_group);
	rec_layout->addWidget(m_receiver_terminal);
	m_receive_group->setLayout(rec_layout);

	QVBoxLayout* sen_layout = new QVBoxLayout(m_sent_group);
	sen_layout->addWidget(m_sender_terminal);
	m_sent_group->setLayout(sen_layout);

	QVBoxLayout* inp_layout = new QVBoxLayout(m_input_group);
	inp_layout->addWidget(m_sender_input);
	m_input_group->setLayout(inp_layout);

	m_splitter->addWidget(m_receive_group);
	m_splitter->addWidget(m_sent_group);

	m_splitter->setStretchFactor(0, 1);
	m_splitter->setStretchFactor(1, 1);

    connect(m_sender_input, &TerminalInputWidget::sendData,
            this, &TerminalSplitWidget::sendData);
    connect(m_sender_input, &TerminalInputWidget::setRTS,
            this, &TerminalSplitWidget::setRTS);
    connect(m_sender_input, &TerminalInputWidget::setDTR,
            this, &TerminalSplitWidget::setDTR);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_splitter);
	layout->addWidget(m_input_group);
	layout->setStretch(0, 1);  // splitter stretches
	layout->setStretch(1, 0);  // input stays fixed
	setLayout(layout);
}

void TerminalSplitWidget::updateSendData(const QByteArray& data) {
	QDateTime now = QDateTime::currentDateTime();
	m_sender_terminal->appendData(data, now, false);
}

void TerminalSplitWidget::handleSendData(const QByteArray& data)
{
	emit sendData(data);
}

void TerminalSplitWidget::handleReceivedData(const QByteArray& data)
{
	QDateTime now = QDateTime::currentDateTime();
	m_receiver_terminal->appendData(data, now, true);
}

void TerminalSplitWidget::setVerticalSplitView(bool enable)
{
	if (!m_splitter) return;

	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(this->layout());
	if (!layout) return;

	// Remove old splitter
	layout->removeWidget(m_splitter);
	m_splitter->deleteLater();

	// Create new splitter
	m_splitter = new QSplitter(enable ? Qt::Horizontal : Qt::Vertical, this);

	m_splitter->addWidget(m_receive_group);
	m_splitter->addWidget(m_sent_group);

	m_splitter->setStretchFactor(0, 1);
	m_splitter->setStretchFactor(1, 1);

	layout->addWidget(m_splitter);
	layout->addWidget(m_input_group);
	layout->setStretch(0, 1);
	layout->setStretch(1, 0);
	setLayout(layout);
}
