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

    /*Grab config settings*/
    get_config();
}
void CougarBackup::update_backup_stats()
{
    backup_stats.clear();

    // List OS
    backup_stats.append("OS: ");
    backup_stats.append(os_str);

    // Take the backup size in bytes, convert it to GB
    backup_stats.append("\nBackupSize: ");
    backup_stats.append(QString::number((double)source_size/1000000000,'f', 3));
    backup_stats.append(" GB");

    // Write to backup_stats box
    ui->textBrowser_backupStats->setText(backup_stats);
}

void CougarBackup::os_combobox()
{
    os_str.clear();
    os_str = ui->comboBox_os->currentText();
    update_backup_stats();
}

void CougarBackup::start_button()
{
    source_size = compute_source_size(source_str);
    update_backup_stats();
}

void CougarBackup::cancel_button()
{
    close();
}

void CougarBackup::browse_button()
{
    source_str = QFileDialog::getExistingDirectory();
    ui->textBrowser_source->setText(source_str);
    update_backup_stats();
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
    //default_directory.clear();
    //default_directory.append("/media/backupfolder/");
    destination_str.append(default_directory);
    destination_str.append(name_text);
    destination_str.append(number_text);
    ui->textBrowser_destination->setText(destination_str);
    update_backup_stats();
}

void CougarBackup::get_config()
{
    QFile config_file("settings.cfg");
    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        default_directory.clear();
        default_directory.append("Error Reading Config");
        return; //terminate if settings.cfg is not found
    }
    QTextStream in(&config_file);
    QString line = in.readLine();//read in default directory
    default_directory.clear();
    default_directory.append(line);
}

qlonglong CougarBackup::compute_source_size(const QString &path)
{
    // http://www.informit.com/articles/article.aspx?p=1405549&seqNum=3
    // Credit to Jasmin Blanchette and Mark Summerfield
    QDir dir(path);
    qlonglong size = 0;

    // Needs to be updated to filter out symlinks
    QStringList filters;
    //foreach (QByteArray format, QImageReader::supportedImageFormats())
    //   filters += "*." + format;

    foreach (QString file, dir.entryList(filters, QDir::Files))
        size += QFileInfo(dir, file).size();

    foreach (QString subDir, dir.entryList(QDir::Dirs
                                           | QDir::NoDotAndDotDot))
        size += CougarBackup::compute_source_size(path + QDir::separator() + subDir);

    return size;
}

CougarBackup::~CougarBackup()
{
    delete ui;
}
