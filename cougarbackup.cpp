#include "cougarbackup.h"
#include "ui_cougarbackup.h"

CougarBackup::CougarBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CougarBackup)
{
    ui->setupUi(this);
    //ui->pushButton_start->actions(start_button());
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(start_button()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel_button()));
}
void CougarBackup::start_button()
{
    // Code Here
}

void CougarBackup::cancel_button()
{
    close();
}

CougarBackup::~CougarBackup()
{
    delete ui;
}
