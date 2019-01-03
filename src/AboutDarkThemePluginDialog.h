#pragma once

#include <memory>
#include <QDialog>

namespace Ui {
class AboutDarkThemePluginDialog;
}

namespace libqdark {

class AboutDarkThemePluginDialog final : public QDialog
{
    Q_OBJECT

public:

    explicit AboutDarkThemePluginDialog(QWidget* parent = nullptr);
    virtual ~AboutDarkThemePluginDialog() override;

    AboutDarkThemePluginDialog(const AboutDarkThemePluginDialog&) = delete;
    AboutDarkThemePluginDialog(AboutDarkThemePluginDialog&&) = delete;
    AboutDarkThemePluginDialog& operator=(const AboutDarkThemePluginDialog&) = delete;
    AboutDarkThemePluginDialog& operator=(AboutDarkThemePluginDialog&&) = delete;

private:
    std::unique_ptr<Ui::AboutDarkThemePluginDialog> ui;
};

} // namespace libqdark
