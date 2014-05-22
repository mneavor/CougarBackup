#ifndef COUGARBACKUP_H
#define COUGARBACKUP_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

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

private:
    Ui::CougarBackup *ui;
};

#endif // COUGARBACKUP_H
