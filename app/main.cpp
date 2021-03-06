#include <memory>
#include <QApplication>
#include <QDarkThemePlugin.h>
#include "example.h"

int main(int argc, char *argv[])
{
    auto app = std::unique_ptr<QApplication>{ new QApplication{ argc, argv } };

    app->setOrganizationName("QDarkStyle (libqdark)");
    app->setApplicationName("QDarkStyle Example (libqdark)");

    auto darker = std::unique_ptr<libqdark::QDarkThemePlugin>{ new libqdark::QDarkThemePlugin{} };
    darker->changeLinksColor();

    auto mainWindow = std::unique_ptr<MainWindow>{ new MainWindow{} };

    darker->initialize(mainWindow.get());
    mainWindow->show();

    return app->exec();
}

