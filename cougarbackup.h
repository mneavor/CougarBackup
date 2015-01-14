#ifndef COUGARBACKUP_H
#define COUGARBACKUP_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QDir>
#include <vector>
#include <string>
#include <QString>
#include <QProcess>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget> // Adds support for w.move, w.show in main.cpp

using namespace std;

namespace Ui {
class CougarBackup;
}

class CougarBackup : public QWidget
{
    Q_OBJECT

public:
    explicit CougarBackup(QWidget *parent = 0);
    ~CougarBackup();

private slots:
    void start_button();
    void cancel_button();
    void browse_button();
    void exclude_button();
    void config_button();
    void update_checkbox();
    void windows_checkbox();
    void name_textfield();
    void number_textfield();
    void calculate_destination();
    void get_config();
    void list_directories();
    void clear_excluded_folders();
    void executeRsync();
    void rsyncConfigFolder();
    void executeFinished();
    void update_output();
    void close_output();

private:
    Ui::CougarBackup *ui;
    QString name_text;
    QString number_text;
    QString source_str;
    QString destination_str;
    QString config_source;
    QString backup_stats;
    QString os_str;
    QString default_directory;
    string source_size;
    vector<QString> excluded_folders_vector;
    vector<QString> included_folders_vector;
    QString folder_list;
    bool is_windows;
    bool update_option;
    bool config_backedup;
    QProcess *proc;
    QString process_file;
    QTimer *process_timer;
    qint64 process_file_pos;
};

#endif // COUGARBACKUP_H
