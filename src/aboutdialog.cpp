#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->connect(ui->bClose, SIGNAL(clicked(bool)), this, SLOT(onCloseClick()));
    this->connect(ui->bWebsite, SIGNAL(clicked(bool)), this, SLOT(onWebsiteClick()));

    ui->lblVersion->setText(QString("Version: %1").arg(APP_VERSION));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onCloseClick()
{
    this->close();
}

void AboutDialog::onWebsiteClick()
{
    QDesktopServices::openUrl(QUrl("https://gitlab.com/kotorsge-team"));
}
