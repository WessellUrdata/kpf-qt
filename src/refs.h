#ifndef REFS_H
#define REFS_H

#include <QDir>
#include <QString>

/**
 * Reference definitions for constants
 * All constants will be stored in here
 */

#define KOTOR_PATH QString("/steamapps/common/swkotor")
#define KOTOR2_PATH QString("/steamapps/common/Knights of the Old Republic II")

#ifdef Q_OS_WIN32
#define KOTOR_EXE QString("/swkotor.exe")
#define KOTOR2_EXE QString("/swkotor2.exe")
#define DEFAULT_STEAM_PATH QString("C:\\Program Files (x86)\\Steam")
#define STEAM_EXE QString("/steam.exe")
#define INI_BACKUP QString("%1/AppData/Local/Temp/kse.ini.bak").arg(QDir::homePath())
#else
#define KOTOR_EXE QString("/swkotor")
#define KOTOR2_EXE QString("/swkotor2")
#define DEFAULT_STEAM_PATH QString(QDir::homePath() + "/.local/share/Steam")
#define STEAM_EXE QString("/steam")
#define INI_BACKUP QString("%1/.local/kse.ini.backup").arg(QDir::homePath())
#endif

#ifdef Q_OS_WIN32
#define STEAM_REG_KEY QString("HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam")
#define GOG_32_REG_KEY QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\GOG.com")
#define GOG_64_REG_KEY QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GOG.com")
#define KOTOR_CD_REG_KEY ""
#define KOTOR2_CD_REG_KEY ""
#endif

#endif // REFS_H

