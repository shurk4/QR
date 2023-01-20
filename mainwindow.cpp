#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonStartBasic_clicked()
{
    hide();
    qrDialog* qrDiag = new qrDialog;
    qrDiag->exec();
}

void MainWindow::on_pushButtonExit_clicked()
{
    QCoreApplication::quit();
}

