#include "cougarbackup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CougarBackup w;
    w.show();

    return a.exec();
}
