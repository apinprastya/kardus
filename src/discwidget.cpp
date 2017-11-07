#include "discwidget.h"
#include "ui_discwidget.h"
#include <parted/parted.h>
#include <parted/device.h>
#include <parted/disk.h>
#include <QProcess>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

DiscWidget::DiscWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscWidget)
{
    ui->setupUi(this);
    connect(ui->pushGParted, SIGNAL(clicked(bool)), SLOT(openGParted()));
    connect(ui->pushRefresh, SIGNAL(clicked(bool)), SLOT(refreshPartition()));
    connect(ui->pushNext, SIGNAL(clicked(bool)), SLOT(checkNext()));
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(checkEnableButton()));
    auto hi = ui->treeWidget->headerItem();
    hi->setText(0, tr("HDD / Partition Path"));
    hi->setText(1, tr("Filesystem"));
    hi->setText(2, tr("Size"));
    ui->treeWidget->setColumnWidth(0, 400);
    ui->treeWidget->setColumnWidth(1, 200);
    QTimer::singleShot(100, this, SLOT(refreshPartition()));
}

DiscWidget::~DiscWidget()
{
    delete ui;
}

void DiscWidget::openGParted()
{
    QProcess::startDetached("gparted");
}

void DiscWidget::refreshPartition()
{
    ui->treeWidget->clear();
    ped_device_probe_all();
    PedDevice *device = ped_device_get_next(NULL);
    while(device != NULL) {
        auto rootItem = new QTreeWidgetItem(QStringList{QString("%1 (%2)").arg(QString(device->model)).arg(QString(device->path))});
        PedDisk *disk = ped_disk_new(device);
        if(disk != NULL) {
            PedPartition *partition = disk->part_list;
            while(partition != NULL) {
                if(partition->fs_type != NULL) {
                    auto _item = new QTreeWidgetItem(QStringList{QString(ped_partition_get_path(partition)),
                                                        QString(partition->fs_type->name),
                                                        QString(ped_unit_format(device, (partition->geom.end - partition->geom.start)))});
                    rootItem->addChild(_item);
                }
                if(partition->type == PED_PARTITION_EXTENDED) {
                    PedPartition *prt = partition->part_list;
                    while(prt != NULL) {
                        if(prt->fs_type != NULL) {
                            auto _item = new QTreeWidgetItem(QStringList{QString(ped_partition_get_path(prt)),
                                                            QString(prt->fs_type->name),
                                                            QString(ped_unit_format(device, (prt->geom.end - prt->geom.start)))});
                            rootItem->addChild(_item);
                        }
                        prt = prt->next;
                    }
                }
                partition = partition->next;
            }
        }
        ui->treeWidget->addTopLevelItem(rootItem);
        device = ped_device_get_next(device);
    }
    ped_device_free_all();
}

void DiscWidget::checkEnableButton()
{
    auto item = ui->treeWidget->currentItem();
    mSelectedPartition = item->text(0);
    ui->pushNext->setEnabled(!item->text(1).compare("ext4", Qt::CaseInsensitive));
}

void DiscWidget::checkNext()
{
    int res = QMessageBox::warning(this, tr("Warning Partition"),
                                   tr("The partiion %1 will be formatted. Are you sure to continue?").arg(mSelectedPartition), QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::Yes)
        emit nextClicked();
}
