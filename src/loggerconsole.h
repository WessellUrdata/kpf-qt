#ifndef LOGGERCONSOLE_H
#define LOGGERCONSOLE_H

#include <QDialog>

namespace Ui {
class LoggerConsole;
}

class LoggerConsole : public QDialog
{
    Q_OBJECT

public:
    explicit LoggerConsole(QWidget *parent = 0);
    ~LoggerConsole();

private:
    Ui::LoggerConsole *ui;
};

#endif // LOGGERCONSOLE_H
