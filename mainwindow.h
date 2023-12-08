#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QProcess>
#include <iostream>
#include "qrDialog.h"
#include "txtfiles.h"
#include "containers.h"
#include "config.h"
#include "version.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void log(QString _log);
    void optimizeLogs();
    void deleteOldLogs();

private slots:

    void on_pushButtonStartBasic_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButtonStartTxt_clicked();

    void on_pushButtonCont_clicked();

    void on_pushButtonUpdate_clicked();

    void on_pushButtonLog_clicked();

private:
    Ui::MainWindow *ui;

    Config config;
    QString currentVer;
    QString sourseVer;

    QString logFileName;

    void setStyle();

public slots:
    void logs(QString _log);
};

#endif // MAINWINDOW_H
