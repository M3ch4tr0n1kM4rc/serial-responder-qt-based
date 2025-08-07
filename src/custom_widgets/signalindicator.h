// SPDX-License-Identifier: GPL-3.0-only
//
// ┌──────────────────────────────────────────────────────────┐
// │           File: signalindicator.h                        │
// │    Description: [Beschreibung hier einfügen]             │
// │        Project: serial-responder-qt-based                │
// │         Author: M3ch4tr0n1kM4rc                          │
// │        Created: 2025-08-08                               │
// │  Last Modified: 2025-08-08                               │
// │        License: GNU GPL v3.0-only                        │
// └──────────────────────────────────────────────────────────┘

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
