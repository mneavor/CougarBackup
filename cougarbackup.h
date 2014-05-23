#ifndef COUGARBACKUP_H
#define COUGARBACKUP_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QComboBox>
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
    void os_combobox();
    void name_textfield();
    void number_textfield();
    void calculate_destination();
    void get_config();
    qlonglong compute_source_size(const QString &path);
    void update_backup_stats();
private:
    Ui::CougarBackup *ui;
    QString name_text;
    QString number_text;
    QString source_str;
    QString destination_str;
    QString backup_stats;
    QString os_str;
    QString default_directory;
    qlonglong source_size;
};

#endif // COUGARBACKUP_H
