#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->connect(this->ui->bClose, SIGNAL(clicked(bool)), this, SLOT(onCloseClick()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onCloseClick()
{
    this->close();
}
