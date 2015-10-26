#include "inireader.h"

INIReader::INIReader()
{
}

INIReader::~INIReader()
{
}

void INIReader::open(QString filename)
{
    this->iniFile = filename;
    this->s = new QSettings(this->iniFile, QSettings::IniFormat);
}

QString INIReader::getValue(QString group, QString key)
{
    s->beginGroup(group);
    QString v = s->value(key).toString();
    s->endGroup();
    return v;
}

void INIReader::setValue(QString group, QString key, QString value)
{
    s->beginGroup(group);
    s->setValue(key, value);
    s->endGroup();
}
