#ifndef REFS_H
#define REFS_H

#include <QDir>
#include <QString>

/**
 * Reference definitions for constants
 * All constants will be stored in here
 */

#define KOTOR_PATH "/steamapps/common/swkotor"
#define KOTOR2_PATH "/steamapps/common/Knights of the Old Republic II"

#ifdef Q_OS_WIN32
#define KOTOR_EXE "/swkotor.exe"
#define KOTOR2_EXE "/swkotor2.exe"
#define KOTOR2_CLOUD_SAVES "/cloudsaves"
#define DEFAULT_STEAM_PATH "C:\\Program Files (x86)\\Steam"
#define STEAM_EXE "/steam.exe"
#define INI_BACKUP QString("%1/AppData/Local/Temp/kse.ini.bak").arg(QDir::homePath())
#define INI_PATH "kse.ini"
#else
#define KOTOR_EXE "/swkotor"
#define KOTOR2_EXE "/KOTOR2"
#define KOTOR2_SAVES QString("%1/.local/share/aspyr-media/kotor2/saves").arg(QDir::homePath())
#define DEFAULT_STEAM_PATH QString("%1/.local/share/Steam").arg(QDir::homePath())
#define STEAM_EXE "/steam.sh"
#define INI_BACKUP QString("%1/.local/kse.ini.backup").arg(QDir::homePath())
#define INI_PATH QString("kse.ini")
#endif

#ifdef Q_OS_WIN32
#define STEAM_REG_KEY "HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam"
#define GOG_32_REG_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\GOG.com"
#define GOG_64_REG_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GOG.com"
#define KOTOR_CD_REG_KEY ""
#define KOTOR2_CD_REG_KEY ""
#endif

#endif // REFS_H

