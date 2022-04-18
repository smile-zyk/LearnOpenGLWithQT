#ifndef SHADERSHOW_H
#define SHADERSHOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "learnopenglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // SHADERSHOW_H
