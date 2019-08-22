#include "simulation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Simulation *simulation;
    simulation = new Simulation();
    simulation->show();

    return a.exec();
}
