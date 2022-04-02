#ifndef SHADERSHOW_H
#define SHADERSHOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ShaderShow; }
QT_END_NAMESPACE

class ShaderShow : public QMainWindow
{
    Q_OBJECT

public:
    ShaderShow(QWidget *parent = nullptr);
    ~ShaderShow();

private:
    Ui::ShaderShow *ui;
};
#endif // SHADERSHOW_H
