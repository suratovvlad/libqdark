#include "AboutDarkThemePluginDialog.h"
#include "ui_AboutDarkThemePluginDialog.h"

AboutDarkThemePluginDialog::AboutDarkThemePluginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDarkThemePluginDialog)
{
	ui->setupUi(this);

//	ui->nameLabel->setText(m_ui->nameLabel->text() + " " + KEEPASSX_VERSION);

	QFont nameLabelFont = ui->nameLabel->font();
	nameLabelFont.setBold(true);
	nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
	ui->nameLabel->setFont(nameLabelFont);
}

AboutDarkThemePluginDialog::~AboutDarkThemePluginDialog()
{
	delete ui;
}
