#include "Paint.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Paint window;
    window.show();
    return app.exec();
}
