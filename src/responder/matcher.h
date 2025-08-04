#ifndef MATCHER_H
#define MATCHER_H

#include <QByteArray>

class Matcher {
   public:
	static int kmpSearch(const QByteArray &text, const QByteArray &pattern);
	Matcher();
};

#endif	// MATCHER_H
