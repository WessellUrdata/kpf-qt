#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup default values
    this->steamPath = "undef";

    this->connect(ui->bQuit, SIGNAL(clicked(bool)), this, SLOT(onQuitClicked()));
    this->connect(ui->bK1Browse, SIGNAL(clicked(bool)), this, SLOT(onK1BrowseClicked()));
    this->connect(ui->bK2Browse, SIGNAL(clicked(bool)), this, SLOT(onK2BrowseClicked()));
    this->connect(ui->bExportINI, SIGNAL(clicked(bool)), this, SLOT(onINIExportClicked()));
    this->connect(ui->bExportReg, SIGNAL(clicked(bool)), this, SLOT(onRegistryExport()));

    this->connect(ui->menuExit, SIGNAL(triggered(bool)), this, SLOT(onMenuItemExitClicked()));

    detectPaths();

    // OS specific gaming stuff
    // certain ones are not on certain OSs
#ifdef Q_OS_UNIX
    ui->leKotor->setEnabled(false);
    ui->leKotor->setText("KotOR is not available on this platform!");
    k1onPlat = false;
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onQuitClicked()
{
    qApp->quit();
}

void MainWindow::onK1BrowseClicked()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly);
    int result = dlg.exec();
    QString dir;
    if(result)
    {
        dir = dlg.selectedFiles()[0];
        QFile file(dir + "\\swkotor.exe");
        if(file.exists())
        {
#ifdef Q_OS_WIN32
            dir.replace("/", "\\");
#endif
            ui->leKotor->setText(dir);
        }
        else
            QMessageBox::critical(this, "Invalid Path", "swkotor.exe was not found in this directory. Please try again", QMessageBox::Ok);
    }
}

void MainWindow::onK2BrowseClicked()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly);
    int result = dlg.exec();
    QString dir;
    if(result)
    {
        dir = dlg.selectedFiles()[0];
        QFile file(dir + "\\swkotor2.exe");
        if(file.exists())
        {
#ifdef Q_OS_WIN32
            dir.replace("/", "\\");
#endif
            ui->leKotor2->setText(dir);
        }
        else
            QMessageBox::critical(this, "Invalid Path", "swkotor2.exe was not found in this directory. Please try again", QMessageBox::Ok);
    }
}

void MainWindow::onINIExportClicked()
{
    try
    {
        INIReader reader;
        reader.open("kse.ini");
        if(ui->leKotor->text() != "" || !k1onPlat)
        {
            reader.setValue("Installed", "K1_Installed", "1");
            reader.setValue("Paths", "K1_Path", ui->leKotor->text());
        }
        else
        {
            reader.setValue("Installed", "K1_Installed", "0");
            reader.setValue("Paths", "K1_Path", "undef");
        }

        if(ui->leKotor2->text() != "")
        {
            reader.setValue("Installed", "K2_Installed", "1");
            reader.setValue("Paths", "K2_Path", ui->leKotor2->text());

            // kotor2 really needs special entry for it's regular saves?
            reader.setValue("Paths", "K2_SavePath", ui->leKotor2->text() + "\\saves");
        }
        else
        {
            reader.setValue("Installed", "K2_Installed", "0");
            reader.setValue("Paths", "K2_Path", "undef");

            reader.setValue("Paths", "K2_SavePath", "undef");
        }

        // KotOR 2 cloud saves check
        QDir csdir(ui->leKotor2->text() + "\\cloudsaves\\");
        QStringList dirs = csdir.entryList();
        for(int i = 0; i < dirs.count(); i++)
        {
            QRegularExpression regex("([0-9]+)");
            QRegularExpressionMatch match = regex.match(dirs[i]);
            if(match.hasMatch())
            {
                reader.setValue("Options", "Use_K2_Cloud", "1");
                reader.setValue("Paths", "K2_SavePathCloud", QString(csdir.absolutePath() + "\\" + dirs[i]).replace("/", "\\"));
                break;
            }
            else
            {
                reader.setValue("Options", "Use_K2_Cloud", "0");
                reader.setValue("Paths", "K2_SavePathCloud", "undef");
            }
        }

        reader.setValue("Paths", "Steam_Path", steamPath);
        QMessageBox::information(this, "INI Export", "Contents successfully exported to kse.ini");
    }
    catch(...)
    {
        QMessageBox::critical(this, "INI Export Error", "Error exporting values to kse.ini. Please try again later");
    }
}

void MainWindow::onRegistryExport()
{
#ifdef Q_OS_WIN32
    detectPaths();
    QMessageBox::information(this, "Rescan Complete", QString("Registry rescanning has been complete. You should now see your paths in the main window. ")
                             + "If you do not, they could not be obtained by the registry. Please add them manually.");
#else
    QMessageBox::critical(this, "Win32", "This option is only available on Windows. Sorry.");
#endif
}

void MainWindow::loadINI()
{
    QFile iniFile("kse.ini");
    if(iniFile.exists())
    {
        INIReader reader;
        reader.open("kse.ini");
        QString k1path = reader.getValue("Paths", "K1_Path");
        QString k2path = reader.getValue("Paths", "K2_Path");
        steamPath = reader.getValue("Paths", "Steam_Path");

        if(k1path != "undef")
        {
            QFile file(k1path + "\\swkotor.exe");
            if(file.exists())
                ui->leKotor->setText(k1path);
        }

        if(k2path != "undef")
        {
            QFile file(k2path + "\\swkotor2.exe");
            if(file.exists())
                ui->leKotor2->setText(k2path);
        }
    }
}

#include <QTextStream>

void MainWindow::detectPaths()
{
#ifdef Q_OS_WIN32
    QFile ini("kse.ini");
    if(!ini.exists())
    {
        QString gogKey = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GOG.com";
        QString steamKey = "HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam";

        // Check GOG first, it requires an extra step Steam doesn't
        RegistryReader gogReader;
        gogReader.open(gogKey);
        if(gogReader.hasKey("Games"))
        {
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
            if(k1found == false || k2found == false)
                steamShit(steamKey);
        }
        else
        {
            // Here we do steam stuff. It's been placed in a seperate method in case
            // One game is on GOG and another on Steam. Ya know, now everyone gets games
            // on the same platform ;)
            steamShit(steamKey); // I'll rename to something better later :p
        }
    }
    else
        loadINI();
#else
    QFile file("kse.ini");
    if(file.exists())
        loadINI();
    else
        QMessageBox::information(this, "KPF Information", "kse.ini does not exist, and could not get info from Steam. Please find your game paths manually, and export to an INI.");
#endif
}

void MainWindow::steamShit(QString steamKey)
{
    RegistryReader reader;
    reader.open(steamKey);

    // Win32 slash fix (registry is win32 only anyhow, no need for preprocessor check'
    if(reader.getValue("SteamPath") != "")
        steamPath = reader.getValue("SteamPath").replace("/", "\\");

    QFile steamExe(steamPath + "\\steam.exe");
    if(steamExe.exists())
    {
        QFile config(steamPath + "\\config\\config.vdf");
        if(config.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&config);
            while(!in.atEnd())
            {
                QString line = in.readLine();
                if(line.trimmed().startsWith("\"BaseInstallFolder_1\""))
                {
                    QString instBasePath = line.replace("\"BaseInstallFolder_1\"", "").trimmed().replace("\"", "");

                    // KotOR 1
                    QString k1p = instBasePath + "\\steamapps\\common\\swkotor";
                    QFile k1f(k1p + "\\swkotor.exe");
                    if(k1f.exists())
                        ui->leKotor->setText(k1p.replace("\\\\", "\\"));

                    // KotOR 2
                    QString k2p = instBasePath + "\\steamapps\\common\\Knights of the Old Republic II";
                    QFile k2f(k2p + "\\swkotor2.exe");
                    if(k2f.exists())
                        ui->leKotor2->setText(k2p.replace("\\\\", "\\"));
                }
            }
        }
    }
}

void MainWindow::onMenuItemExitClicked()
{
    qApp->quit();
}
