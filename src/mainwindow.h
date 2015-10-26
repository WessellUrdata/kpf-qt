#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
