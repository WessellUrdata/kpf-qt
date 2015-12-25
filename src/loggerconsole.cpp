#include "loggerconsole.h"
#include "ui_loggerconsole.h"

LoggerConsole::LoggerConsole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggerConsole)
{
    ui->setupUi(this);
}

LoggerConsole::~LoggerConsole()
{
    delete ui;
}
