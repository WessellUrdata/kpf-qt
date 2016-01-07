#ifndef LOGGERCONSOLE_H
#define LOGGERCONSOLE_H

#include <QDialog>
#include <QMenuBar>
#include <QMenu>

#include "logger.h"

namespace Ui {
class LoggerConsole;
}

class LoggerConsole : public QDialog
{
    Q_OBJECT

public:
    explicit LoggerConsole(QWidget *parent = 0);
    ~LoggerConsole();
    void registerLogger(Logger *logger);

public slots:
    void appendLine(QString text);
    void onCloseButtonClicked();

private:
    Ui::LoggerConsole *ui;
    Logger *logger;

    void createMenuBar();

    QMenu *fileMenu;
    QMenu *itemExit;
    QMenu *itemView;

};

#endif // LOGGERCONSOLE_H
