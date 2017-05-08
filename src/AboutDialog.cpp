#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

//	ui->nameLabel->setText(m_ui->nameLabel->text() + " " + KEEPASSX_VERSION);

	QFont nameLabelFont = ui->nameLabel->font();
	nameLabelFont.setBold(true);
	nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
	ui->nameLabel->setFont(nameLabelFont);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
