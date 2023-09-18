#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Version versionCurrent(config.getDestPath() + "/.ver");
    Version versionSource(config.getSourcePath() + "/.ver");

    if(versionCurrent.getVersion() < versionSource.getVersion())
    {
         ui->labelVersion->setText("Текущая версия: " + versionCurrent.getVersion() + "\n Доступна версия: " + versionSource.getVersion());
    }
    else
    {
        ui->labelVersion->hide();
        ui->pushButton_3->hide();
    }
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

void MainWindow::on_pushButton_clicked()
{
    hide();
    txtFiles* textFiles = new txtFiles(this);
    textFiles->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    hide();
    containers* ctnWindow = new containers;
    ctnWindow->exec();
}


void MainWindow::on_pushButton_3_clicked()
{
    QString updater = "./updater/updater.exe"; // Путь к программе
        QProcess::startDetached(updater);
        this->close();
}

