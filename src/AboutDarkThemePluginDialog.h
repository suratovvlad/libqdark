#ifndef QDARKTHEMEPLUGIN_ABOUTDIALOG_H
#define QDARKTHEMEPLUGIN_ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDarkThemePluginDialog;
}

class AboutDarkThemePluginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDarkThemePluginDialog(QWidget *parent = 0);
	~AboutDarkThemePluginDialog();

private:
	Ui::AboutDarkThemePluginDialog *ui;
};

#endif // QDARKTHEMEPLUGIN_ABOUTDIALOG_H
