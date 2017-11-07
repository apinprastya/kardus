#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace Ui {
class MainDialog;
}

class WelcomeWidget;
class DiscWidget;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    Ui::MainDialog *ui;
    WelcomeWidget *mWelcomeWidget;
    DiscWidget *mDiskWidget;

private slots:
    void nextClicked();
};

#endif // MAINDIALOG_H
