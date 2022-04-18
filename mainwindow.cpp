#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // set stylesheet
    QFile f(":/qdarkstyle/dark/style.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    setFixedSize(820,840);

    {
        // connect debugView
        connect(ui->renderWidget,&LearnOpenGLWidget::debugMessage,this,[this](QString msg)
        {
            if(ui->debugView->count() == 20)ui->debugView->takeItem(0);
            ui->debugView->addItem(msg);
            ui->debugView->setCurrentRow(ui->debugView->count()-1);
        });
        // connect startBtn
        connect(ui->startBtn,&QPushButton::clicked,this,[this]()
        {
            ui->renderWidget->state = LearnOpenGLWidget::RUN;
        });
        // connect pauseBtn
        connect(ui->pauseBtn,&QPushButton::clicked,this,[this]()
        {
            ui->renderWidget->state = LearnOpenGLWidget::PAUSE;
        });
        // connect clearBtn
        connect(ui->clearBtn,&QPushButton::clicked,this,[this]()
        {
            ui->debugView->clear();
        });
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,"select model file","","OBJ(*.obj);;FBX(*.fbx)");
    if(!filePath.isEmpty())
        ui->renderWidget->LoadModel(filePath);
}

