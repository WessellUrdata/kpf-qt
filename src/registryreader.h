#ifndef REGISTYREADER_H
#define REGISTYREADER_H

#include <QSettings>

class RegistryReader
{
public:
    enum Root { HKLM, HKCU };
    RegistryReader();
    ~RegistryReader();
//    void open(Root root, QString hive);
    void open(QString hive);
    QString getValue(QString key);
    bool hasKey(QString key);



private:
    QSettings *s;
};

#endif // REGISTYREADER_H
