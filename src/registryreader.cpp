#include "registryreader.h"

RegistryReader::RegistryReader()
{

}

RegistryReader::~RegistryReader()
{
}

void RegistryReader::open(QString hive)
{
    this->s = new QSettings(hive, QSettings::NativeFormat);
}

QString RegistryReader::getValue(QString key)
{
    return s->value(key, "").toString();
}

bool RegistryReader::hasKey(QString key)
{
    return this->s->contains(key);
}

bool RegistryReader::hasGroup(QString group)
{
    QStringList groups = this->s->childGroups();
    int foundCount = 0;
    for(int i = 0; i < groups.count(); i++)
    {
        if(QString(groups[i]) == group)
            return true;
    }
    return false;
}
