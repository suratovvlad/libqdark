#pragma once

#include <memory>
#include "libqdark_global.h"

namespace libqdark {

/**
 * @brief The QDarkThemePlugin class
 * This class implements a plugin for qt applications.
 * This plugin makes the application darker.
 */
class LIBQDARKSHARED_EXPORT QDarkThemePlugin final
{
    class QDarkThemePluginPrivate;
    Q_DECLARE_PRIVATE(QDarkThemePlugin)

public:
    /**
     * @brief QDarkThemePlugin
     * The constructor of this class.
     */
    QDarkThemePlugin();

    /**
     * @brief ~QDarkThemePlugin
     * The virtual destructor of this class.
     */
    virtual ~QDarkThemePlugin();

    QDarkThemePlugin(const QDarkThemePlugin&) = delete;
    QDarkThemePlugin(QDarkThemePlugin&&) = delete;
    QDarkThemePlugin& operator=(const QDarkThemePlugin&) = delete;
    QDarkThemePlugin& operator=(QDarkThemePlugin&&) = delete;

    /**
     * @brief changeLinksColor
     * This method changes (<a>) color for links in the entire application.
     * It uses the QPalette, so, it needs to be called BEFORE any "show" methods.
     */
    void changeLinksColor();

    /**
     * @brief initialize
     * This method initialize base functionality of that plugin: an additional
     * menu with the "Dark theme" action and the "About" action.
     * It uses the instance of QMainWindow, so, it needs to be called AFTER
     * the QMainWindow instantiation.
     */
    void initialize();

private:
    /**
     * @brief d_ptr
     * The pointer to the private realization (PIMPL).
     */
    std::unique_ptr<QDarkThemePluginPrivate> const d_ptr;
};

} // namespace libqdark
