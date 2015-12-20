#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFont>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    QFile f(":styles/res/stylesheets/flat.css");
    if(f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    int id = QFontDatabase::addApplicationFont(":fonts/res/fonts/ubuntu.ttf");
    QString fam = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont ubuntu(fam);
    qApp->setFont(ubuntu);

    MainWindow w;
    w.show();

    return a.exec();
}
