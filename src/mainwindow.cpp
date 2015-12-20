#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->logger = new Logger(this);

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
    k1onPlat = false;
#endif

    // Keyboard shortcuts
    ui->menuExit->setShortcut(Qt::CTRL | Qt::Key_Q);
    ui->menuAbout->setShortcut(Qt::Key_F1);
    ui->menuOpen->setShortcut(Qt::CTRL | Qt::Key_O);

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
    this->connect(ui->menuDelete, SIGNAL(triggered(bool)), this, SLOT(onMenuItemDeleteClicked()));

    
    this->connect(ui->menuOpen, SIGNAL(triggered(bool)), this, SLOT(onMenuItemOpenClicked()));

    detectPaths(false);

    // Show something happened. Let them know to export shit :p
    QFile ini(INI_PATH);
    if(!ini.exists())
    {
        MsgBox msg(this, "Paths Found", "KotOR and/or KotOR 2 were automatically detected on your system. All you need to do is generate the INI for KSE. Simply click \"Export to INI\" and you're all set to continue running KSE",
                   MsgBox::Ok);
        msg.exec();
    }
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
    if(changed)
    {
        MsgBox msg(this, "Unsaved Changes",
                   "Changes have been made in KPF. Are you sure you want to close without saving changes",
                   MsgBox::YesNo, "question");
        if(msg.exec() == MsgBox::No)
            event->ignore();
    }

    // We're not ignoring the event now, let's destroy our backup file!
    QFile file(INI_BACKUP);
    if(file.exists())
        file.remove();
}

void MainWindow::onQuitClicked()
{
    this->close();
}

void MainWindow::onK1BrowseClicked()
{
    if(this->browse(ui->leKotor->text(), KOTOR_EXE))
    {
        ui->leKotor->setText(tempPath);
    }
    else
    {
        MsgBox msg(this, "Invalid Path", "The kotor game executable could not be found! Is this the game's root directory?", MsgBox::Ok, "critical");
        msg.exec();
    }
}

void MainWindow::onK2BrowseClicked()
{
    if(this->browse(ui->leKotor2->text(), KOTOR2_EXE))
    {
        ui->leKotor2->setText(tempPath);
    }
    else
    {
        MsgBox msg(this, "Invalid Path", "The kotor2 game executable could not be found! Is this the game's root directory?", MsgBox::Ok, "critical");
        msg.exec();
    }
}

bool MainWindow::browse(QString location, const char *exe)
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly);
    if(location != "")
        dlg.setDirectory(location);
    int result = dlg.exec();
    QString dir;
    if(result)
    {
        dir = dlg.selectedFiles()[0];
        QFile file(dir + exe);
        if(file.exists())
        {
#ifdef Q_OS_WIN32
            dir.replace("/", "\\");
#endif
            tempPath = QString(dir);
            return true;
        }
    }

    return false;
}

void MainWindow::onINIExportClicked()
{
    // Let's make sure to back up the ini file shall we?
    QFile ini(INI_PATH);
    if(ini.exists())
        ini.copy(INI_BACKUP);

    try
    {
        INIReader reader;
        reader.open(INI_PATH);
        QFile *kf = new QFile(QString("%0%1").arg(ui->leKotor->text(), KOTOR_EXE));
        if(kf->exists())
        {
#ifdef Q_OS_UNIX
            reader.setValue("Installed", "K1_Installed", "0");
            reader.setValue("Paths", "K1_Path", "undef");
#else
            reader.setValue("Installed", "K1_Installed", "1");
            reader.setValue("Paths", "K1_Path", ui->leKotor->text());
#endif
        }
        else
        {
            reader.setValue("Installed", "K1_Installed", "0");
            reader.setValue("Paths", "K1_Path", "undef");
        }

        kf = new QFile(QString("%0%1").arg(ui->leKotor2->text(), KOTOR2_EXE));
        if(kf->exists())
        {
            reader.setValue("Installed", "K2_Installed", "1");
            reader.setValue("Paths", "K2_Path", ui->leKotor2->text());

            // kotor2 really needs special entry for it's regular saves?
            QString k2p;
#ifdef Q_OS_WIN32
            k2p = ui->leKotor2->text() + "\\saves";
#else
            k2p = KOTOR2_SAVES;
#endif
            reader.setValue("Paths", "K2_SavePath", k2p);
        }
        else
        {
            reader.setValue("Installed", "K2_Installed", "0");
            reader.setValue("Paths", "K2_Path", "undef");

            reader.setValue("Paths", "K2_SavePath", "undef");
        }

        // KotOR 2 cloud saves check
        QDir csdir(ui->leKotor2->text() + "/cloudsaves/");
        QStringList dirs = csdir.entryList();
        reader.setValue("Options", "Use_K2_Cloud", "0");
        reader.setValue("Paths", "K2_SavePathCloud", "undef");
        for(int i = 0; i < dirs.count(); i++)
        {
            QRegularExpression regex("([0-9]+)");
            QRegularExpressionMatch match = regex.match(dirs[i]);
            if(match.hasMatch())
            {
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
                reader.setValue("Options", "Use_K2_Cloud", "0");
                reader.setValue("Paths", "K2_SavePathCloud", "undef");
            }
        }

        // Last couble of values. We'll leave as null for now
        reader.setValue("Installed", "TJM_Installed", "0");
        reader.setValue("Paths", "TJM_Path", "undef");

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
    detectPaths(true);
    MsgBox msg(this, "Rescan Complete", QString(
                   "Rescaning has completed. You should now see your paths in the main window. ")
                   + "If you do not, they could not be obtained automatically. Please add them manually", MsgBox::Ok);
    msg.exec();
}

void MainWindow::loadINI()
{
    QFile iniFile(INI_PATH);
    if(iniFile.exists())
    {
        INIReader reader;
        reader.open(INI_PATH);
        QString k1path = reader.getValue("Paths", "K1_Path");
        QString k2path = reader.getValue("Paths", "K2_Path");
        steamPath = reader.getValue("Paths", "Steam_Path");

        if(k1path != "undef")
        {
            QFile file(k1path + KOTOR_EXE);
            if(file.exists())
                ui->leKotor->setText(k1path);
        }

        if(k2path != "undef")
        {
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
        this->changed = true;
#ifdef Q_OS_WIN32
        // We'll look for CD entries first. Since I only kave KotOR1 on CD, it's the only check for now
        RegistryReader reader;
        reader.open(KOTOR_CD_REG_KEY_64);
        if(reader.hasKey("Path"))
        {
            ui->leKotor->setText(reader.getValue("Path"));
        }

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
        this->steamShit();
#endif
    }
}

// <OLD_COMMENT>I'll clean this method up later :p</OLD_COMMENT>
// Method is cleaned up a bit. I'll rename later
void MainWindow::steamShit()
{
    QFile file(QString("%1%2").arg(DEFAULT_STEAM_PATH, STEAM_EXE));
    if(file.exists())
    {
        steamPath = DEFAULT_STEAM_PATH;
    }
    else
    {
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
        QFile config(steamPath + "/config/config.vdf");
        if(config.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&config);
            bool k = false, kk = false; // might remove...
            while(!in.atEnd())
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
                    }

                    QFile k2(k2f + KOTOR2_EXE);
                    if(k2.exists())
                    {
                        ui->leKotor2->setText(k2f);
                        kk = true;
                    }

                    // Be prepared. Big if statement, I'll do what I can
                    // to clean it up later
                    if((k && kk) ||
                            (k || kk))
                    {
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
        reader.open(gogKey + "\\Games\\1207666283");
        ui->leKotor->setText(reader.getValue("PATH"));
        k1found = true;
    }
    reader.open(gogKey + "\\Games"); // Remember using reader to open another key? Gotta reopen the last one ;p
    if(reader.hasKey("1421404581"))
    {
        reader.open(gogKey + "\\Games\\1421404581");
        ui->leKotor2->setText(reader.getValue("PATH"));
        k2found = true;
    }

    // Now we run the steam check if one or none of the
    // following conditions are met
    if((k1found == false || k2found == false))
        this->steamShit();
#else
    this->steamShit();
#endif
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
    if(ini.exists())
    {
        msg = MsgBox(this, "Confirmation", "Are you sure you wish to remove kse.ini? You'll have to regenerate it if you need to use KSE again.", MsgBox::YesNo);
        msg.setIcon("question");
        if(msg.exec() == MsgBox::Yes)
        {
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
    if(ini.exists())
        QDesktopServices::openUrl(QString(INI_PATH));
    else
    {
        MsgBox msg(this, "INI Not Found", QString("%1 was not found on your system. Are you sure it exists?").arg(INI_PATH), MsgBox::Ok, "critical");
        msg.exec();
    }
}

void MainWindow::onUndoClicked()
{
    QFile ini(INI_PATH);
    if(ini.exists() && this->exported)
    {
        QFile tempini(INI_BACKUP);

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
