#include "AboutDarkThemePluginDialog.h"
#include "ui_AboutDarkThemePluginDialog.h"

#include <string>

using namespace libqdark;

AboutDarkThemePluginDialog::AboutDarkThemePluginDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AboutDarkThemePluginDialog)
{
    ui->setupUi(this);

    ui->nameLabel->setText(ui->nameLabel->text() + " " + LIBQDARK_VERSION);

    auto nameLabelFont = ui->nameLabel->font();
    nameLabelFont.setBold(true);
    nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
    ui->nameLabel->setFont(nameLabelFont);

    QString commitHash;
    if (!QString(GIT_VERSION).isEmpty()) {
        commitHash = GIT_VERSION;
    }

    if (!commitHash.isEmpty()) {
        QString labelText = tr("Revision").append(": ").append(commitHash);
        ui->label_git->setText(labelText);
    }

    auto libs = QString("%1\n- Qt %2")
            .arg(ui->label_libs->text())
            .arg(QString::fromLocal8Bit(qVersion()));
    ui->label_libs->setText(libs);
}

AboutDarkThemePluginDialog::~AboutDarkThemePluginDialog() = default;
