// SPDX-License-Identifier: GPL-3.0-only

#include "matcher.h"

#include <QVector>

Matcher::Matcher()
{
}

int Matcher::kmpSearch(const QByteArray &text, const QByteArray &pattern)
{
	if (pattern.isEmpty()) {
		return 0;
	}
	const int m = pattern.size();
	const int n = text.size();
	// Build prefix table
	QVector<int> lps(m, 0);
	int len = 0;
	for (int i = 1; i < m;) {
		if (pattern[i] == pattern[len]) {
			lps[i++] = ++len;
		}
		else if (len != 0) {
			len = lps[len - 1];
		}
		else {
			lps[i++] = 0;
		}
	}
	// Search
	int i = 0;
	int j = 0;
	while (i < n) {
		if (text[i] == pattern[j]) {
			i++;
			j++;
			if (j == m) {
				return i - j;
			}
		}
		else {
			if (j != 0) {
				j = lps[j - 1];
			} else {
				i++;
			}
		}
	}
	// No match
	return -1;
}
