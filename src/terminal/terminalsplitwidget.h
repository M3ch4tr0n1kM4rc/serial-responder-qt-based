// SPDX-License-Identifier: GPL-3.0-only

#ifndef TERMINALSPLITWIDGET_H
#define TERMINALSPLITWIDGET_H

#include <QGroupBox>
#include <QSplitter>
#include <QWidget>

class TerminalViewWidget;
class TerminalInputWidget;

class TerminalSplitWidget : public QWidget {
	Q_OBJECT

   public:
	explicit TerminalSplitWidget(QWidget* parent = nullptr);

   signals:
	void sendData(const QByteArray& data);
    void setDTR(bool value);
    void setRTS(bool value);

   public slots:
	void updateSendData(const QByteArray& data);
	void handleReceivedData(const QByteArray& data);
	void setVerticalSplitView(bool enable);

   private slots:
	void handleSendData(const QByteArray& data);

   private:
	QGroupBox* m_receive_group;
	QGroupBox* m_sent_group;
	QGroupBox* m_input_group;
	QSplitter* m_splitter;
	TerminalViewWidget* m_receiver_terminal;
	TerminalViewWidget* m_sender_terminal;
	TerminalInputWidget* m_sender_input;
};

#endif	// TERMINALSPLITWIDGET_H
