#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggerconsole.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // If support, set support in title
#ifdef SUPPORT
    this->setWindowTitle("KPF: KSE PathFinder (Support Build)");
#endif

    this->logger = new Logger(this);
    this->logger->write("Initializing main window");

    // Center window on screen
    QRect pos = frameGeometry();
    pos.moveCenter(QDesktopWidget().availableGeometry().center());
    move(pos.topLeft());

    // setup default values
    this->steamPath = "undef";
    this->exported = false;
    this->changed = false;

    // OS specific gaming stuff
    // certain ones are not on certain OSs
#ifdef Q_OS_UNIX
    ui->leKotor->setEnabled(false);
    ui->leKotor->setText("KotOR is not available on this platform!");
    ui->bK1Browse->setEnabled(false);
    k1onPlat = false;
#endif

    this->logger->write("Creating menu strip keyboard shortcuts");
    // Keyboard shortcuts
    ui->menuExit->setShortcut(Qt::CTRL | Qt::Key_Q);
    ui->menuAbout->setShortcut(Qt::Key_F1);
    ui->menuOpen->setShortcut(Qt::CTRL | Qt::Key_O);
    ui->menuLogger->setShortcut(Qt::CTRL | Qt::Key_C);
    ui->menuLogs->setShortcut(Qt::CTRL | Qt::Key_L);
    ui->menuDelete->setShortcut(Qt::CTRL | Qt::Key_D);

    this->logger->write("Registering widget event listeners");
    this->connect(ui->bQuit, SIGNAL(clicked(bool)), this, SLOT(onQuitClicked()));
    this->connect(ui->bK1Browse, SIGNAL(clicked(bool)), this, SLOT(onK1BrowseClicked()));
    this->connect(ui->bK2Browse, SIGNAL(clicked(bool)), this, SLOT(onK2BrowseClicked()));
    this->connect(ui->bExportINI, SIGNAL(clicked(bool)), this, SLOT(onINIExportClicked()));
    this->connect(ui->bRescan, SIGNAL(clicked(bool)), this, SLOT(onRescanClicked()));
    this->connect(ui->bUndoExport, SIGNAL(clicked(bool)), this, SLOT(onUndoClicked()));

    this->connect(ui->leKotor, SIGNAL(textEdited(QString)), this, SLOT(onTextChanged(QString)));
    this->connect(ui->leKotor2, SIGNAL(textEdited(QString)), this, SLOT(onTextChanged(QString)));

    this->connect(ui->menuExit, SIGNAL(triggered(bool)), this, SLOT(onMenuItemExitClicked()));
    this->connect(ui->menuAbout, SIGNAL(triggered(bool)), this, SLOT(onMenuItemAboutClicked()));
    this->connect(ui->menuLogger, SIGNAL(triggered(bool)), this, SLOT(onMenuItemLoggerClicked()));
    this->connect(ui->menuLogs, SIGNAL(triggered(bool)), this, SLOT(onMenuItemLogsClicked()));

#ifdef SUPPORT
    this->connect(ui->menuDelete, SIGNAL(triggered(bool)), this, SLOT(onTempMenuItemClicked()));
#else
    this->connect(ui->menuDelete, SIGNAL(triggered(bool)), this, SLOT(onMenuItemDeleteClicked()));
#endif
    
    this->connect(ui->menuOpen, SIGNAL(triggered(bool)), this, SLOT(onMenuItemOpenClicked()));

    this->logger->write("Initializing automated path detection");
    detectPaths(false);

    // Show something happened. Let them know to export shit :p
    // Finally fixed this piece of shit check :)
    QFile ini(INI_PATH);
    if(!ini.exists())
    {
        MsgBox msg(this, "Paths Found",
            "KotOR and/or KotOR 2 were automatically detected on your system. All you need to do is generate the INI for KSE. Simply click \"Export to INI\" and you're all set to continue running KSE",
            MsgBox::Ok);

        if(((ui->leKotor->text() != "") && ui->leKotor->isEnabled() == true)
                || (ui->leKotor2->text() != ""))
        {
            msg.exec();
        }
    }
}

void MainWindow::onTempMenuItemClicked()
{
    MsgBox msg(this, "Temprarily Disabled", QString("Thanks for checking out this feature to KPF. Currently, this option is disabled for support reasons. This option will ")
               + "be re-enabled once KSE is updated. But for now, it will stay this way. Thanks for trying out KPF Qt, and I hope it works well for you. :)", MsgBox::Ok);
    msg.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged(QString)
{
    this->changed = true;
}

// subclassing QMainWindow for closing event
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->logger->write("Application close event fired");
    if(changed)
    {
        MsgBox msg(this, "Unsaved Changes",
                   "Changes have been made in KPF. Are you sure you want to close without saving changes",
                   MsgBox::YesNo, "question");
        if(msg.exec() == MsgBox::No)
            event->ignore();
    }

    this->logger->write("Cleaning up any potential INI backups");
    // We're not ignoring the event now, let's destroy our backup file!
    QFile file(INI_BACKUP);
    if(file.exists())
        file.remove();

    this->logger->write("Saving log file");
    this->logger->write("Terminating KPF");
    this->logger->save();
}

void MainWindow::onQuitClicked()
{
    this->close();
}

void MainWindow::onK1BrowseClicked()
{
    if(this->browse(ui->leKotor->text(), KOTOR_EXE))
    {
        this->logger->write(QString("KotOR 1 found from manual selection: %1/%2").arg(ui->leKotor->text(), KOTOR_EXE));
        ui->leKotor->setText(tempPath);
    }
    else
    {
        MsgBox msg(this, "Invalid Path", "The kotor game executable could not be found! Is this the game's root directory?", MsgBox::Ok, MsgBox::IconError);
        msg.exec();
    }
}

void MainWindow::onK2BrowseClicked()
{
    if(this->browse(ui->leKotor2->text(), KOTOR2_EXE))
    {
        this->logger->write(QString("KotOR 2 found from manual selection: %1/%2").arg(ui->leKotor2->text(), KOTOR2_EXE));
        ui->leKotor2->setText(tempPath);
    }
    else
    {
        MsgBox msg(this, "Invalid Path", "The kotor2 game executable could not be found! Is this the game's root directory?", MsgBox::Ok, MsgBox::IconError);
        msg.exec();
    }
}

bool MainWindow::browse(QString location, const char *exe)
{
    QFileDialog dlg;
    dlg.setWindowTitle(QString("Find Game Executable: %1").arg(QString(exe).replace("/", "")));
    dlg.setNameFilter(QString(exe).replace("/", ""));
    if(location != "")
        dlg.setDirectory(location);
    int result = dlg.exec();
    QString dir;
    if(result)
    {
        dir = dlg.selectedFiles()[0];
        QFile file(dir);
        if(file.exists())
        {
            dir.replace(QString(exe), "/");
#ifdef Q_OS_WIN32
            dir.replace("/", "\\");
#endif
            dir.truncate(dir.length() - 1); // attempt to remove trailing slash
            tempPath = QString(dir);
            return true;
        }
    }

    return false;
}

void MainWindow::onINIExportClicked()
{
    this->logger->write("Gathering required information for INI generation");
    // Let's make sure to back up the ini file shall we?
    this->logger->write("Attempting to create backup of current INI");
    QFile ini(INI_PATH);
    if(ini.exists())
        ini.copy(INI_BACKUP);

    try
    {
        INIReader reader;
        reader.open(INI_PATH);
        this->logger->write("Validating given path for KotOR 1");
        QFile *kf = new QFile(QString("%0%1").arg(ui->leKotor->text(), KOTOR_EXE));
        if(kf->exists())
        {
#ifdef Q_OS_UNIX
            reader.setValue("Installed", "K1_Installed", "0");
            reader.setValue("Paths", "K1_Path", "undef");
#else
            this->logger->write("Path validated. Applying to INI config");
            reader.setValue("Installed", "K1_Installed", "1");
            reader.setValue("Paths", "K1_Path", ui->leKotor->text());

            // this is to add vstore support (location not confirmed. give me a mo to confirm)
            this->logger->write("Applying secondary KotOR save info");
            QString kp;
            QDir s(ui->leKotor->text() + "\\saves");
            if (s.exists())
                kp = ui->leKotor->text() + "\\saves";
            else
            {
                QString pf;
                if (is32Bit())
                    pf = "Program Files";
                else
                    pf = "Program Files (x86)";

                kp = QString("%1%2\\LucasArts\\SWKotOR\\saves").arg(VSTORE, pf);
            }
#endif
        }
        else
        {
            this->logger->write("KotOR 1 not found. Applying default values");
            reader.setValue("Installed", "K1_Installed", "0");
            reader.setValue("Paths", "K1_Path", "undef");
        }

        this->logger->write("Validating given path for KotOR 2");
        kf = new QFile(QString("%0%1").arg(ui->leKotor2->text(), KOTOR2_EXE));
        if(kf->exists())
        {
            this->logger->write("Path validated. Applying to INI config");
            reader.setValue("Installed", "K2_Installed", "1");
            reader.setValue("Paths", "K2_Path", ui->leKotor2->text());

            this->logger->write("Applying secondary KotOR 2 save info");
            // kotor2 really needs special entry for it's regular saves? (Yes, for VStore support, just like KotOR)
            QString k2p;
#ifdef Q_OS_WIN32
//            k2p = ui->leKotor2->text() + "\\saves";
            QDir s(ui->leKotor2->text() + "\\saves");
            if(s.exists())
                k2p = ui->leKotor2->text() + "\\saves";
            else
            {
                QString pf;
                if (is32Bit())
                    pf = "Program Files";
                else
                    pf = "Program Files (x86)";

                k2p = QString("%1%2\\LucasArts\\SWKotOR2\\saves").arg(VSTORE, pf);
            }
#else
            k2p = KOTOR2_SAVES;
#endif
            reader.setValue("Paths", "K2_SavePath", k2p.replace("/", "\\"));
        }
        else
        {
            this->logger->write("KotOR 2 not found. Applying default values");
            reader.setValue("Installed", "K2_Installed", "0");
            reader.setValue("Paths", "K2_Path", "undef");

            reader.setValue("Paths", "K2_SavePath", "undef");
        }

        this->logger->write("Attempting to determine cloud saves for KotOR 2");
        // KotOR 2 cloud saves check
        QDir csdir(ui->leKotor2->text() + "/cloudsaves/");
        QStringList dirs = csdir.entryList();
        reader.setValue("Options", "Use_K2_Cloud", "0");
        reader.setValue("Paths", "K2_SavePathCloud", "undef");
        this->logger->write("Scanning directory for steam game/user ID");
        for(int i = 0; i < dirs.count(); i++)
        {
            QRegularExpression regex("([0-9]+)");
            QRegularExpressionMatch match = regex.match(dirs[i]);
            if(match.hasMatch())
            {
                this->logger->write("Match found. Applying to INI config");
                reader.setValue("Options", "Use_K2_Cloud", "1");
#ifdef Q_OS_WIN32
                reader.setValue("Paths", "K2_SavePathCloud", QString(csdir.absolutePath() + "\\" + dirs[i]).replace("/", "\\"));
#else
                reader.setValue("Paths", "K2_SavePathCloud", QString(csdir.absolutePath() + "/" + dirs[i]));
#endif
                break;
            }
            else
            {
                this->logger->write("Cloud saves cannot be found. Applying default values");
                reader.setValue("Options", "Use_K2_Cloud", "0");
                reader.setValue("Paths", "K2_SavePathCloud", "undef");
            }
        }

        // Last couble of values. We'll leave as null for now
        reader.setValue("Installed", "TJM_Installed", "0");
        reader.setValue("Paths", "TJM_Path", "undef");

        this->logger->write("Generating INI");
        reader.setValue("Paths", "Steam_Path", steamPath);
        MsgBox msg(this, "INI Export", QString("Contents successfully exported to %1").arg(INI_PATH), MsgBox::Ok);
        msg.exec();

        this->exported = true;
        this->changed = false;
    }
    catch(...)
    {
        MsgBox msg(this, "INI Export Error", "Error exporting values to kse.ini. Please try again later", MsgBox::Ok);
        msg.exec();
    }
}

void MainWindow::onRescanClicked()
{
    this->logger->write("Initializing rescan of paths");
    detectPaths(true);
    MsgBox msg(this, "Rescan Complete", QString(
                   "Rescaning has completed. You should now see your paths in the main window. ")
                   + "If you do not, they could not be obtained automatically. Please add them manually", MsgBox::Ok);
    msg.exec();
}

void MainWindow::loadINI()
{
    this->logger->write("Checking for existance of INI");
    QFile iniFile(INI_PATH);
    if(iniFile.exists())
    {
        this->logger->write("INI found, reading from INI");
        INIReader reader;
        reader.open(INI_PATH);
        QString k1path = reader.getValue("Paths", "K1_Path");
        QString k2path = reader.getValue("Paths", "K2_Path");
        steamPath = reader.getValue("Paths", "Steam_Path");

        if(k1path != "undef")
        {
            this->logger->write("Applying found path of KotOR 1 to UI");
            QFile file(k1path + KOTOR_EXE);
            if(file.exists())
                ui->leKotor->setText(k1path);
        }

        if(k2path != "undef")
        {
            this->logger->write("Applying found path of KotOR 2 to UI");
            QFile file(k2path + KOTOR2_EXE);
            if(file.exists())
                ui->leKotor2->setText(k2path);
        }
    }
}

void MainWindow::detectPaths(bool rescan)
{
    QFile ini(INI_PATH);
    if(ini.exists() && !rescan)
        loadINI();
    else
    {
        this->logger->write("Initializing registry scanner");
        this->changed = true;
#ifdef Q_OS_WIN32
        // We'll look for CD entries first.
        this->cdShit("k1", KOTOR_CD_REG_KEY_32, KOTOR_CD_REG_KEY_64); // KotOR 1
        this->cdShit("k2", KOTOR2_CD_REG_KEY_32, KOTOR2_CD_REG_KEY_64); // KotOR 2

        // This hole block will be overlooked if GOG stuff is not found.
        // Otherwise it'll look. But isn't fruitful if they're not from GOG
        // GOG calles on Steam too. Same rule applies
        RegistryReader reader;
        reader.open(GOG_64_REG_KEY);
        if(reader.hasGroup("Games"))
            this->gogShit(GOG_64_REG_KEY);
        else
        {
            reader.open(GOG_32_REG_KEY);
            if(reader.hasGroup("Games"))
                this->gogShit(GOG_32_REG_KEY);
            else
                this->steamShit();
        }
#else
        this->logger->write("Registry not supported on current OS. Attempting scan of config files instead");
        this->steamShit();
#endif
    }
}

// <OLD_COMMENT>I'll clean this method up later :p</OLD_COMMENT>
// Method is cleaned up a bit. I'll rename later
void MainWindow::steamShit()
{
    this->logger->write("Checking for existance of Steam");
    QFile file(QString("%1%2").arg(DEFAULT_STEAM_PATH, STEAM_EXE));
    if(file.exists())
    {
        this->logger->write("Reading Steam config for possible paths");
        steamPath = DEFAULT_STEAM_PATH;
    }
    else
    {
        this->logger->write("Steam not found, getting Steam location from Registry");
#ifdef Q_OS_WIN32
        RegistryReader reader;
        reader.open(STEAM_REG_KEY);

        if(reader.getValue("SteamPath") != "")
            steamPath = reader.getValue("SteamPath").replace("/", "\\");
#else
        MsgBox msg(this, "Error", "Default Steam Path was not found, closing. (This will be fixed later...", MsgBox::Ok);
        msg.exec();
#endif
    }

    QFile steamExe(steamPath + STEAM_EXE);
    if(steamExe.exists())
    {
        this->logger->write("Steam found. Loading config.vdf");
        QFile config(steamPath + "/config/config.vdf");
        if(config.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->logger->write("Scanning config.vdf for library information");
            QTextStream in(&config);
            bool k = false, kk = false, stop = false; // might remove...
            while((!in.atEnd()) && stop == false)
            {
                QString line = in.readLine();
                QString k1f;
                QString k2f;
                // I don't see users using this many install directories. Do you? Nah :p
                // A nice buffer I guess
                for(int i = 0; i < 50; i++)
                {
                    QString instBasePath;
                    if(line.trimmed().startsWith(QString("\"BaseInstallFolder_%1\"").arg(i)))
                        instBasePath = line.replace(QString("\"BaseInstallFolder_%1\"").arg(i), "").trimmed().replace("\"", "");
                    else
                        instBasePath = steamPath;

                    k1f = QString("%1%2").arg(instBasePath, KOTOR_PATH);
                    k2f = QString("%1%2").arg(instBasePath, KOTOR2_PATH);

#ifdef Q_OS_WIN32
                    k1f.replace("\\\\", "\\").replace("/", "\\");
                    k2f.replace("\\\\", "\\").replace("/", "\\");
#endif
                    QFile k1(k1f + KOTOR_EXE);

                    if(k1.exists())
                    {

                        ui->leKotor->setText(k1f);
                        k = true;
                        if(k)
                            this->logger->write("KotOR 1 found. Applying to UI");
                    }

                    QFile k2(k2f + KOTOR2_EXE);
                    if(k2.exists())
                    {

                        ui->leKotor2->setText(k2f);
                        kk = true;
                        if(kk)
                            this->logger->write("KotOR 2 found. Applying to UI");
                    }

                    // Be prepared. Big if statement, I'll do what I can
                    // to clean it up later
                    if((k && kk) ||
                            (k || kk))
                    {
                        stop = true;
                        break;
                    }
                }
            }
        }
    }


}

// GOG stuff is win32 for now. I'll work with other OS's when GOG adds KotOR2 to Linux
void MainWindow::gogShit(QString gogKey)
{
#ifdef Q_OS_WIN32
    bool k1found = false, k2found = false;
    RegistryReader reader;
    reader.open(gogKey + "\\Games");
    if(reader.hasKey("1207666283"))
    {
        this->logger->write("KotOR 1 found for GOG. Applying to UI");
        reader.open(gogKey + "\\Games\\1207666283");
        ui->leKotor->setText(reader.getValue("PATH"));
        k1found = true;
    }
    reader.open(gogKey + "\\Games"); // Remember using reader to open another key? Gotta reopen the last one ;p
    if(reader.hasKey("1421404581"))
    {
        this->logger->write("KotOR 2 found for GOG. Applying to UI");
        reader.open(gogKey + "\\Games\\1421404581");
        ui->leKotor2->setText(reader.getValue("PATH"));
        k2found = true;
    }

    // Now we run the steam check if one or none of the
    // following conditions are met
    this->logger->write("Determining if Steam needs to be checked for Paths");
    if((k1found == false || k2found == false))
    {
        this->logger->write("Initializing steam search");
        this->steamShit();
    }
#else
    this->logger->write("Initializing steam search");
    this->steamShit();
#endif
}

void MainWindow::cdShit(QString game, QString key32, QString key64)
{
    RegistryReader reader;
    reader.open(key32); // check 32 bit first // 64 if 32 isn't found
    if(reader.hasKey("Path"))
    {
        if(game == "k1")
            ui->leKotor->setText(reader.getValue("Path"));
        else if(game == "k2")
            ui->leKotor2->setText(reader.getValue("Path"));
    }
    else
    {
        // 32 bit found nothing. Moving to 64 bit
        reader.open(key64);
        if(reader.hasKey("Path"))
        {
            if(game == "k1")
                ui->leKotor->setText(reader.getValue("Path"));
            else if(game == "k2")
                ui->leKotor2->setText(reader.getValue("Path"));
        }
    }
}

void MainWindow::onMenuItemExitClicked()
{
    qApp->quit();
}

void MainWindow::onMenuItemAboutClicked()
{
    AboutDialog *dlg = new AboutDialog();
    dlg->show();
}

void MainWindow::onMenuItemDeleteClicked()
{
    MsgBox msg;
    QFile ini(INI_PATH);
    this->logger->write("Checking for existance of INI");
    if(ini.exists())
    {
        this->logger->write("INI found, sending delete confirmation");
        msg = MsgBox(this, "Confirmation", "Are you sure you wish to remove kse.ini? You'll have to regenerate it if you need to use KSE again.", MsgBox::YesNo);
        msg.setIcon(MsgBox::IconQuestion);
        if(msg.exec() == MsgBox::Yes)
        {
            this->logger->write("Confirmed deletion, deleting INI");
#ifdef Q_OS_WIN32
            QDir dir(QString(INI_PATH).replace("kse.ini", ""));
            dir.removeRecursively();
#else
            ini.remove();
#endif
            msg = MsgBox(this, "INI Removal Successfull", "kse.ini has been successfully removed!", MsgBox::Ok);
            msg.exec();
        }
    }
    else
    {
        msg = MsgBox(this, "INI Removal Failed", "kse.ini does not exist, and therefore cannot be deleted. Did you really think you could get away with that?", MsgBox::Ok);
        msg.exec();
    }
}

void MainWindow::onMenuItemOpenClicked()
{
    QFile ini(INI_PATH);
    this->logger->write("Checking for existance of INI");
    if(ini.exists())
    {
        this->logger->write("INI found. Opening in default text editor");
        QDesktopServices::openUrl(QString(INI_PATH));
    }
    else
    {
        MsgBox msg(this, "INI Not Found", QString("%1 was not found on your system. Are you sure it exists?").arg(INI_PATH), MsgBox::Ok, MsgBox::IconError);
        msg.exec();
    }
}

void MainWindow::onMenuItemLoggerClicked()
{
    this->logger->write("Creating logger window");
    LoggerConsole *console = new LoggerConsole(this);
    this->logger->write("Registering logger to console");
    console->registerLogger(this->logger);
    console->show();
}

void MainWindow::onMenuItemLogsClicked()
{
    QDir dir(LOGS_DIR);
    if(dir.exists())
    {
        // Windows won't run Explorer for some reason
        // At least QProcess works....ass
#ifdef Q_OS_WIN32
       QString proc("C:\\Windows\\explorer.exe");
       QProcess::startDetached(proc, QStringList(dir.absolutePath().replace("/", "\\")));
#else
        QDesktopServices::openUrl(QString(LOGS_DIR));
#endif
    }
    else
    {
        MsgBox msg(this, "Logs Location Error", "The logs directory was not found on your PC!", MsgBox::Ok, MsgBox::IconError);
        msg.exec();
    }
}

void MainWindow::onUndoClicked()
{
    QFile ini(INI_PATH);
    this->logger->write("Attempting to revert generation of INI");
    if(ini.exists() && this->exported)
    {
        this->logger->write("INI found. Reverting changes");
        QFile tempini(INI_BACKUP);

        this->logger->write("Cleaning up backups");
        if(tempini.exists())
        {
            QFile::remove(INI_PATH);
            tempini.copy(INI_PATH);
            tempini.remove();
        }
        else
            QFile::remove(INI_PATH);

        MsgBox msg(this, "Undo Export", "Export has been undone!", MsgBox::Ok);
        msg.exec();
        this->exported = false;
    }
    else
    {
        MsgBox msg(this, "Undo Export", "Could not undo export. No changes were made", MsgBox::Ok);
        msg.exec();
    }
}

bool MainWindow::is32Bit()
{
    QString arch = QSysInfo::buildCpuArchitecture();
    if (arch == "i386")
        return true;

    return false;
}
