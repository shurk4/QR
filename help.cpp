#include "help.h"
#include "ui_help.h"

help::help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);

    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);

    ui->listWidget->addItem("Простая выборка");

    ui->listWidget->addItem("Сопоставление по нескольким столбцам");

    ui->listWidget->addItem("Ручное сопоставление");

    ui->listWidget->addItem("При ошибке");
}

help::~help()
{
    delete ui;
}

void help::on_pushButton_clicked()
{
    this->close();
}

