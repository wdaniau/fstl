#include <QApplication>

#include "app.h"

int main(int argc, char *argv[])
{
    // Force C locale to force decimal point
    QLocale::setDefault(QLocale::c());

    QCoreApplication::setOrganizationName("fstl-e");
    QCoreApplication::setOrganizationDomain("https://github.com/wdaniau/fstl-e");
    QCoreApplication::setApplicationName("fstl-e");
    QCoreApplication::setApplicationVersion(FSTLE_VERSION);
    App a(argc, argv);

    return a.exec();
}
