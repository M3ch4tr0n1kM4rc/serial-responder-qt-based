// SPDX-License-Identifier: GPL-3.0-only

#ifndef TERMINALINPUTWIDGET_H
#define TERMINALINPUTWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QComboBox;

class TerminalInputWidget : public QWidget {
	Q_OBJECT

   public:
	explicit TerminalInputWidget(QWidget* parent = nullptr);

   signals:
	void sendData(const QByteArray& data);

   private slots:
	void onSendClicked();
	void onFormatChanged(const QString& format);
	void onReturnPressed();

   private:
	QLineEdit* m_input_field;
	QComboBox* m_input_format;
	QPushButton* m_send_button;
    QByteArray parseHexInput(const QString& raw_text);
};

#endif	// TERMINALINPUTWIDGET_H
