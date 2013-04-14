#include "MainWindow.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);
    a.setFont(font);

    MainWindow w;
    w.show();
    
    return a.exec();
}
