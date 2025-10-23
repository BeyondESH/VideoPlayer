#define SDL_MAIN_HANDLED
#include "xvideoviewtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XVideoViewTest w;
    w.show();
    return a.exec();
}
