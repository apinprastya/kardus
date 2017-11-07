#include "maindialog.h"
#include "ui_maindialog.h"
#include "welcomewidget.h"
#include "discwidget.h"
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    mWelcomeWidget(new WelcomeWidget(this)),
    mDiskWidget(new DiscWidget(this))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(mWelcomeWidget);
    ui->stackedWidget->addWidget(mDiskWidget);
    ui->stackedWidget->setCurrentIndex(0);
    connect(mWelcomeWidget, SIGNAL(nextClicked()), SLOT(nextClicked()));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::nextClicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
}
