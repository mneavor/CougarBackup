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
    backup_stats.append(source_size.c_str());

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
    //source_size = compute_source_size(source_str);
    //update_backup_stats();
}

void CougarBackup::cancel_button()
{
    close();
}

void CougarBackup::browse_button()
{
    source_str = QFileDialog::getExistingDirectory();
    source_size = compute_source_size(source_str);
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
    //Reads in settings file from CougarBackup Folder
    QFile config_file("../CougarBackup/settings.cfg");
    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        default_directory.clear();
        default_directory.append("/media/backupfolder/");
        return; //terminate if settings.cfg is not found
    }
    QTextStream in(&config_file);
    QString line = in.readLine();//read in default directory
    default_directory.clear();
    default_directory.append(line);
}

string CougarBackup::compute_source_size(const QString &path)
{
    string s1 = "du -hc ";
    string s2 = path.toStdString();
    string s3 = " | tail -n 1 | awk '{print $1}'";
    string s4 = s1 + s2 + s3;
    //double size = system(s4.c_str());
    // popen
    //FILE *file_size = popen(s4.c_str(), "r");

    FILE *fpipe;
    const char *command = s4.c_str();
    char line[256];

    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(1);
    }

    while (fread(line, sizeof line, 1, fpipe))
    {
        printf("%s", line);
    }
    pclose(fpipe);
    source_size = line;
    return line;
}

//function to grab the common directories that we backup, spcifically the users and the config (on windows). This way we can narrow down the search easier
void CougarBackup::get_directories(){
//check the target OS

    if(os_str == "Mac"){//mac

       QDir users(source_str + "/Users");

       folders.push(users);


    }else if(os_str == "Linux"){//Linux

           QDir users(source_str + "/Home"); //I think this will be the right path

           folders.push(users);

        }else{//is windows

            QDir config(source_str + "\\Windows\\Systm32\\config");
            QDir users("\\Users");

            folders.push(config);
            folders.push(users);

    }

}

CougarBackup::~CougarBackup()
{
    delete ui;
}
