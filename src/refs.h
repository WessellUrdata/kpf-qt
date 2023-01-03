#ifndef REFS_H
#define REFS_H

#include <QDir>
#include <QString>

/**
 * Reference definitions for constants
 * All constants will be stored in here
 */

#define APP_VERSION "1.2-linux"

#define KOTOR_PATH "/steamapps/common/swkotor"
#define KOTOR2_PATH "/steamapps/common/Knights of the Old Republic II"

//#define SUPPORT // uncomment for branch: kse-337a-support

#define KOTOR_EXE "/swkotor.exe"
#define KOTOR2_EXE "/KOTOR2"
#define KOTOR2_SAVES QString("%1/.local/share/aspyr-media/kotor2/saves").arg(QDir::homePath())
#define DEFAULT_STEAM_PATH QString("%1/.local/share/Steam").arg(QDir::homePath())
#define STEAM_EXE "/steam.sh"
#define INI_BACKUP QString("%1/.local/kse/kse.ini.backup").arg(QDir::homePath())
#define INI_PATH QString("%1/kse/kse.ini").arg(QDir::homePath())
#define LOGS_DIR QString("%1/kse/logs/").arg(QDir::homePath())

#endif // REFS_H

