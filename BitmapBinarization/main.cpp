#include "BitmapBinarization.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BitmapBinarization w;
    w.show();
    return a.exec();
}
