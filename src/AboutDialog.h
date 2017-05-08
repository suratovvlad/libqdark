#ifndef QDARKTHEMEPLUGIN_ABOUTDIALOG_H
#define QDARKTHEMEPLUGIN_ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private:
	Ui::AboutDialog *ui;
};

#endif // QDARKTHEMEPLUGIN_ABOUTDIALOG_H
