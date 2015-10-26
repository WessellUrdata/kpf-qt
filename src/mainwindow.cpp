#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connect(ui->bQuit, SIGNAL(clicked(bool)), this, SLOT(onQuitClicked()));
    this->connect(ui->bK1Browse, SIGNAL(clicked(bool)), this, SLOT(onK1BrowseClicked()));
    this->connect(ui->bK2Browse, SIGNAL(clicked(bool)), this, SLOT(onK2BrowseClicked()));
    this->connect(ui->bExportINI, SIGNAL(clicked(bool)), this, SLOT(onINIExportClicked()));

    loadINI();
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
    INIReader reader;
    reader.open("kse.ini");
    if(ui->leKotor->text() != "")
    {
        reader.setValue("Installed", "K1_Installed", "1");
        reader.setValue("Paths", "K1_Path", ui->leKotor2->text());
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
    }
    else
    {
        reader.setValue("Installed", "K2_Installed", "0");
        reader.setValue("Paths", "K2_Path", "undef");
    }
}

void MainWindow::loadINI()
{
    QFile iniFile("kse.ini");
    if(iniFile.exists())
    {
        INIReader reader;
        reader.open("kse.ini");
        ui->leKotor2->setText(reader.getValue("Paths", "K2_Path"));
    }
}
