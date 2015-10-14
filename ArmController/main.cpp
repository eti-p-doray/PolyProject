#include <QApplication>
#include <QDesktopWidget>

#include "Controller.h"

int main(int argc, char *argv[])
{
    std::cout << "bou" << std::endl;
    QApplication application(argc, argv);

    Controller controller;

    return application.exec();
}
