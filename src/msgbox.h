#ifndef MSGBOX_H
#define MSGBOX_H

#include <QObject>
#include <QMessageBox>
#include <QPixmap>

class MsgBox
{
public:
    explicit MsgBox(QWidget *parent = 0, QString title = "", QString text = "", int button = MsgBox::NoButton, QString icon = "info");
    void setIcon(QString type);
    int exec();
    void setTitle(QString title);
    void setText(QString text);
    void resetContent(QString title, QString text, int button = MsgBox::NoButton, QString icon = "info");
    void close();

    static int YesNo;
    static int Yes;
    static int No;
    static int Ok;
    static int OkCancel;
    static int Cancel;
    static int NoButton;

private:
    QMessageBox *msgBox;
    void setButtonType(int button);
    QPixmap msgError;
    QPixmap msgInfo;
    QPixmap msgQuestion;
};

#endif // MSGBOX_H
