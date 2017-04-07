#ifndef QDARKTHEMEPLUGIN_H
#define QDARKTHEMEPLUGIN_H

#include "libqdark_global.h"
#include <QScopedPointer>

class QDarkThemePluginPrivate;

class LIBQDARKSHARED_EXPORT QDarkThemePlugin
{
    Q_DECLARE_PRIVATE(QDarkThemePlugin)

public:
    QDarkThemePlugin();
    virtual ~QDarkThemePlugin();

    void initialize() const;

private:
    QScopedPointer<QDarkThemePluginPrivate> const d_ptr;
};

#endif // QDARKTHEMEPLUGIN_H
