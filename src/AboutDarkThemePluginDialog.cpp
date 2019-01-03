#include "AboutDarkThemePluginDialog.h"
#include "ui_AboutDarkThemePluginDialog.h"

using namespace libqdark;

AboutDarkThemePluginDialog::AboutDarkThemePluginDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AboutDarkThemePluginDialog)
{
    ui->setupUi(this);

    //ui->nameLabel->setText(m_ui->nameLabel->text() + " " + KEEPASSX_VERSION);

    auto nameLabelFont = ui->nameLabel->font();
    nameLabelFont.setBold(true);
    nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
    ui->nameLabel->setFont(nameLabelFont);
}

AboutDarkThemePluginDialog::~AboutDarkThemePluginDialog() = default;
