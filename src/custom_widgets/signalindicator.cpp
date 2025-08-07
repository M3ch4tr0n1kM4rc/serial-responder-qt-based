// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: signalindicator.cpp                                │
// │    Description: implementation of SignalIndicator class            │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#include "signalindicator.h"

SignalIndicator::SignalIndicator(const QString &labelText, QWidget *parent)
    : QWidget(parent), m_led_label(new QLabel(labelText)) {

    m_led_label->setFixedSize(30, 25);
    m_led_label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_led_label);

    setState(State::Off);
}

void SignalIndicator::setState(SignalIndicator::State state) {
    QString color;
    switch (state) {
    case State::On:
        color = "limegreen";
        break;
    case State::Off:
        color = "lightgray";
        break;
    case State::Error:
        color = "red";
        break;
    }
    m_led_label->setStyleSheet("font-size:10px;"
                               " border-radius:8px;"
                               " background-color:" + color + ";");
}

void SignalIndicator::setValue(bool value)
{
    setState(value ? State::On : State::Off);
}
