// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: terminalinputwidget.h                              │
// │    Description: definition of QFrame class                         │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef TERMINALINPUTWIDGET_H
#define TERMINALINPUTWIDGET_H

#include <QWidget>

class QFrame;
class QLineEdit;
class QPushButton;
class QComboBox;

class TerminalInputWidget : public QWidget {
	Q_OBJECT

   public:
	explicit TerminalInputWidget(QWidget* parent = nullptr);

   public slots:

   signals:
	void sendData(const QByteArray& data);
    void setDTR(bool value);
    void setRTS(bool value);

   private slots:
	void onSendClicked();
	void onFormatChanged(const QString& format);
	void onReturnPressed();

   private:
	QLineEdit* m_input_field;
	QComboBox* m_input_format;
	QPushButton* m_send_button;
    QPushButton* m_rts_button;
    QPushButton* m_dtr_button;

    QByteArray parseHexInput(const QString& raw_text);
    void toggleFrame(QFrame *frame, bool state);
};

#endif	// TERMINALINPUTWIDGET_H
