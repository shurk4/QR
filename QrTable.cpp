#include "QrTable.h"
#include "ui_QrTable.h"

QrTable::QrTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QrTable)
{
    ui->setupUi(this);
}

QrTable::~QrTable()
{
    delete ui;
}

void QrTable::on_pushButtonOpenFile_clicked()
{
    QString invPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл QR", QDir::homePath(), "*.xls");
    if(invPathTemp.isEmpty())
    {
        ui->lineEditPath->setText("Выберите файл!");
    }
    else
    {
        ui->lineEditPath->setText(invPathTemp);
        path = invPathTemp.toStdString();

        qr.read(path);

//        // !!! ТАБЛИЦА !!!

        int rows = qr.xlsArr.size();
        int cols = qr.xlsArr[0].size();

        ui->tableWidget->setRowCount(rows);
        ui->tableWidget->setColumnCount(cols);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                   << "D" << "E" << "F" << "G" << "H"
                                                   << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(qr.xlsArr[i][j]));;
                ui->tableWidget->setItem(i, j, tbl);
            }
        }
        // Размеры
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->resizeColumnsToContents();

        // развернуть окно
        this->showMaximized();
        // !!! ТАБЛИЦА !!!
    }
}


void QrTable::on_pushButtonQrCol_clicked()
{
    ui->pushButtonQrCol->setCheckable(true);
    ui->pushButtonQrCol->setDown(true);
}


void QrTable::on_pushButtonItemCol_clicked()
{
    ui->pushButtonItemCol->setCheckable(true);
    ui->pushButtonItemCol->setDown(true);
}

void QrTable::on_pushButtonFirstRow_clicked()
{
    ui->pushButtonFirstRow->setCheckable(true);
    ui->pushButtonFirstRow->setDown(true);
}

void QrTable::on_tableWidget_cellClicked(int row, int column)
{
    if(ui->pushButtonQrCol->isCheckable())
    {
        qr.qrCol = column;

        ui->lineEditQrCol->setText(QString::fromStdString(std::to_string(column)));
        ui->pushButtonQrCol->setCheckable(false);
    }

    if(ui->pushButtonItemCol->isCheckable())
    {
        qr.itemCol = column;

        ui->lineEditItemCol->setText(QString::fromStdString(std::to_string(column)));
        ui->pushButtonItemCol->setCheckable(false);
    }

    if(ui->pushButtonFirstRow->isCheckable())
    {
        qr.startRow = row;
        qr.stopRow = qr.xlsArr.size();

        ui->lineEditQrRow->setText(QString::fromStdString(std::to_string(row)));
        ui->lineEditQrQty->setText(QString::fromStdString(std::to_string(qr.stopRow - 1)));
        ui->pushButtonFirstRow->setCheckable(false);
    }
}

void QrTable::on_pushButtonCancel_clicked()
{
    this->close();
}


void QrTable::on_pushButtonOk_clicked()
{
    emit signalQr(qr);
    this->close();
}

