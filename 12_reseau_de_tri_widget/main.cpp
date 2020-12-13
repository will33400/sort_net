#include "qstd.h"
using namespace qstd;

#include "mycanvas.h"
#include "mainwindow.h"
#include <QApplication>

#include "sortnet.h"


int main(int argc, char *argv[])
{
    QList<int> a_list = {7,10,5,1};

    QApplication a(argc, argv);

    MyCanvas w;

    w.SortnetInit(5);

    w.SortnetInject(a_list, 4);

    w.SortnetAppendConnector(0, 2);
    w.SortnetAppendConnector(1, 3);
    w.SortnetAppendConnector(0, 1);
    w.SortnetAppendConnector(2, 3);
    w.SortnetAppendConnector(1, 2);

    //w.SortnetParallel();

    w.show();
    return a.exec();
}
