#include "cougarbackup.h"
#include "ui_cougarbackup.h"

CougarBackup::CougarBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CougarBackup)
{
    ui->setupUi(this);

    // Setup Start, Cancel, and Browse Buttons
    // clicked -> when pressed
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(start_button()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel_button()));
    connect(ui->pushButton_browse, SIGNAL(clicked()), this, SLOT(browse_button()));

    // Setup Name and Number Fields
    // textChanged -> Updates Constantly
    // editingFinished -> Click outside of box
    connect(ui->lineEdit_name, SIGNAL(editingFinished()), this, SLOT(name_textfield()));
    connect(ui->lineEdit_number, SIGNAL(editingFinished()), this, SLOT(number_textfield()));
}
void CougarBackup::start_button()
{
    // Code Here
}

void CougarBackup::cancel_button()
{
    close();
}

void CougarBackup::browse_button()
{
    source_str = QFileDialog::getExistingDirectory();
    ui->textBrowser_source->setText(source_str);
}

void CougarBackup::name_textfield()
{
    name_text = ui->lineEdit_name->text(); // QString containing the contents of lineEdit_name
    calculate_destination();
    //ui->textBrowser_destination->setText(name_text);
}

void CougarBackup::number_textfield()
{
    number_text = ui->lineEdit_number->text(); // QString containing the contents of lineEdit_number
    calculate_destination();
    //number_text = ui->lineEdit_number->text();
    //ui->textBrowser_source->setText(number_text);
}

void CougarBackup::calculate_destination()
{
    destination_str.clear();
    default_directory.clear();
    default_directory.append("/media/backupfolder/");
    destination_str.append(default_directory);
    destination_str.append(name_text);
    destination_str.append(number_text);
    ui->textBrowser_destination->setText(destination_str);
}

CougarBackup::~CougarBackup()
{
    delete ui;
}
