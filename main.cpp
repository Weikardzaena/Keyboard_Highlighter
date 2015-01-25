#include <QApplication>

#include "mainWindow.hpp"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWindow window;

    window.beginLoop();

    window.show();

    return app.exec();
}
