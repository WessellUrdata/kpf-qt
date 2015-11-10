#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>
#include <QCloseEvent>
#include <QDesktopWidget>

#include "refs.h"
#include "inireader.h"
#include "registryreader.h"
#include "aboutdialog.h"

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
    void onUndoClicked();

    void onMenuItemExitClicked();
    void onMenuItemAboutClicked();

    void onTextChanged(QString);

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
    void loadINI();
    void detectPaths(bool rescan);
    void steamShit();
    void gogShit(QString gogKey);

    QString steamPath;
    bool k1onPlat;
    bool exported;
    bool changed;
};

#endif // MAINWINDOW_H
