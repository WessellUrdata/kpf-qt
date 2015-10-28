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
