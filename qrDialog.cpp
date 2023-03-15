#include "qrDialog.h"

qrDialog::qrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrDialog)
{
    ui->setupUi(this);

    this->resize(1280, 720);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);


    helpWindow = new help(this); // Указатель создан в хедере для одновременного использования окон
}

qrDialog::~qrDialog()
{
    delete ui;
}

// БУЛКИ

bool qrDialog::filesOk()
{
    return converter.invoiceXls.size() > 0 && converter.qrXls.size() > 0;
}

bool qrDialog::compared(std::string a, std::string b)
{
    return a == b;
}

bool qrDialog::emptyCell(std::string &str)
{
    return str.size() == 0 || str[0] == '\0';
}

bool qrDialog::itQty(std::string &str)
{
    if(emptyCell(str)) return false;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool qrDialog::tabAlreadyAdded(int tab)
{
    if(addedQrTabs.empty()) return false;
    for(int i = 0; i < addedQrTabs.size(); i++)
    {
        if(addedQrTabs[i] == tab) return true;
    }
    return false;
}

void qrDialog::qrNewFileClear()
{
    addedQrTabs.clear();
    selectedQrCols.clear();
    compares.clear();

    ui->labelColQR->clear();
    ui->listWidget->clear();
}

void qrDialog::qrClear()
{
    qrNewFileClear();
}
// БУЛКИ

// ЛЕВАЯ ТАБЛИЦА
//Выбор первой ячейки количества
void qrDialog::on_pushButtonFirstQty_clicked()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget->selectionModel()->currentIndex().column();
        int row = ui->tableWidget->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].qtyCol = col;
        converter.invoiceSheetSettings[currentTab].startRow = row;

        ui->labelColQty->setText(QString::fromStdString(Extras::IntToSymbol(col)));
        ui->labelFirstQty->setText(QString::number(row + 1));
    }
}
//Выбор первой ячейки количества

//Выбор последней ячейки количества
void qrDialog::on_pushButtonLastQty_clicked()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int row = ui->tableWidget->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].stopRow = row;
        ui->labelLastQty->setText(QString::number(row + 1));
    }
}
//Выбор последней ячейки количества

// Открыть файл
void qrDialog::on_pushButtonOpenFile_clicked()
{
    converter.clearInvoiceData(); // Надо ли делать выборку из нескольких инвойсов

    QString invPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл инвойса", lastPath.absolutePath(), "*.xls *.xlsx");
    if(invPathTemp.isEmpty())
    {
        ui->labelPath->setText("Выберите файл!");
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

        ui->labelPath->setText(QFileInfo(invPathTemp).fileName());

        QFile::remove(invoiceFileName);

        // !!! ТАБЛИЦА !!!
        currentTab = 0;
        converter.invoiceSheetSettings.resize(converter.invoiceXls.size());

        showTab(converter.invoiceXls[currentTab]);
        ui->labelTabName1->setText(converter.invoiceSheetNames[currentTab]);
    }
}
// Открыть файл

// Переключение вкладок
void qrDialog::on_pushButtonTabUp_clicked()
{
    if(currentTab < converter.invoiceXls.size() - 1 && converter.invoiceXls.size() > 0)
    {
        currentTab++;
        showInvoice = true;
        ui->pushButtonShowInvoice->setText("Товары");

        showTab(converter.invoiceXls[currentTab]);        
        ui->labelTabName1->setText(converter.invoiceSheetNames[currentTab]);
    }
}

void qrDialog::on_pushButtonTabDown_clicked()
{
    if(currentTab > 0 && converter.invoiceXls.size() > 0)
    {
        currentTab--;
        showInvoice = true;
        ui->pushButtonShowInvoice->setText("Товары");

        showTab(converter.invoiceXls[currentTab]);
        ui->labelTabName1->setText(converter.invoiceSheetNames[currentTab]);
    }
}
// Переключение вкладок

// Вывести данные в таблицу
void qrDialog::showTab(std::vector<std::vector<std::string>> &inTab)
{
    QString labelTabString = QString::number(currentTab + 1);
    QString labelTabsString = QString::number(converter.invoiceXls.size());

    ui->labelTab->setText(labelTabString);
    ui->labelTabs->setText(labelTabsString);

    QVector<QVector<QString>> temp(Extras::toQvecConvert(inTab));
    Extras::showTable(temp, ui->tableWidget);
}
// Вывести данные в таблицу

// Сбросить все данные
void qrDialog::on_pushButtonResetInvoice_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->labelPath->clear();
    ui->labelTab->setText("0");
    ui->labelTabs->setText("0");
    ui->labelFirstQty->setText("");
    ui->labelItemsQty->setText("");
    ui->labelColQty->setText("");
    ui->labelItemsQty->setText("");
    ui->labelPositions->setText("");
    ui->labelLastQty->setText("-");
    compares.clear();
    converter.clearInvoiceData();
    converter.clearArt();

    addedToInv = false;

    QMessageBox::information(this, "!", "Индексы инвойса очищены");
}
// Сбросить все данные

//Показать Инвойс
void qrDialog::on_pushButtonShowInvoice_clicked()
{
    if(converter.invoiceXls.size() > 0 && converter.invoiceResult.size() > 0)
    {
        if(showInvoice)
        {
            showInvoice = false;
            showTab(converter.invoiceResult);
            ui->pushButtonShowInvoice->setText("Инвойс");
        }
        else
        {
            showInvoice = true;
            showTab(converter.invoiceXls[currentTab]);
            ui->pushButtonShowInvoice->setText("Товары");
        }
    }
}
//Показать Инвойс

// Анализируем данные в инвойс по заданным параметрам
void qrDialog::on_pushButtonAnalyzeInvoice_clicked()
{
    if(!converter.invoiceSheetSettings.empty()
            && (converter.invoiceSheetSettings[currentTab].qtyCol > 0
            || converter.invoiceSheetSettings[currentTab].qtyCol < converter.invoiceXls[currentTab][0].size()))
    {
        converter.invoiceResult.clear();
        int CellSize;
        int itemsQty = 0;
        int colsNum;
        int stopRow = converter.invoiceXls[currentTab].size();

        if(converter.invoiceSheetSettings[currentTab].stopRow > -1
                &&converter.invoiceSheetSettings[currentTab].stopRow > converter.invoiceSheetSettings[currentTab].startRow)
        {
            stopRow = converter.invoiceSheetSettings[currentTab].stopRow + 1;
        }
        std::vector<int> notEmptyCells;

        // поиск строк товаров

        for(int row = converter.invoiceSheetSettings[currentTab].startRow; row < stopRow; row++)
        {
        std::vector<std::string> tempRow;
        bool notEmpty = true;

            if(row == converter.invoiceSheetSettings[currentTab].startRow)
            {
                for(int col = 0; col < converter.invoiceXls[currentTab][row].size(); col++)
                {
                    if(!emptyCell(converter.invoiceXls[currentTab][row][col]))
                    {
                        std::string cellData = converter.invoiceXls[currentTab][row][col];
                        notEmptyCells.push_back(col);
                        tempRow.push_back(cellData);
                        if(col == converter.invoiceSheetSettings[currentTab].qtyCol)
                        {
                            swap(tempRow[tempRow.size() - 1], tempRow[0]);
                        }
                    }
                }
                colsNum = tempRow.size();
            }
            else
            {
                int emptyRowsCounter = 0;
                for(int i = 0; i < notEmptyCells.size(); i++)
                {
                    tempRow.resize(colsNum);
                    std::string cellData = converter.invoiceXls[currentTab][row][notEmptyCells[i]];

                    if(emptyCell(cellData))
                    {
                        emptyRowsCounter++;
                    }

                    if(notEmptyCells[i] == converter.invoiceSheetSettings[currentTab].qtyCol && !itQty(cellData)) notEmpty = false;

                    tempRow[i] = cellData;

                    if(notEmptyCells[i] == converter.invoiceSheetSettings[currentTab].qtyCol)
                    {
                        swap(tempRow[i], tempRow[0]);
                    }
                }
                if(emptyRowsCounter > notEmptyCells.size() / 2) notEmpty = false;
            }

            if(notEmpty)
            {
                converter.invoiceResult.push_back(tempRow);
            }
        }
        //поиск строк товаров

        // подсчёт товаров
        for(int i = 0; i < converter.invoiceResult.size(); i++)
        {
            if(itQty(converter.invoiceResult[i][0]))
            {
                itemsQty += std::stoi(converter.invoiceResult[i][0]);
            }
        }
        // подсчёт товаров

        ui->labelPositions->setText(QString::number(converter.invoiceResult.size()));
        ui->labelItemsQty->setText(QString::number(itemsQty));
        ui->pushButtonShowInvoice->setText("Инвойс");
        showTab(converter.invoiceResult);
        analyzed = true;
    }
}

void qrDialog::on_pushButton_clicked()
{
    //удаление не нужных столбцов
    if(!converter.invoiceResult.empty())
    {
        struct Match
        {
            std::string data;
            int count;
        };

        std::vector<Match> matches;
        matches.resize(converter.invoiceResult[0].size());
        int maxMatches = ui->lineEditDept->text().toInt();
//        std::cout << "Max matches = " << maxMatches << "\n";

        for(int col = 1; col < converter.invoiceResult[0].size(); col++)
        {
            for(int row = 0; row < converter.invoiceResult.size(); row++)
            {
                if(matches.empty())
                {
                    matches[col].data = converter.invoiceResult[row][col];
                    matches[col].count = 1;
                }
                else
                {
                    if(matches[col].data == converter.invoiceResult[row][col])
                    {
                        matches[col].count++;
                        if(matches[col].count <= maxMatches)
                        {
                            for(int i = 0; i < converter.invoiceResult.size(); i++)
                            {
                                converter.invoiceResult[i].erase(converter.invoiceResult[i].begin() + col);
                            }
                            matches.erase(matches.begin() + col);
                            col--;
                            break;
                        }
                    }
                    else
                    {
                        matches[col].data = converter.invoiceResult[row][col];
                        matches[col].count = 1;
                    }
                }
            }
        }
        ui->labelPositions->setText(QString::number(converter.invoiceResult.size()));
        ui->pushButtonShowInvoice->setText("Инвойс");
        showTab(converter.invoiceResult);
        analyzed = true;
        //удаление не нужных столбцов // ПРОВЕРИТЬ ФАЙЛ C:\Users\User\Desktop\test\F22ST11GX035CIPL_UPD.xlsx,_Spec14
    }
}
// ЛЕВАЯ ТАБЛИЦА

// ПРАВАЯ ТАБЛИЦА
void qrDialog::on_pushButtonOpenQR_clicked()
{
    converter.qrXls.clear();
    qrNewFileClear();

    QString qrPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл QR", lastPath.absolutePath(), "*.xls *.xlsx");
    if(qrPathTemp.isEmpty())
    {
        ui->labelPathQR->setText("Выберите файл!");
    }
    else
    {
        lastPath = qrPathTemp;

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists()) QDir().mkdir("temp"); // Создание папки temp

        // определение разрешения
        if(qrPathTemp.right(4) == "xlsx" || qrPathTemp.right(4) == "XLSX")
        {
            ext = ".xlsx";
        }
        else
        {
            ext = ".xls";
        }

        qrFileName = "temp/qr" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(qrPathTemp, qrFileName);

        if(!copySucces) QMessageBox::information(this, "Открытие файла", "Что-то пошло не так!");

        if(ext == ".xlsx")
        {
            std::string path = qrFileName.toStdString();
            converter.readXlsX(path, converter.qrXls, converter.qrSheetNames);
        }
        else
        {
            std::wstring path = qrFileName.toStdWString();
            converter.readXls(path, converter.qrXls, converter.qrSheetNames);
        }

        ui->labelPathQR->setText(QFileInfo(qrPathTemp).fileName());

        QFile::remove(qrFileName);

        // !!! ТАБЛИЦА !!!
        currentTabQr = 0;
        converter.qrSheetSettings.resize(converter.qrXls.size());
        selectedQrCols.resize(converter.qrXls.size());

        showQr = true;
        showTabQr(converter.qrXls[currentTabQr]);
    }
}

void qrDialog::on_pushButtonTabUpQR_clicked()
{
    if(currentTabQr < converter.qrXls.size() - 1 && converter.qrXls.size() > 0)
    {
        currentTabQr++;
        showQr = true;
        showTabQr(converter.qrXls[currentTabQr]);
        ui->pushButtonShowQR->setText("Показать выбранные");

        ui->listWidget->clear();
        if(selectedQrCols[currentTabQr].size() > 0)
        {
            for(int i = 0; i < selectedQrCols[currentTabQr].size(); i++)
            {
                QString itemText = QString::fromUtf16( u"Колонка : ") + QString::fromStdString(Extras::IntToSymbol(selectedQrCols[currentTabQr][i]))
                        + " (" + QString::fromStdString(converter.qrXls[currentTabQr][1][selectedQrCols[currentTabQr][i]]) + ")";

                ui->listWidget->addItem(itemText);
            }
        }        

        ui->labelColQR->setText(QString::fromStdString(Extras::IntToSymbol(converter.qrSheetSettings[currentTabQr].qrCol)));
    }
}

void qrDialog::on_pushButtonTabDownQR_clicked()
{
    if(currentTabQr > 0 && converter.qrXls.size() > 0)
    {
        currentTabQr--;
        showQr = true;
        showTabQr(converter.qrXls[currentTabQr]);
        ui->pushButtonShowQR->setText("Показать выбранные");

        ui->listWidget->clear();
        if(!selectedQrCols[currentTabQr].empty())
        {
            for(int i = 0; i < selectedQrCols[currentTabQr].size(); i++)
            {
                QString itemText = QString::fromUtf16( u"Колонка : ") + QString::fromStdString(Extras::IntToSymbol(selectedQrCols[currentTabQr][i]))
                        + " (" + QString::fromStdString(converter.qrXls[currentTabQr][1][selectedQrCols[currentTabQr][i]]) + ")";
                ui->listWidget->addItem(itemText);
            }
        }

        ui->labelColQR->setText(QString::fromStdString(Extras::IntToSymbol(converter.qrSheetSettings[currentTabQr].qrCol)));
    }
}

void qrDialog::showTabQr(std::vector<std::vector<std::string>> &inTab)
{
    QString labelTabString = QString::number(currentTabQr + 1);
    QString labelTabsString = QString::number(converter.qrXls.size());

    ui->labelTabName2->setText(converter.qrSheetNames[currentTabQr]);

    ui->labelTabQR->setText(labelTabString);
    ui->labelTabsQR->setText(labelTabsString);

    QVector<QVector<QString>> temp(Extras::toQvecConvert(inTab));
    Extras::showTable(temp, ui->tableWidget_2);

    ui->tableWidget_2->horizontalScrollBar()->setValue(0);
}

void qrDialog::on_pushButtonColQR_clicked()
{
    if(converter.qrXls.size() > 0 && showQr)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_2->selectionModel()->currentIndex().column();
        int row = ui->tableWidget_2->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.qrSheetSettings[currentTabQr].qrCol = col;
        converter.qrSheetSettings[currentTabQr].startRow = row;

        ui->labelColQR->setText(QString::fromStdString(Extras::IntToSymbol(converter.qrSheetSettings[currentTabQr].qrCol)));
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

void qrDialog::on_pushButtonAddColsQR_clicked()
{
    if(!converter.qrXls.empty() && showQr)
    {
//        QMessageBox::information(this, "!?", "AddColsQr clicked\n");
        std::cout << "AddColsQr clicked\n";

        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_2->selectionModel()->currentIndex().column();
        int row = ui->tableWidget_2->selectionModel()->currentIndex().row();

        if(converter.qrXls[currentTabQr][row][col].size() > ui->lineEditQrLenght->text().toInt())
        {
            QMessageBox::information(this, "Внимание!",
                                     "Длина содержимого ячейки похожа на QR код.\n"
                                     "Данные будут добавлены. Если данные ошибочны - удалите их!\n"
                                     "Для добавления столбца QR кода надо воспользоваться кнопкой:\n \"Добавить QR\"");
        }
        // Взять данные из выбранной ячейки

        bool exist = false;

        if(selectedQrCols[currentTabQr].size() > 0)
        {
//            QMessageBox::information(this, "!?", "Выбрана ли уже такая колонка");
            for(int i = 0; i < selectedQrCols[currentTabQr][i]; i++)
            {
                if(selectedQrCols[currentTabQr][i] == col)
                {
                    exist = true;
                }
            }
        }

        if(!exist)
        {
//            QMessageBox::information(this, "!?", "Колонка уже есть");
            selectedQrCols[currentTabQr].push_back(col);
            QString itemText = QString::fromUtf16( u"Колонка : ") + QString::fromStdString(Extras::IntToSymbol(col))
                    + " (" + QString::fromStdString(converter.qrXls[currentTabQr][row][col]) + ")";
            ui->listWidget->addItem(itemText);
        }
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

// Добавить в коды
void qrDialog::on_pushButtonAddToQrResult_clicked()
{
    if(converter.qrXls.size() > 0 && !selectedQrCols.empty() &&
            (converter.qrSheetSettings[currentTabQr].qrCol >= 0
            && converter.qrSheetSettings[currentTabQr].qrCol < converter.qrXls[currentTabQr].size()))
    {
        if(tabAlreadyAdded(currentTabQr))
        {
            QMessageBox::information(this, "!?", "Эта вкладка уже добавлена к списку кодов");
        }
        else
        {
            int currentSize = converter.qrResult.size();
            int maxSize = currentSize + converter.qrXls[currentTabQr].size();

            QString message = "Данные установлены\n Текущий размер массива: ";

            for(int row = 0; row < converter.qrXls[currentTabQr].size(); row++)
            {

                std::vector<std::string> newRow;

                if(emptyCell(converter.qrXls[currentTabQr][row][converter.qrSheetSettings[currentTabQr].qrCol])
                             || converter.qrXls[currentTabQr][row][converter.qrSheetSettings[currentTabQr].qrCol].size() < ui->lineEditQrLenght->text().toInt())
                {
                   continue;
                }
                else
                {
                    newRow.push_back(converter.qrXls[currentTabQr][row][converter.qrSheetSettings[currentTabQr].qrCol]);
                }


                for(int col = 0; col < selectedQrCols[currentTabQr].size(); col++)
                {
                    int selectedCol = selectedQrCols[currentTabQr][col];

                    if(emptyCell(converter.qrXls[currentTabQr][row][selectedCol]))
                    {
                        newRow.push_back("NO DATA");
                    }
                    else
                    {
                        newRow.push_back(converter.qrXls[currentTabQr][row][selectedCol]);
                    }
                }
                    converter.qrResult.push_back(newRow);
            }

            if(showQr)
            {
                showQr = false;
                showTabQr(converter.qrResult);
                ui->pushButtonShowQR->setText("Файл");
            }

            addedQrTabs.push_back(currentTabQr);

            ui->labelQrQty->setText(QString::number(converter.qrResult.size()));
            showTabQr(converter.qrResult);
        }
    }
}

void qrDialog::on_pushButtonShowQR_clicked()
{
    if(converter.qrXls.size() > 0 && converter.qrResult.size() > 0)
    {
        if(showQr)
        {
            showQr = false;
            showTabQr(converter.qrResult);
            ui->pushButtonShowQR->setText("Файл");
        }
        else
        {
            showQr = true;
            showTabQr(converter.qrXls[currentTabQr]);
            ui->pushButtonShowQR->setText("Коды");
        }
    }
}

void qrDialog::on_pushButtonResetQR_clicked()
{
    converter.clearQrData();

    selectedQrCols.clear();
    addedQrTabs.clear();
    compares.clear();

    showQr = false;

    ui->labelPathQR->clear();
    ui->labelColQR->clear();
    ui->labelQrQty->clear();
    ui->listWidget->clear();
    ui->labelCodesFounded->clear();
    ui->labelTabQR->setText("0");
    ui->labelTabsQR->setText("0");
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(0);

    QMessageBox::information(this, "!", "Индексы QR очищены");
}
// ПРАВАЯ ТАБЛИЦА

// ГЛАВНОЕ МЕНЮ
void qrDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void qrDialog::on_pushButtonBasicSize_clicked()
{
    this->showMinimized();
}

void qrDialog::on_pushButtonClear_clicked()
{
    ui->listWidget->clear();
    if(!selectedQrCols.empty())
    {
        selectedQrCols[currentTabQr].clear();
    }
}
//ГЛАВНОЕ МЕНЮ
void qrDialog::on_pushButtonDelete_clicked()
{
    selectedQrCols[currentTabQr].erase(selectedQrCols[currentTabQr].begin() + ui->listWidget->currentRow());

    ui->listWidget->clear();

    if(selectedQrCols[currentTabQr].size() > 0)
    {
        for(int i = 0; i < selectedQrCols[currentTabQr].size(); i++)
        {
            QString itemText;
            QTextStream textStream(&itemText);

            textStream << "Колонка : " << QString::fromStdString(Extras::IntToSymbol(selectedQrCols[currentTabQr][i]))
                       << " (" << QString::fromStdString(converter.qrXls[currentTabQr][1][selectedQrCols[currentTabQr][i]]) << ")";
            ui->listWidget->addItem(itemText);
        }
    }
}

// Показать результат, если надо - заполнить
void qrDialog::on_pushButtonShowResult_clicked()
{
    if(converter.invoiceResult.size() > 0 && converter.qrResult.size() > 0)
    {
        tempInvoice = converter.invoiceResult;
        tempQr = converter.qrResult;

        if(converter.result.empty() && converter.resultInfo.empty())
        {
            // Сопоставление столбцов
            for(int itemCol = 1; itemCol < tempInvoice[0].size(); itemCol++)
            {
                for(int qrRow = 0; qrRow < tempQr.size(); qrRow++)
                {
                    bool founded = false;

                    for(int qrCol = 1; qrCol < tempQr[qrRow].size(); qrCol++)
                    {
                        if(tempInvoice[0][itemCol] == tempQr[qrRow][qrCol])
                        {
                            bool exist = false;
                            for(int i = 0; i < compares.size(); i++)
                            {
                                if(compares[i].invoiceCol == itemCol || compares[i].qrCol == qrCol) exist = true;
                            }
                            if(!exist)
                            {
                                Compare tempCompare;
                                tempCompare.invoiceCol = itemCol;
                                tempCompare.qrCol = qrCol;
                                founded = true;
                                compares.push_back(tempCompare);
                            }
                            break;
                        }
                    }

                    if(founded) break;
                }
            }
            // Сопоставление столбцов

            if(compares.empty())
            {
                QMessageBox::information(this, "Внимание!", "Нет совпадений для поиска!");
                return;
            }

            // Заполнение таблиц converter.result converter.resultInfo

            int counter = 0; // Номер ПП

            // проход по товарам
            for(int invoiceRow = 0; invoiceRow < tempInvoice.size(); invoiceRow++)
            {
                int itemsQty = std::stoi(tempInvoice[invoiceRow][0]); // берём количество кодов к товару
                int numGroup = 0; // номер группы(столбец С)

                // поиск товара в кодах
                for(int qrRow = 0; qrRow < tempQr.size(); qrRow++)
                {
                    bool found = true;
                    // Поиск совпадений по указанным сопоставлениям
                    // Проход по столбцам указанным в сопоставлении
                    for(int i = 0; i < compares.size(); i++)
                    {
                        // если хоть одно значение в ячейки для сопоставления не сходится
                        if(tempInvoice[invoiceRow][compares[i].invoiceCol] != tempQr[qrRow][compares[i].qrCol])
                        {
                            found = false;
                        }
                    }
                    // Поиск совпадений по указанным сопоставлениям

                    // Если найдено совпадение
                    if(found && itemsQty > 0)
                    {
                        counter++;
                        numGroup++;
                        itemsQty--;

                        std::vector<std::string> tempResultRow;

                        // Добавление порядкового номера
                        tempResultRow.push_back(std::to_string(counter));

                        // Добавление "№товара"
                        tempResultRow.push_back(std::to_string(invoiceRow + 1));


                        // Добавление "№группы"
                        tempResultRow.push_back(std::to_string(numGroup));

                        // добавление QR "Идентификатор"
                        std::string qrCut;                        
                        std::string qr = tempQr[qrRow][0];

                        for(int k = 0; k < ui->lineEditQrLenght->text().toInt(); k++)
                        {
                            qrCut += qr[k];
                        }
                        tempResultRow.push_back(qrCut);
                        tempQr[qrRow][1] = "used";

                        // добавление "Код вида"
                        tempResultRow.push_back(std::to_string(301));

                        // добавление "Уровень маркировки"
                        tempResultRow.push_back(std::to_string(0));

                        // добавление Item code
                        std::string itemCode;
                        for(int i = 0; i < compares.size(); i++)
                        {
                            itemCode += tempInvoice[invoiceRow][compares[i].invoiceCol];
                            if(i < compares.size() - 1) itemCode += " ";
                        }
                        tempResultRow.push_back(itemCode);

                        // Добавление полученной строки в таблицу
                        converter.result.push_back(tempResultRow);
                    }
                }

                // заполнение информационной таблицы

                std::vector<std::string> tempInfoRow = tempInvoice[invoiceRow];


                tempInfoRow.insert(tempInfoRow.begin(), "\0");
                tempInfoRow.insert(tempInfoRow.begin(), std::to_string(numGroup));

                converter.resultInfo.push_back(tempInfoRow);

                // Вывод результатов
                ui->labelCodesFounded->setText(QString::number(converter.result.size()));
            }
        }
        showTabQr(converter.result);
        showTab(converter.resultInfo);
    }
    else
    {
        QMessageBox::information(this, "Внимание!", "Не подготовлены промежуточные данные!");
    }
}

// Очиситить результат
void qrDialog::on_pushButtonClearResult_clicked()
{
    if(!converter.result.empty()) converter.result.clear();
    if(!converter.resultInfo.empty()) converter.resultInfo.clear();

    showTab(converter.invoiceXls[currentTab]);
    showTabQr(converter.qrXls[currentTabQr]);

    ui->labelCodesFounded->clear();
}

void qrDialog::on_pushButtonShowNotUsedCodes_clicked()
{
    for(int rowQr = 0; rowQr < converter.qrResult.size(); rowQr++)
    {
        if(converter.qrResult[rowQr][0] != "used")
        {
            notUsedQr.push_back(converter.qrResult[rowQr]);
        }
    }
    if(notUsedQr.empty())
    {
        QMessageBox::information(this, "Внимание!", "Нет неиспользованных QR кодов");
    }
    else
    {
        showTabQr(notUsedQr);
    }
}


void qrDialog::on_pushButtonSave_clicked()
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
        }
    }
    else
    {
        QMessageBox::information(this, "!", "Нечего сохранять!");
    }
}


void qrDialog::on_pushButtonExit_clicked()
{
    QCoreApplication::quit();
}


void qrDialog::on_pushButtonNew_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->labelPath->clear();
    ui->labelTab->setText("0");
    ui->labelTabs->setText("0");
    ui->labelFirstQty->setText("");
    ui->labelItemsQty->setText("");
    ui->labelColQty->setText("");
    ui->labelItemsQty->setText("");
    ui->labelPositions->setText("");
    compares.clear();
    converter.clearInvoiceData();

    qrClear();

    converter.clearQrData();
    selectedQrCols.clear();
    addedQrTabs.clear();
    compares.clear();

    showQr = false;

    ui->labelPathQR->clear();
    ui->labelColQR->clear();
    ui->labelQrQty->clear();
    ui->listWidget->clear();
    ui->labelCodesFounded->clear();
    ui->labelTabQR->setText("0");
    ui->labelTabsQR->setText("0");
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(0);

    addedToInv = false;
}


void qrDialog::on_pushButtonArticulesQr_clicked()
{
    converter.calculateQrArticules();
    showTabQr(converter.qrArtuculesVec);
}


void qrDialog::on_pushButtonArticulesQrReset_clicked()
{
    converter.clearArt();
}


void qrDialog::on_pushButtonAddToInv_clicked()
{
    // Данные из QR для добавления
    int rowDataToAdd = ui->tableWidget_2->selectionModel()->currentIndex().row();
    // Целевая строка инвойса
    int rowItem = ui->tableWidget->selectionModel()->currentIndex().row();
    // Взять данные из выбранной ячейки

    if(!addedToInv)
    {
        for(int row = 0; row < converter.invoiceResult.size(); row++)
        {
            converter.invoiceResult[row].push_back("\0");
        }
        addedToInv = true;
    }

    converter.invoiceResult[rowItem][converter.invoiceResult[rowItem].size() - 1] = converter.qrArtuculesVec[rowDataToAdd][0];

//    converter.invoiceResult[rowItem].push_back(converter.qrArtuculesVec[rowDataToAdd][0]);

    showTab(converter.invoiceResult);
}

void qrDialog::on_pushButtonHelp_clicked()
{
    if(!helpWindow->isVisible())
    {
        helpWindow->show();
    }
}

void qrDialog::fromTextFiles(std::vector<std::vector<std::string> > data)
{
    converter.qrResult = data;
    showTabQr(converter.qrResult);
}
