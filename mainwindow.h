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

private slots:

    void on_pushButtonStartBasic_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    Config config;
    QString currentVer;
    QString sourseVer;
};

#endif // MAINWINDOW_H
