#ifndef SIGNALINDICATOR_H
#define SIGNALINDICATOR_H

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

class SignalIndicator : public QWidget {
    Q_OBJECT

public:
    enum class State : uint8_t {
        Off, On, Error
    };

    explicit SignalIndicator(const QString &labelText, QWidget *parent = nullptr);
    void setState(SignalIndicator::State state);

    void setValue(bool value);

private:
    QLabel *m_led_label;
};

#endif // SIGNALINDICATOR_H
