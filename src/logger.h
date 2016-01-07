#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "refs.h"

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    void write(QString line);
    void save();
    QString getCurrentLog();

signals:
    void appendLine(QString text);

private:
    QString m_LogFilename;
    QString m_LogContents;

    QString getLogsDir();
};

#endif // LOGGER_H
