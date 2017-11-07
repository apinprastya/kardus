#include "welcomewidget.h"
#include "ui_welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    connect(ui->pushNext, SIGNAL(clicked(bool)), SIGNAL(nextClicked()));
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}
