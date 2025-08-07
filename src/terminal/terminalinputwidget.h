// SPDX-License-Identifier: GPL-3.0-only

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
    void handleCTS(bool state);
    void handleDSR(bool state);
    void handleRI(bool state);
    void handleDCD(bool state);

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
    QFrame* m_cts_led_frame;
    QFrame* m_dsr_led_frame;
    QFrame* m_ri_led_frame;
    QFrame* m_dcd_led_frame;
    QByteArray parseHexInput(const QString& raw_text);
    void toggleFrame(QFrame *frame, bool state);
};

#endif	// TERMINALINPUTWIDGET_H
