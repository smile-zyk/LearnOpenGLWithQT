#include "shadershow.h"
#include "ui_shadershow.h"

ShaderShow::ShaderShow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShaderShow)
{
    ui->setupUi(this);
}

ShaderShow::~ShaderShow()
{
    delete ui;
}

