#include "cougarbackup.h"
#include "ui_cougarbackup.h"

CougarBackup::CougarBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CougarBackup)
{
    ui->setupUi(this);
    /* Setup Buttons
    // Setup Start, Cancel, and Browse Buttons
    // clicked -> when pressed
    */
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(start_button()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel_button()));
    connect(ui->pushButton_browse, SIGNAL(clicked()), this, SLOT(browse_button()));

    /* Setup Text Fields
    // Setup Name and Number Fields
    // textChanged -> Updates Constantly
    // editingFinished -> Click outside of box
    */
    connect(ui->lineEdit_name, SIGNAL(editingFinished()), this, SLOT(name_textfield()));
    connect(ui->lineEdit_number, SIGNAL(editingFinished()), this, SLOT(number_textfield()));

    /* Setup ComboBox
     * currentIndexChanged(int) -> "int" is important for making it work
    */
    connect(ui->comboBox_os, SIGNAL(currentIndexChanged(int)), this, SLOT(os_combobox()));
}
void CougarBackup::os_combobox()
{
    //os_str = QFileDialog::getExistingDirectory();
    os_str.clear();
    os_str = ui->comboBox_os->currentText();
    ui->textBrowser_backupStats->setText(os_str);
    //os_str = comboBox_os->itemData(comboBox_os->currentIndex());
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
}

void CougarBackup::number_textfield()
{
    number_text = ui->lineEdit_number->text(); // QString containing the contents of lineEdit_number
    calculate_destination();
}

void CougarBackup::calculate_destination()
{
    destination_str.clear();
    default_directory.clear();
    default_directory.append("/media/backupfolder/");
    destination_str.append(default_directory);
    destination_str.append(name_text);
    destination_str.append(number_text);
    destination_str.append(os_str);
    ui->textBrowser_destination->setText(destination_str);
}

CougarBackup::~CougarBackup()
{
    delete ui;
}
