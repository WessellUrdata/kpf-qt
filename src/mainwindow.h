#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

#include "inireader.h"
#include "registryreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onQuitClicked();
    void onK1BrowseClicked();
    void onK2BrowseClicked();
    void onINIExportClicked();
    void onRescanClicked();

    void onMenuItemExitClicked();

private:
    Ui::MainWindow *ui;
    void loadINI();
    void detectPaths();
    void steamShit(QString steamKey);
    void gogShit(QString steamKey, QString gogKey);

    QString steamPath;
    bool k1onPlat;
};

#endif // MAINWINDOW_H
