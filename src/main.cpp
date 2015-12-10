#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>
#include <QPalette>
#include <QColor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFile f(":styles/qdarkstyle.css");
    if(f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    return a.exec();
}
