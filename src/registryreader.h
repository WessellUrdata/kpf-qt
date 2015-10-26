#ifndef REGISTYREADER_H
#define REGISTYREADER_H

#include <QSettings>

class RegistryReader
{
public:
    RegistryReader();
    ~RegistryReader();
    void open(QString hive);
    void getValue(QString key);
};

#endif // REGISTYREADER_H
