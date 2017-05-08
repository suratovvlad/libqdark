
#include <QApplication>
#include <QDarkThemePlugin.h>
#include "example.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("libqdarkExampleApp");

	QSharedPointer<QDarkThemePlugin> darker = QSharedPointer<QDarkThemePlugin>(new QDarkThemePlugin());
	darker->changeLinksColor();

	MainWindow mainWindow;
	mainWindow.show();

	darker->initialize();

    return app.exec();
}
