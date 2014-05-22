#include "cougarbackup.h"
#include "ui_cougarbackup.h"

CougarBackup::CougarBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CougarBackup)
{
    ui->setupUi(this);
}

CougarBackup::~CougarBackup()
{
    delete ui;
}
