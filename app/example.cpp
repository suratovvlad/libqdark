#include "example.h"
#include "ui_mw_menus.h"
#include "ui_dw_buttons.h"
#include "ui_dw_containers_no_tabs.h"
#include "ui_dw_containers_tabs.h"
#include "ui_dw_displays.h"
#include "ui_dw_inputs_fields.h"
#include "ui_dw_inputs_no_fields.h"
#include "ui_dw_views.h"
#include "ui_dw_widgets.h"

#include "customdockwidget.h"

template <typename T>
auto createDockWidget(const QString& title, const Qt::DockWidgetArea& dockWidgetArea, QMainWindow* mainWindow)
{
    auto dockWidget = new CustomDockWidget<T>{ title, mainWindow };
    mainWindow->addDockWidget(dockWidgetArea, dockWidget);
    return dockWidget;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("QDarkStyle v.0.4");

    auto dw_buttons = createDockWidget<Ui::DWButtons>("buttons", Qt::RightDockWidgetArea, this);
    auto dw_displays = createDockWidget<Ui::DWDisplays>("displays", Qt::RightDockWidgetArea, this);
    auto dw_inputs_fields = createDockWidget<Ui::DWInputFields>("_fields", Qt::RightDockWidgetArea, this);
    auto dw_inputs_no_fields = createDockWidget<Ui::DWInputNoFields>("inputs_no_fields", Qt::RightDockWidgetArea, this);

    auto dw_widgets = createDockWidget<Ui::DWWidgets>("widgets", Qt::LeftDockWidgetArea, this);
    auto dw_views = createDockWidget<Ui::DWViews>("views", Qt::LeftDockWidgetArea, this);
    auto dw_containers_tabs = createDockWidget<Ui::DWContainersTabs>("containers", Qt::LeftDockWidgetArea, this);
    auto dw_containers_no_tabs = createDockWidget<Ui::DWContainersNoTabs>("containers_no_tabs", Qt::LeftDockWidgetArea, this);

    // tabify right docks
    this->tabifyDockWidget(dw_buttons, dw_displays);
    this->tabifyDockWidget(dw_displays, dw_inputs_fields);
    this->tabifyDockWidget(dw_inputs_fields, dw_inputs_no_fields);

    // tabify left docks
    this->tabifyDockWidget(dw_containers_no_tabs, dw_containers_tabs);
    this->tabifyDockWidget(dw_containers_tabs, dw_widgets);
    this->tabifyDockWidget(dw_widgets, dw_views);
}

MainWindow::~MainWindow() = default;
