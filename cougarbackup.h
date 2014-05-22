#ifndef COUGARBACKUP_H
#define COUGARBACKUP_H

#include <QWidget>

namespace Ui {
class CougarBackup;
}

class CougarBackup : public QWidget
{
    Q_OBJECT

public:
    explicit CougarBackup(QWidget *parent = 0);
    ~CougarBackup();

private:
    Ui::CougarBackup *ui;
};

#endif // COUGARBACKUP_H
