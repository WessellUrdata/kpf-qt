#include "msgbox.h"

int MsgBox::YesNo = 1;
int MsgBox::Yes = 16384;
int MsgBox::No = 65536;
int MsgBox::Ok = 1024;
int MsgBox::OkCancel = 2;
int MsgBox::Cancel = 4194304;
int MsgBox::NoButton = 0;

QString MsgBox::IconError = "critical";
QString MsgBox::IconQuestion = "question";
QString MsgBox::IconInfo = "info";

MsgBox::MsgBox(QWidget *parent, QString title, QString text, int button, QString icon)
{
    this->msgBox = new QMessageBox(parent);
    this->setTitle(title);
    this->setText(text);
    this->setButtonType(button);
    this->setIcon(icon);
}

void MsgBox::setButtonType(int button)
{
    this->msgBox->addButton(QMessageBox::NoButton);
    switch(button) {
    case 1:
        this->msgBox->addButton(QMessageBox::Yes);
        this->msgBox->addButton(QMessageBox::No);
        break;
    case 16384:
        this->msgBox->addButton(QMessageBox::Yes);
        break;
    case 1024:
        this->msgBox->addButton(QMessageBox::Ok);
        break;
    case 2:
        this->msgBox->addButton(QMessageBox::Cancel);
        this->msgBox->addButton(QMessageBox::Ok);
        break;
    case 0:
        this->msgBox->addButton(QMessageBox::NoButton);
        break;
    case 65536:
        this->msgBox->addButton(QMessageBox::No);
        break;
    case 4194304:
        this->msgBox->addButton(QMessageBox::Cancel);
        break;
    default:
        this->msgBox->addButton(QMessageBox::NoButton);
        break;
    }
}

void MsgBox::setIcon(QString type)
{
    // set icon
    QPixmap img;
    if(type == "info")
    {
        img = QPixmap(":images/res/images/info.png");
    }
    else if(type == "critical")
    {
        img = QPixmap(":images/res/images/critical.png");
    }
    else if(type == "question")
    {
        img = QPixmap(":images/res/images/question.png");
    }

    this->msgBox->setIconPixmap(img);
}

int MsgBox::exec()
{
    return this->msgBox->exec();
}

void MsgBox::setTitle(QString title)
{
    this->msgBox->setWindowTitle(title);
}

void MsgBox::setText(QString text)
{
    this->msgBox->setText(text);
}

void MsgBox::resetContent(QString title, QString text, int button, QString icon)
{
    this->setTitle(title);
    this->setText(text);
    this->setButtonType(button);
    this->setIcon(icon);
}

void MsgBox::close()
{
    this->msgBox->close();
}
