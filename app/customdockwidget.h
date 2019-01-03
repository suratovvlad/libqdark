#pragma once

#include <memory>

#include <QDockWidget>

template <typename T>
class CustomDockWidget final: public QDockWidget
{
public:
    explicit CustomDockWidget(const QString& title, QWidget* parent = nullptr);

    CustomDockWidget(const CustomDockWidget&) = delete;
    CustomDockWidget(CustomDockWidget&&) = delete;

    CustomDockWidget& operator=(const CustomDockWidget&) = delete;
    CustomDockWidget& operator=(CustomDockWidget&&) = delete;

    virtual ~CustomDockWidget() override = default;

private:
    std::unique_ptr<T> ui;
};

template <typename T>
CustomDockWidget<T>::CustomDockWidget(const QString& title, QWidget *parent)
    : QDockWidget(title, parent)
    , ui(new T)
{
    this->setObjectName(title);
    ui->setupUi(this);
}
