#include "logger.h"

Logger::Logger(QObject *parent) : QObject(parent)
{
    QDateTime dt(QDateTime::currentDateTime());
    m_LogFilename = QString("kpf_log_%1.log").arg(dt.toString("yyyy-MM-dd_hh-mm-ss"));
    m_LogContents = QString("==== Begin log. Start time: %1 ====\n\n").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
}

void Logger::write(QString line)
{
    // Write Log line
    QDateTime dt(QDateTime::currentDateTime());
    m_LogContents.append(QString("[%1]: %2\n").arg(dt.toString("hh:mm:ss"), line));
    emit appendLine(this->m_LogContents);
}

void Logger::save()
{
    QDateTime dt(QDateTime::currentDateTime());
    m_LogContents.append(QString("\n==== End log. Time: %1 ====\n\n").arg(dt.toString("yyyy-MM-dd hh:mm:ss")));
    // Save log file
    QDir logDir(LOGS_DIR);
    if(!logDir.exists())
        logDir.mkpath(".");

    QFile logFile(LOGS_DIR + m_LogFilename);
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream writer(&logFile);
    writer << m_LogContents;
    writer.flush();
    logFile.close();
}

QString Logger::getCurrentLog()
{
    return this->m_LogContents;
}
