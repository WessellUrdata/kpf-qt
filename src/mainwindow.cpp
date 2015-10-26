#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connect(ui->bQuit, SIGNAL(clicked(bool)), this, SLOT(onQuitClicked()));
    this->connect(ui->bK2Browse, SIGNAL(clicked(bool)), this, SLOT(onK2BrowseClicked()));
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
{}

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
