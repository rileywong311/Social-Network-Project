#include "socialnetworkui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SocialNetworkUI w;
    w.show();
    return a.exec();
}
