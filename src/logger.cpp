#include "logger.h"

#include <QMessageBox>

Logger::Logger(QObject *parent) : QObject(parent)
{
    QMessageBox::information(0, "", parent->objectName());
}

void Logger::write(QString line)
{
    // Write Log line
}

void Logger::save()
{
    // Save log file
}
