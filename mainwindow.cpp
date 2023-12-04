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

    optimizeLogs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString _log)
{
    if(ui->checkBoxLog->isChecked())
    {
        QFile logFile(logFileName);
        if(logFile.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << _log;
            QTextStream text(&logFile);
            text << _log << "\n";
            logFile.close();
        }
        else
        {
            qDebug() << "Файл логов не открыт";
        }
    }
}

void MainWindow::optimizeLogs()
{
    if(!QDir("Logs").exists())
    {
        QDir().mkdir("Logs");
    }
    deleteOldLogs();
}

void MainWindow::deleteOldLogs()
{
    log("Удаление старых логов: ");
    QFileInfoList filesList = QDir("Logs").entryInfoList();
    for(const auto &i : filesList)
    {
        if(i.birthTime().daysTo(QDateTime::currentDateTime()) > 7)
        {
            QFile(i.absoluteFilePath()).remove();
            log(i.absoluteFilePath() + " - удалён.");
        }
    }
}

void MainWindow::on_pushButtonStartBasic_clicked()
{
    hide();
    qrDialog* qrDiag = new qrDialog;
    qrDiag->exec();
}

void MainWindow::on_pushButton_clicked()
{
    hide();
    txtFiles* textFiles = new txtFiles;
    connect(textFiles, &txtFiles::log, this, &MainWindow::logs);
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

void MainWindow::logs(QString _log)
{
    log(_log);
}

void MainWindow::on_pushButtonExit_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_checkBoxLog_stateChanged(int arg1)
{
    if(arg1)
    {
        logFileName = "./Logs/" + QDateTime::currentDateTime().toString("yy_MM_dd_hhmmss") + ".txt";
        qDebug() << "Log file name: " << logFileName;
    }
}

