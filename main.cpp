#include "cougarbackup.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CougarBackup w;
    // Set up Window
    w.setFixedSize(770,340); // Sets the window's size to 400x279
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center()); // Centers the Window on Screen
    w.show();

    return a.exec();
}
