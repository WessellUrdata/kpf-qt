#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTextStream>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDirIterator>
#include <QDir>
#include <QScreen>

#include "refs.h"
#include "inireader.h"
#include "aboutdialog.h"
#include "logger.h"
#include "msgbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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
    void onMenuItemDeleteClicked();
    void onMenuItemOpenClicked();
    void onMenuItemLoggerClicked();
    void onMenuItemLogsClicked();

    void onTempMenuItemClicked();

    void onTextChanged(QString);

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *);
    void loadINI();
    void detectPaths(bool);
    void cdShit(QString game, QString key32, QString key64);
    void steamShit();
    bool browse(QString location, const char *);
    bool is32Bit();

    QString steamPath;
    bool k1onPlat;
    bool exported;
    bool changed;

    QString tempPath;
    Logger *logger;
};

#endif // MAINWINDOW_H
