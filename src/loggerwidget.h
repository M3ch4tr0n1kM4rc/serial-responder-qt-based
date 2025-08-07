#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class LoggerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);
    void addLogEntry(const QString &logLine);
    void addLogEntry(QString timestamp, QString level, QString message, QString source, QString additional);

private slots:
    void handleExportClicked();

private:
    QPushButton *m_export_button;
    QTableWidget *m_table_widget;
    QString getEmojiForLevel(const QString &level) const;
    QString getLevelForEmoji(const QString &emoji) const;
};

#endif // LOGGERWIDGET_H
