#ifndef DISKWIDGET_H
#define DISKWIDGET_H

#include <QWidget>

namespace Ui {
class DiscWidget;
}

class DiscWidget : public QWidget
{
    Q_OBJECT

public:
    DiscWidget(QWidget *parent = 0);
    ~DiscWidget();

private:
    Ui::DiscWidget *ui;
    QString mSelectedPartition;

signals:
    void nextClicked();

private slots:
    void openGParted();
    void refreshPartition();
    void checkEnableButton();
    void checkNext();
};

#endif // DISKWIDGET_H
