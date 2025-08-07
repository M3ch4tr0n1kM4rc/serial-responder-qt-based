// SPDX-License-Identifier: GPL-3.0-only
//
// ┌────────────────────────────────────────────────────────────────────┐
// │           File: matcher.h                                          │
// │    Description: definition of Matcher class                        │
// │        Project: serial-responder-qt-based                          │
// │         Author: M3ch4tr0n1kM4rc                                    │
// │        Created: 2025-08-08                                         │
// │  Last Modified: 2025-08-08                                         │
// │        License: GNU GPL v3.0-only                                  │
// └────────────────────────────────────────────────────────────────────┘

#ifndef MATCHER_H
#define MATCHER_H

#include <QByteArray>

class Matcher {
   public:
	static int kmpSearch(const QByteArray &text, const QByteArray &pattern);
	Matcher();
};

#endif	// MATCHER_H
