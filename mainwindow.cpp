#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    config.setValue("destPath", "E:/QRtoTKS_2_0");
//    config.setValue("sourcePath", "Y:/QRtoTKS_2_0");

    Version versionCurrent(config.getValue("destPath") + "/.ver");
    Version versionSource(config.getValue("sourcePath") + "/.ver");

    if(config.contains("sourcePath"))
    {
        currentVer = versionCurrent.getVersion();
    }
    if(config.contains("destPath"))
    {
        sourseVer = versionSource.getVersion();
    }

    ui->labelVersion->setText("Текущая версия: " + currentVer + "\n Доступна версия: " + sourseVer);

}

MainWindow::~MainWindow()
{
    config.saveConfig();
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


void MainWindow::on_pushButton_clicked()
{
    hide();
    txtFiles* textFiles = new txtFiles;
    textFiles->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    hide();
    containers* ctnWindow = new containers;
    ctnWindow->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    QString updater = "./updater/updater.exe"; // Путь к программе
        QProcess::startDetached(updater);
}

