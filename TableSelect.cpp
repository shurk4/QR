#include "TableSelect.h"
#include "ui_TableSelect.h"
#include "qrCodes.h"

TableSelect::TableSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableSelect)
{
    ui->setupUi(this);
}

TableSelect::~TableSelect()
{
    delete ui;
}

void TableSelect::on_pushButtonOpenFile_clicked()
{
    QString invPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл инвойса", QDir::homePath(), "*.xls");
    if(invPathTemp.isEmpty())
    {
        ui->lineEditPath->setText("Выберите файл!");
    }
    else
    {
        ui->lineEditPath->setText(invPathTemp);
        path = invPathTemp.toStdString();

        invoice.read(path);

//        // !!! ТАБЛИЦА !!!

        int rows = invoice.xlsArr.size();
        int cols = invoice.xlsArr[0].size();

        ui->tableWidget->setRowCount(rows);
        ui->tableWidget->setColumnCount(cols);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                   << "D" << "E" << "F" << "G" << "H"
                                                   << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(invoice.xlsArr[i][j]));;
//                tbl->setFlags(Qt::ItemIsEditable); // Не редактируется
//                tbl->setFlags(Qt::ItemIsUserCheckable); // Не выделяется
                ui->tableWidget->setItem(i, j, tbl);
            }
        }
        // Размеры
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->resizeColumnsToContents();

        // развернуть окно

//        this->setGeometry(QScreen::availableGeometry(); // Разобраться с QScreen
        this->showMaximized();
        // !!! ТАБЛИЦА !!!
    }
}


void TableSelect::on_pushButtonOk_clicked()
{
    std::cout << "Signal sended!\n";
    emit signalInvoice(invoice);

    this->close();
}


void TableSelect::on_pushButtonCancel_clicked()
{
    std::cout << "Cancel!\n";
    emit signalCancel("Operation canceled!\n");
    this->close();
}

// Выбор Старт Итем

void TableSelect::on_pushButtonItemFirst_clicked()
{
    ui->pushButtonItemFirst->setCheckable(true);
    ui->pushButtonItemFirst->setDown(true);
}


void TableSelect::on_tableWidget_cellClicked(int row, int column)
{
    if(ui->pushButtonItemFirst->isCheckable())
    {
        invoice.startRow = row;
        invoice.itemCol = column;

        ui->lineEditItemFirst->setText(QString::fromStdString(std::to_string(row)));
        ui->lineEditItemCol->setText(QString::fromStdString(std::to_string(column)));
        ui->pushButtonItemFirst->setCheckable(false);
    }

    if(ui->pushButtonItemLast->isCheckable())
    {
        invoice.stopRow = row;
        invoice.itemCol = column;

        ui->lineEditItemLast->setText(QString::fromStdString(std::to_string(row)));
        ui->lineEditItemCol->setText(QString::fromStdString(std::to_string(column)));
        ui->pushButtonItemLast->setCheckable(false);
    }

    if(ui->pushButtonQtyCol->isCheckable())
    {
        invoice.qtyCol = column;

        ui->lineEditQtyCol->setText(QString::fromStdString(std::to_string(column)));
        ui->pushButtonQtyCol->setCheckable(false);
    }
}

// Выбор энд итем

void TableSelect::on_pushButtonItemLast_clicked()
{
    ui->pushButtonItemLast->setCheckable(true);
    ui->pushButtonItemLast->setDown(true);
}


void TableSelect::on_pushButtonQtyCol_clicked()
{
    ui->pushButtonQtyCol->setCheckable(true);
    ui->pushButtonQtyCol->setDown(true);
}

