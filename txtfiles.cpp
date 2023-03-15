#include "txtfiles.h"
#include "qdatetime.h"
#include "ui_txtfiles.h"

txtFiles::txtFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::txtFiles)
{
    ui->setupUi(this);

    this->resize(1280, 720);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);
}

txtFiles::~txtFiles()
{
    delete ui;
}

void txtFiles::showTable_1(std::vector<std::vector<std::string>> &inTab)
{
        QString labelTabString = QString::number(currentTab + 1);
        QString labelTabsString = QString::number(converter.invoiceXls.size());

        ui->labelTab->setText(labelTabString);
        ui->labelTabs->setText(labelTabsString);

            int rows;
            if(inTab.size() > 10000)
            {
                 rows = 10000;
            }
            else
            {
                rows = inTab.size();
            }

            int cols = inTab[0].size();

            ui->tableWidget_1->setRowCount(rows);
            ui->tableWidget_1->setColumnCount(cols);

            ui->tableWidget_1->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                       << "D" << "E" << "F" << "G" << "H"
                                                       << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр

            for(int row = 0; row < inTab.size(); row++)
            {
                for(int col = 0; col < inTab[row].size(); col++)
                {
                    if(inTab[row].size() > cols) cols = inTab[row].size();
                    QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(inTab[row][col]));
                    ui->tableWidget_1->setItem(row, col, tbl);
                }
            }

            // Размеры
            ui->tableWidget_1->resizeRowsToContents();
            ui->tableWidget_1->resizeColumnsToContents();
    // Вывести данные в таблицу
}

void txtFiles::showTable_2(std::vector<std::vector<std::string>> &table)
{
    // Вывести данные в таблицу

            int rows = table.size();
            int cols = table[0].size();

            ui->tableWidget_2->setRowCount(rows);
            ui->tableWidget_2->setColumnCount(cols);

            ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                       << "D" << "E" << "F" << "G" << "H"
                                                       << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр

            for(int row = 0; row < table.size(); row++)
            {
                for(int col = 0; col < table[row].size(); col++)
                {
                    if(table[row].size() > cols) cols = table[row].size();
                    QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(table[row][col]));
                    ui->tableWidget_2->setItem(row, col, tbl);
                }
            }

            // Размеры
            ui->tableWidget_2->resizeRowsToContents();
            ui->tableWidget_2->resizeColumnsToContents();
            // Вывести данные в таблицу
}

void txtFiles::on_pushButtonInv_clicked()
{
    converter.clearInvoiceData();
    QString invPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл инвойса", lastPath.absolutePath(), "*.xls *.xlsx");
    if(invPathTemp.isEmpty())
    {
        ui->labelInv->setText("Выберите файл!");
    }
    else
    {
        lastPath = invPathTemp;

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists()) QDir().mkdir("temp"); // Создание папки temp

        // определение разрешения
        if(invPathTemp.right(4) == "xlsx" || invPathTemp.right(4) == "XLSX")
        {
            ext = ".xlsx";
        }
        else
        {
            ext = ".xls";
        }

        invoiceFileName = "temp/inv" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(invPathTemp, invoiceFileName);

        if(!copySucces) QMessageBox::information(this, "Открытие файла", "Что-то пошло не так!");

        if(ext == ".xlsx")
        {
            std::string path = invoiceFileName.toStdString();
            converter.readXlsX(path, converter.invoiceXls, converter.invoiceSheetNames);
        }
        else
        {
            std::wstring path = invoiceFileName.toStdWString();
            converter.readXls(path, converter.invoiceXls, converter.invoiceSheetNames);
        }

        ui->labelInv->setText(invPathTemp);

        QFile::remove(invoiceFileName);

        // !!! ТАБЛИЦА !!!
        converter.invoiceSheetSettings.resize(converter.invoiceXls.size());
        showTable_1(converter.invoiceXls[currentTab]);

        invoiceShowed = true;
        ui->pushButtonShow->setText("Результат");
    }
}

void txtFiles::on_pushButtonDown_clicked()
{
    if(currentTab > 0 && converter.invoiceXls.size() > 0)
    {
        currentTab--;
        showTable_1(converter.invoiceXls[currentTab]);
    }
}

void txtFiles::on_pushButtonUp_clicked()
{
    if(currentTab < converter.invoiceXls.size() - 1 && converter.invoiceXls.size() > 0)
    {
        currentTab++;
        showTable_1(converter.invoiceXls[currentTab]);
    }
}

void txtFiles::on_pushButtonTxt_clicked()
{
    QStringList fileNames;

    docsData.clear();
    data.clear();
    ui->tableWidget_2->clear();
    ui->listWidget->clear();

    // Получение списка файлов
    fileNames = QFileDialog::getOpenFileNames(nullptr, "Выбрать текстовые файлы", lastPath.absolutePath() , "Текстовые файлы(*.txt)");

    lastPath = fileNames[0];

    if(!fileNames.isEmpty())
    {
        // Получение данных из списка файлов
        for (auto xfile : fileNames)
        {
            QFile file (xfile);
            // Получение имени файла
            QFileInfo info(file.fileName());
            FileData tempData;
            tempData.name = info.fileName();

            tempData.status = NEW;
            docsData.push_back(tempData);

            // Загрузка данных из файла в поток
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);

            // Обработка данных
            std::vector<std::vector<std::string>> doc; // Данные из одного документа
            while (!in.atEnd())
            {
                std::vector<std::string> col;
                QString line = in.readLine();
                col.push_back(line.toStdString());
                doc.push_back(col);
            }
            data.push_back(doc);
        }

        showTable_2(data[currentDoc]);
        ui->labelCodesInDoc->setText(QString::number(data[currentDoc].size()));

        ui->listWidget->clear();

        for(int i = 0; i < docsData.size(); i++)
        {
            ui->listWidget->addItem(docsData[i].name);
        }

        ui->listWidget->setCurrentRow(0);

        ui->labelFilesNum->setText(QString::number(fileNames.size()));
    }
}

void txtFiles::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentDoc = ui->listWidget->currentRow();
    showTable_2(data[currentDoc]);
    ui->labelCodesInDoc->setText(QString::number(data[currentDoc].size()));
}


void txtFiles::on_pushButtonAddQr_clicked()
{
    if(docsData[currentDoc].status == ADDED)
    {
        QMessageBox::information(this, "!?", "Этот документ уже добавлен!");
    }
    else
    {
        itemNum++;
        int numGroup = 0;
        docsData[currentDoc].status = ADDED;

        for(int i = 0; i < data[currentDoc].size(); i++)
        {
            serial++;
            numGroup++;

            std::vector<std::string> tempResultRow;

            // Добавление порядкового номера
            tempResultRow.push_back(std::to_string(serial));

            // Добавление "№товара"
            tempResultRow.push_back(std::to_string(itemNum));


            // Добавление "№группы"
            tempResultRow.push_back(std::to_string(numGroup));

            // добавление QR "Идентификатор"
            std::string qrCut;
            std::string qr = data[currentDoc][i][0];
            for(int k = 0; k < 31; k++)
            {
                qrCut += qr[k];
            }
            tempResultRow.push_back(qrCut);

            // добавление "Код вида"
            tempResultRow.push_back(std::to_string(301));

            // добавление "Уровень маркировки"
            tempResultRow.push_back(std::to_string(0));

            // добавление Item code
            QString itemCode;

            for(int j = 0; j < docsData[currentDoc].name.size() - 4; j++)
            {
                if(docsData[currentDoc].name[j] > 122)
                {
                    continue;
                }
                else
                {
                    itemCode.push_back(docsData[currentDoc].name[j]);
                }
            }
            tempResultRow.push_back(itemCode.toStdString());

            // Добавление полученной строки в таблицу
            converter.result.push_back(tempResultRow);

            ui->listWidget->currentItem()->setBackgroundColor("#a1ff9f");
        }
    }

    showTable_1(converter.result);
    ui->labelResultItems->setText(QString::number(itemNum));
    ui->labelResultCodes->setText(QString::number(converter.result.size()));
    ui->pushButtonShow->setText("Инвойс");
    invoiceShowed = false;
}

void txtFiles::on_pushButtonSave_clicked()
{
    if(converter.result.size() > 0)
    {
        lastPath = QFileInfo(lastPath.path()).absolutePath() + "/result.xls";

        QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить результат"), lastPath.absolutePath(), tr("Таблица xls (*.xls)"));
        if (fileName != "")
        {
            try
            {
                std::wstring path = fileName.toStdWString();
                converter.saveResult(path);
                QMessageBox::information(this, "!", "Файл сохранён!");
            }
            catch (...)
            {
                QMessageBox::critical(this, "!", "Не удалось сохранить файл!");
            }
            converter.clear();
        }
    }
    else
    {
        QMessageBox::information(this, "!", "Нечего сохранять!");
    }
}


void txtFiles::on_pushButtonShow_clicked()
{
    if(invoiceShowed && converter.result.size() > 0)
    {
        showTable_1(converter.result);
        ui->pushButtonShow->setText("Инвойс");
        invoiceShowed = false;
    }
    else if(!invoiceShowed && converter.invoiceXls.size() > 0)
    {
        showTable_1(converter.invoiceXls[currentTab]);
        ui->pushButtonShow->setText("Результат");
        invoiceShowed = true;
    }
}

