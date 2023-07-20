#include "quickStart.h"
#include "QrTable.h"
#include "ui_quickStart.h"

QuickStart::QuickStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickStart)
{
    ui->setupUi(this);
}

QuickStart::~QuickStart()
{
    delete ui;
}

void QuickStart::slotInvoice(Invoice in)
{
    std::cout << "Signal ctahed!\n";

    invoice.startRow = in.startRow;
    invoice.stopRow = in.stopRow;
    invoice.itemCol = in.itemCol;
    invoice.qtyCol = in.qtyCol;

    invoice.xlsArr = in.xlsArr;

    int rows = invoice.stopRow - invoice.startRow + 1;
    int cols = 2;

    ui->tableWidgetInvoice->setRowCount(rows);
    ui->tableWidgetInvoice->setColumnCount(cols);

    ui->tableWidgetInvoice->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                               << "D" << "E" << "F" << "G" << "H"
                                               << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр
    int row = 0;
    int goodsCount = 0;
    int stopRow = invoice.stopRow;
    for(int i = invoice.startRow; i <= invoice.stopRow; i++)
    {
        if(ctnNumber(invoice.xlsArr[i][invoice.itemCol]))
        {
            std::cout << "Deleted row: " << i << " with data: " << invoice.xlsArr[i][invoice.itemCol] << "\n";
            invoice.xlsArr.erase(invoice.xlsArr.begin() + i);
            i--;
            invoice.stopRow--;
            rows--;
            ui->tableWidgetInvoice->setRowCount(rows);
        }
        else
        {
            std::cout << "Row: " << i << " data: " << invoice.xlsArr[i][invoice.itemCol] << "\n";
            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(invoice.xlsArr[i][invoice.itemCol]));
            ui->tableWidgetInvoice->setItem(row, 0, item);

            QTableWidgetItem *qty = new QTableWidgetItem(QString::fromStdString(invoice.xlsArr[i][invoice.qtyCol]));
            ui->tableWidgetInvoice->setItem(row, 1, qty);

            goodsCount += std::stoi(invoice.xlsArr[i][invoice.qtyCol]);
            row++;
        }
    }
    // Размеры
    ui->tableWidgetInvoice->resizeRowsToContents();
    ui->tableWidgetInvoice->resizeColumnsToContents();


    //    ui->lineEditItemFirst->setText(QString::fromStdString(std::to_string(invoice.startRow)));
    ui->labelQtyCodes->setText(QString::fromStdString(std::to_string(rows)));
    ui->labelQtyGoods->setText(QString::fromStdString(std::to_string(goodsCount)));
}

void QuickStart::slotQr(QR in)
{
    try
        {
        std::cout << "Signal catched!\n";

        qr.startRow = in.startRow;
        qr.stopRow = in.stopRow;
        qr.itemCol = in.itemCol;
        qr.qrCol = in.qrCol;

        std::cout << "Start row = " << qr.startRow << "\n"
                  << "Stop row = " << qr.stopRow << "\n"
                  << "ItemCol = " << qr.itemCol << "\n"
                  << "qrCol = " << qr.qrCol << "\n";

        qr.xlsArr = in.xlsArr;

        int rows = qr.stopRow - qr.startRow;
        int cols = qr.xlsArr[0].size();

        ui->tableWidgetQr->setRowCount(rows);
        ui->tableWidgetQr->setColumnCount(cols);

        ui->tableWidgetQr->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                   << "D" << "E" << "F" << "G" << "H"
                                                   << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр
        std::vector<entry> entries;
        int row = 0;


        for(int i = qr.startRow; i < qr.stopRow; i++)
        {
            if(qr.xlsArr[i][qr.qrCol].size() < 32)
            {
                std::cout << "Deleted row: " << i << " with data: " << qr.xlsArr[i][qr.itemCol] << "\n";
                qr.xlsArr.erase(qr.xlsArr.begin() + i);
                i--;
                qr.stopRow--;
                rows--;
                ui->tableWidgetQr->setRowCount(rows);
            }
            else
            {
                QTableWidgetItem *itemCell = new QTableWidgetItem(QString::fromStdString(qr.xlsArr[i][qr.itemCol]));;
                ui->tableWidgetQr->setItem(row, 0, itemCell);

                QTableWidgetItem *qrCell = new QTableWidgetItem(QString::fromStdString(qr.xlsArr[i][qr.qrCol]));;
                ui->tableWidgetQr->setItem(row, 1, qrCell);
                row++;
            }
        }

        // Размеры
        ui->tableWidgetQr->resizeRowsToContents();
        ui->tableWidgetQr->resizeColumnsToContents();

        ui->labelQtyQrQr->setText(QString::fromStdString(std::to_string(rows)));
        }
        catch(std::exception &ex)
        {
            std::cerr << "slotQr exception:\n" << ex.what() << "\n";
        }
}

void QuickStart::slotCancel(QString in)
{
    std::cout << in.toStdString() << "\n";
}

void QuickStart::on_pushButtonExit_clicked()
{
    this->close();
}

void QuickStart::on_pushButtonStart_clicked()
{
    try
    {

        readyToWrite = new Result(qr,invoice);
        readyToWrite->getResult();


        // вывод предварительной таблицы
        int rows = readyToWrite->resultData.size();
        int cols = 4;

        ui->tableWidgetQr->setRowCount(rows);
        ui->tableWidgetQr->setColumnCount(cols);

        ui->tableWidgetQr->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                   << "D" << "E" << "F" << "G" << "H"
                                                   << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр

        for(int i = 0; i < rows; i++)
        {
            QTableWidgetItem *itemNum = new QTableWidgetItem(QString::fromStdString(readyToWrite->resultData[i].itemNum));
            ui->tableWidgetQr->setItem(i, 0, itemNum);

            QTableWidgetItem *itemCounter = new QTableWidgetItem(QString::fromStdString(readyToWrite->resultData[i].itemCounter));
            ui->tableWidgetQr->setItem(i, 1, itemCounter);

            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(readyToWrite->resultData[i].item));
            ui->tableWidgetQr->setItem(i, 2, item);

            QTableWidgetItem *qr = new QTableWidgetItem(QString::fromStdString(readyToWrite->resultData[i].idQr));
            ui->tableWidgetQr->setItem(i, 3, qr);
//            row++;
        }

        // вывод в консоль
        std::cout << "\n-----======= Pre result =========--------\n";
        for(auto i : readyToWrite->resultData)
        {
            std::cout << i.itemNum << "\t" << i.itemCounter << "\t" << i.item << "\t" << i.idQr << "\n";
        }
        // Размеры
        ui->tableWidgetQr->resizeRowsToContents();
        ui->tableWidgetQr->resizeColumnsToContents();

        ui->labelQtyQrQr->setText(QString::fromStdString(std::to_string(rows)));
    }
    catch(std::exception &ex)
    {
        QMessageBox::warning(this, "Start eror!", "Not enough qr codes!");
    }
}


void QuickStart::on_pushButtonSaveAs_clicked()
{
    QString resultPathTemp = QFileDialog::getSaveFileName(this, "Сохранить в...", QDir::homePath(), "result(*.xls)");
    std::string path = resultPathTemp.toStdString();
    readyToWrite->writeResult(&(path[0]));
    delete readyToWrite;
    this->close();
}

void QuickStart::on_pushButtonInvTable_clicked()
{
    TableSelect* tableSelect = new TableSelect;
    connect(tableSelect, &TableSelect::signalInvoice, this, &QuickStart::slotInvoice); // Получение класса из второго окна
    connect(tableSelect, &TableSelect::signalCancel, this, &QuickStart::slotCancel);
    tableSelect->exec();
}

void QuickStart::on_pushButtonQrTable_clicked()
{
    QrTable* qrTable = new QrTable;
    connect(qrTable, &QrTable::signalQr, this, &QuickStart::slotQr);
    qrTable->exec();
}
