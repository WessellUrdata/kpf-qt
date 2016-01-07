#include "loggerconsole.h"
#include "ui_loggerconsole.h"

LoggerConsole::LoggerConsole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggerConsole)
{
    ui->setupUi(this);

    this->connect(this->ui->bClose, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
//    this->createMenuBar(); // I'll figure it out one day...
}

LoggerConsole::~LoggerConsole()
{
    delete ui;
}

void LoggerConsole::registerLogger(Logger *logger)
{
    this->logger = logger;
    this->logger->write("Logger registered to console. Updating UI");
    this->ui->txtContents->setText(logger->getCurrentLog());

    // register signal/slots for real-time update
    this->logger->write("Registering signal/slot connection to logger");
    this->connect(this->logger, SIGNAL(appendLine(QString)), this, SLOT(appendLine(QString)));
}

void LoggerConsole::appendLine(QString text)
{
    this->ui->txtContents->setText(text);
}

void LoggerConsole::onCloseButtonClicked()
{
    this->logger->write("Terminating logger");
    this->close();
}

void LoggerConsole::createMenuBar()
{
    QMenuBar *menu = new QMenuBar(ui->centralWidget);
    menu->adjustSize();

    this->fileMenu = new QMenu("&File");

    menu->addMenu(this->fileMenu);
}
