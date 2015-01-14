#include "cougarbackup.h"
#include "ui_cougarbackup.h"

CougarBackup::CougarBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CougarBackup)
{
    ui->setupUi(this);
    this->window()->setGeometry(0,0,770,400);
    this->window()->resize(770,400);
    this->setGeometry(0,0,770,400);
    /* Setup Buttons
    // Setup Start, Cancel, Exclude, and Browse Buttons
    // clicked -> when pressed
    */
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(start_button()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel_button()));
    connect(ui->pushButton_browse, SIGNAL(clicked()), this, SLOT(browse_button()));
    connect(ui->pushButton_exclude, SIGNAL(clicked()), this, SLOT(exclude_button()));
    connect(ui->pushButton_exclude, SIGNAL(clicked()), this, SLOT(list_directories()));
    connect(ui->pushButton_config, SIGNAL(clicked()), this, SLOT(config_button()));
    connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(clear_excluded_folders()));

    /* Setup Text Fields
    // Setup Name and Number Fields
    // textChanged -> Updates Constantly
    // editingFinished -> Click outside of box
    */
    connect(ui->lineEdit_name, SIGNAL(editingFinished()), this, SLOT(name_textfield()));
    connect(ui->lineEdit_number, SIGNAL(editingFinished()), this, SLOT(number_textfield()));

    /* Setup Rsync output window
       create new process and timer*/
    ui->textBrowser_output->setVisible(false);
    ui->textBrowser_output->setEnabled(false);
    ui->pushButton_outFinished->setVisible(false);
    ui->pushButton_outFinished->setEnabled(false);
    proc = new QProcess;

    //Use a text file to store standard out of rsync
    process_file = "rsync_output.txt";
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->setStandardOutputFile(process_file);

    process_timer = new QTimer(0);
    process_timer->setInterval(10);
    process_timer->setSingleShot(false);
    connect(process_timer, SIGNAL(timeout()), this, SLOT(update_output()));
    connect(proc, SIGNAL(finished(int)), this, SLOT(executeFinished()));
    connect(ui->pushButton_outFinished, SIGNAL(clicked()), this, SLOT(close_output()));

    /* Setup Checkboxes
     * initialize config button to invisible aswell
    */
    connect(ui->checkBox_update, SIGNAL(stateChanged(int)), this, SLOT(update_checkbox()));
    connect(ui->checkBox_windows, SIGNAL(stateChanged(int)), this, SLOT(windows_checkbox()));
    ui->pushButton_config->setVisible(false);
    ui->pushButton_config->setEnabled(false);
    is_windows = false;
    update_option = false;
    config_backedup = false;

    /*Grab config settings*/
    get_config();

    /* Create Welcome Dialog */
    QMessageBox *welcome_window = new QMessageBox();
    welcome_window->addButton(QMessageBox::Ok);
    welcome_window->setText("Welcome!\nHovering over buttons and textboxes will give useful information of what the object does");
    welcome_window->move(QApplication::desktop()->screen()->rect().center() - welcome_window->rect().center() + QPoint(50, 200));
    welcome_window->setWindowFlags(Qt::WindowStaysOnTopHint);
    welcome_window->show();

    ui->pushButton_clear->setToolTip("Removes all folders from exclusion list");
    ui->checkBox_update->setToolTip("Check this if running rsync again on the same backup source. Will backup with the -u option");
    ui->checkBox_windows->setToolTip("Check this if backup source is running Windows");
    ui->pushButton_exclude->setToolTip("Select which folder to exclude (e.g AppData); Press again to exclude more");
    ui->lineEdit_name->setToolTip("Enter Customers Last Name");
    ui->lineEdit_number->setToolTip("Enter 4-digit ticket number");
    ui->pushButton_browse->setToolTip("Select source directory (e.g Users folder)");
}

void CougarBackup::update_checkbox()
{
    if (update_option == false)
    {
        update_option = true;
    }
    else
    {
        update_option = false;
    }
    return;
}

void CougarBackup::windows_checkbox()
{
    if (is_windows == false)
    {
        is_windows = true;
        ui->pushButton_config->setVisible(true);
        ui->pushButton_config->setEnabled(true);
    }
    else
    {
        is_windows = false;
        ui->pushButton_config->setVisible(false);
        ui->pushButton_config->setEnabled(false);
    }
    return;
}

void CougarBackup::config_button()
{
    config_source = "\'";
    config_source += QFileDialog::getExistingDirectory();
    config_source += "\'";
    ui->pushButton_config->setToolTip(config_source);
    return;
}

void CougarBackup::start_button()
{
    if (source_str.isEmpty() || name_text.isEmpty() || number_text.isEmpty())
    {
        QMessageBox::information(this, "Error!", "Please make sure a source directory is selected and Name/Number input fields are filled", QMessageBox::Ok);
        return;
    }
    executeRsync();
}

void CougarBackup::cancel_button()
{
    close();
}

void CougarBackup::browse_button()
{
    source_str = "\'";
    source_str += QFileDialog::getExistingDirectory();
    source_str += "\'";
    ui->textBrowser_source->setText(source_str);
    ui->textBrowser_source->setToolTip(source_str);
    return;
}

void CougarBackup::exclude_button()
{
    // Appends the selected directory to the excluded folders vector
    QString absolute_path = QFileDialog::getExistingDirectory(); // get the absolute path first
    QDir absolute_dir(absolute_path);

    QString folder_str = "\'";
    folder_str += absolute_dir.dirName(); // append only the directory name since rsync excludes folder relative to the source folder
    folder_str += "\'"; // add ' to beginning and end of string incase there are spaces in folder name
    excluded_folders_vector.push_back(folder_str);
    return;
}

void CougarBackup::clear_excluded_folders()
{
    excluded_folders_vector.clear(); // Remove excluded folders and update list
    list_directories();
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
    destination_str = "\'";
    destination_str.append(default_directory);
    destination_str.append(name_text);
    destination_str.append(number_text);
    destination_str.append("\'");
    ui->textBrowser_destination->setText(destination_str);
    ui->textBrowser_destination->setToolTip(destination_str);
}

void CougarBackup::get_config()
{
    // Reads in settings file from CougarBackup Folder

    QFile config_file("../CougarBackup/settings.cfg");
    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        default_directory.clear();
        default_directory.append("/media/backupfolder/");
        return; // Set default directory to ^^ if settings.cfg is not found
    }
    QTextStream in(&config_file);
    QString line = in.readLine();//read in default directory
    default_directory.clear();
    default_directory.append(line);
}

void CougarBackup::list_directories()
{
    folder_list.clear();
    for (int i = 0; i < (int)excluded_folders_vector.size(); i++)
    {
        folder_list.append(excluded_folders_vector[i]);
        folder_list.append("\n");
    }
     ui->textBrowser_folders->setText(folder_list);
}

void CougarBackup::executeRsync()
{
    QString command;

    command.append("\"rsync ");
    if (update_option == true)
    {
        command.append("-avzrPu --ignore-errors "); // Append -u if update option is selected
    }
    else
    {
        command.append("-avzrP --ignore-errors ");
    }

    if (is_windows == false)
    {
        command.append("--exclude '.*' "); // exclude hidden folders on Mac or Linux
    }

    if (excluded_folders_vector.size() != 0)
    {
        for (int i = 0; i < (int)excluded_folders_vector.size(); i++)
        {
            command.append("--exclude " + excluded_folders_vector[i] + " ");
        }
    }

    command.append(source_str);
    command.append(" ");
    command.append(destination_str);
    command.append("\"");

    QFile::remove(process_file);
    process_file_pos = 0;
    process_timer->start(); // Process timer used to actively update the text of the textbrowser


    QString mac_cmd = "cocoasudo " + command;
    proc->start(mac_cmd, QIODevice::ReadWrite); // First attempt to execute rsync for mac

    if (proc->error())
    {
        proc->close();
        QString linux_cmd = "gksudo " + command;
        proc->start(linux_cmd, QIODevice::ReadWrite); // Execute rsync for linux
    }

    if (!proc->waitForStarted())
    {
        return;
    }

    return;
}

void CougarBackup::rsyncConfigFolder()
{
    QString command;
    command.append("\"rsync ");
    command.append("-avzrPu --ignore-errors ");
    command.append(config_source);
    command.append(" ");
    command.append(destination_str);
    command.append("\"");

    QFile::remove(process_file);
    process_file_pos = 0;
    process_timer->start();


    QString mac_cmd = "cocoasudo " + command;
    proc->start(mac_cmd, QIODevice::ReadWrite); // First attempt to execute rsync for mac

    if (proc->error())
    {
        proc->close();
        QString linux_cmd = "gksudo " + command;
        proc->start(linux_cmd, QIODevice::ReadWrite); // Execute rsync for linux
    }

    if (!proc->waitForStarted())
    {
        return;
    }

    return;
}

void CougarBackup::executeFinished()
{
    /* This function is called when the rsync process is finished*/
    proc->close();
    process_timer->stop();
    update_output();

    if (is_windows && config_backedup == false && config_source.isEmpty() == false) // Checks if config has been backed up so not stuck in loop
    {
        ui->textBrowser_output->insertPlainText("\n\n------- BACKING UP CONFIG FOLDER -------\n\n");
        QThread::msleep(1000);
        config_backedup = true;
        rsyncConfigFolder(); //rsync config folder after initial backup only if windows OS
    }
    else
    {
        update_output(); // get rest of output after process is finished
        ui->pushButton_outFinished->setEnabled(true);
        ui->pushButton_outFinished->setVisible(true);
        //Create dialog box telling to check file sizes
        QMessageBox::information(this, "Backup Finished", "Please check file sizes now..", QMessageBox::Ok);
        return;
    }

    return;
}

void CougarBackup::update_output()
{
    if (ui->textBrowser_output->isVisible() == false) //make output window visible if not
    {
        ui->textBrowser_output->setEnabled(true);
        ui->textBrowser_output->setVisible(true);
    }

    QFile file(process_file);
    if (!file.open(QIODevice::ReadOnly)) return; //make sure file is opened

    if (file.size()>process_file_pos)
    {
      file.seek(process_file_pos);
      ui->textBrowser_output->moveCursor(QTextCursor::End);
      ui->textBrowser_output->insertPlainText(file.readAll());
      process_file_pos = file.pos();
    }

    file.close();
}

void CougarBackup::close_output()
{
    ui->pushButton_outFinished->setEnabled(false);
    ui->pushButton_outFinished->setVisible(false);
    ui->textBrowser_output->clear();
    ui->textBrowser_output->setVisible(false); //closes the output windows
    ui->textBrowser_output->setEnabled(false);
    config_backedup = false;
}

CougarBackup::~CougarBackup()
{
    delete ui;
    delete proc;
    delete process_timer;
}
