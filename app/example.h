#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <memory>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // EXAMPLE_H
