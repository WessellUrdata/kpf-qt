#ifndef INIREADER_H
#define INIREADER_H

#include <QSettings>

class INIReader
{
public:
    INIReader();
    ~INIReader();
    void open(QString filename);
    QString getValue(QString group, QString key);
    void setValue(QString group, QString key, QString value);

private:
    QString iniFile;
    QSettings *s;
};

#endif // INIREADER_H
