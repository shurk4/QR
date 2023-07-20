#include "containers.h"
#include "ui_containers.h"

containers::containers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::containers)
{
    ui->setupUi(this);

    this->resize(1280, 720);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);
}

containers::~containers()
{
    delete ui;
}

void containers::showTable_1(QVector<QVector<QString>> &inTab)
{
    QString labelTab = QString::number(currentTab_1 + 1);
    QString labelTabs = QString::number(converter.qrXls.size());

    ui->labelTab->setText(labelTab);
    ui->labelTabs->setText(labelTabs);

    Extras::showTable(inTab, ui->tableWidget_1);
}

void containers::showTable_2(QVector<QVector<QString>> &inTab)
{
    QString labelTabString = QString::number(currentTab_2 + 1);
    QString labelTabsString = QString::number(converter.qrXls.size());

    ui->labelTab_2->setText(labelTabString);
    ui->labelTabs_2->setText(labelTabsString);

    Extras::showTable(inTab, ui->tableWidget_2);
}

// добавление данных из спецификаци в карту: item код / номер контейнера / был ли использован в результате
void containers::collectItemsCtn(QString item, ItemInfo itemInfo)
{
    itemCtn.emplace(item, itemInfo);
}

// ковертер int в символ
std::string containers::toSymbol(const int in)
{
    std::string str;
        str += ('A' + in);
        return str;
}

bool containers::emptyCell(const QString &str)
{
    return str.size() == 0 || str[0] == '\0';
}

// проверка строки на номер контейнера
bool containers::isCntNum(const std::string str)
{
    if(str.size() != 11)
    {
        return false;
    }
    else
    {
        for(int i = 0; i < str.size(); i++)
        {
            return ((i < 4 && std::tolower(str[i]) >= 'a' && std::tolower(str[i] <= 'z'))
                    || (i >= 4 && str[i] >= '0' && str[i] <= '9'));
        }
    }
}

// загрузить спецификацию, табл. 1
void containers::on_pushButtonSpecOpen_clicked()
{
    QString pathTemp = QFileDialog::getOpenFileName(this, "Выберите файл Спецификации", lastPath.absolutePath(), "*.xls *.xlsx");
    if(pathTemp.isEmpty())
    {
        ui->labelSpec->setText("Выберите файл!");
    }
    else
    {
//        converter.clearQrData(); !!!
//        converter.clearResult(); !!!
//        itemCtn.clear();

        lastPath = pathTemp;

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists()) QDir().mkdir("temp"); // Создание папки temp

        // определение разрешения
        if(pathTemp.right(4) == "xlsx" || pathTemp.right(4) == "XLSX")
        {
            ext = ".xlsx";
        }
        else
        {
            ext = ".xls";
        }

        fileName = "temp/tmp" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(pathTemp, fileName);

        if(!copySucces) QMessageBox::information(this, "Открытие файла", "Что-то пошло не так!");

        if(ext == ".xlsx")
        {
            std::string path = fileName.toStdString();
            converter.readXlsX(path, converter.qrXls, converter.qrSheetNames);
        }
        else
        {
            std::wstring path = fileName.toStdWString();
            converter.readXls(path, converter.qrXls, converter.qrSheetNames);
        }

        ui->labelSpec->setText(pathTemp);

        QFile::remove(fileName);

        // !!! ТАБЛИЦА !!!
        converter.qrSheetSettings.resize(converter.qrXls.size());
        showTable_1(converter.qrXls[currentTab_1]);
    }
}

// загрузить инвойс, табл. 2
void containers::on_pushButtonInvOpen_clicked()
{
    QString pathTemp = QFileDialog::getOpenFileName(this, "Выберите файл Инвойса", lastPath.absolutePath(), "*.xls *.xlsx");
    if(pathTemp.isEmpty())
    {
        ui->labelInv->setText("Выберите файл!");
    }
    else
    {        
        converter.clearInvoiceData();
        converter.clearResult();

        lastPath = pathTemp;

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists()) QDir().mkdir("temp"); // Создание папки temp

        // определение разрешения
        if(pathTemp.right(4) == "xlsx" || pathTemp.right(4) == "XLSX")
        {
            ext = ".xlsx";
        }
        else
        {
            ext = ".xls";
        }

        fileName = "temp/tmp" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(pathTemp, fileName);

        if(!copySucces) QMessageBox::information(this, "Открытие файла", "Что-то пошло не так!");

        if(ext == ".xlsx")
        {
            std::string path = fileName.toStdString();
            converter.readXlsX(path, converter.invoiceXls, converter.invoiceSheetNames);
        }
        else
        {
            std::wstring path = fileName.toStdWString();
            converter.readXls(path, converter.invoiceXls, converter.invoiceSheetNames);
        }

        ui->labelInv->setText(pathTemp);

        QFile::remove(fileName);

        // !!! ТАБЛИЦА !!!
        converter.invoiceSheetSettings.resize(converter.invoiceXls.size());
        showTable_2(converter.invoiceXls[currentTab_2]);
    }
}

// навигация т. 1
void containers::on_pushButtonSpecDown_clicked()
{
    if(currentTab_1 > 0 && converter.qrXls.size() > 0)
    {
        currentTab_1--;
        showTable_1(converter.qrXls[currentTab_1]);
    }
}

void containers::on_pushButtonSpecUp_clicked()
{
    if(currentTab_1 < converter.qrXls.size() - 1 && converter.qrXls.size() > 0)
    {
        currentTab_1++;
        showTable_1(converter.qrXls[currentTab_1]);
    }
}
// навигация т. 1

// навигация т. 2
void containers::on_pushButtonInvDown_clicked()
{
    if(currentTab_2 > 0 && converter.invoiceXls.size() > 0)
    {
        currentTab_2--;
        showTable_2(converter.invoiceXls[currentTab_2]);
    }
}

void containers::on_pushButtonInvUp_clicked()
{
    if(currentTab_2 < converter.invoiceXls.size() - 1 && converter.invoiceXls.size() > 0)
    {
        currentTab_2++;
        showTable_2(converter.invoiceXls[currentTab_2]);
    }
}
// навигация т. 2


// выбор item столбца итем кода в спецификации т. 1 певый итем или строка перед ним в которой есть номер к-ра
void containers::on_pushButtonSpecItemSelect_clicked()
{
    ui->labelItemRow->setText(QString::number(ui->tableWidget_1->currentItem()->row() + 1));
    ui->labelItemCol->setText(QString::fromStdString(toSymbol(ui->tableWidget_1->currentItem()->column())));
    converter.qrSheetSettings.resize(converter.qrXls.size());
    converter.qrSheetSettings[currentTab_1].itemCol = ui->tableWidget_1->currentItem()->column();
    converter.qrSheetSettings[currentTab_1].startRow = ui->tableWidget_1->currentItem()->row();
}

// выборка данных из спецификации
void containers::on_pushButtonSpecAnalyze_clicked()
{
    int startRow = converter.qrSheetSettings[currentTab_1].startRow;
    int itemCol = converter.qrSheetSettings[currentTab_1].itemCol;

    int ctnCounter = 0;
//    bool ctnNumberFirst = false;

    QString currentCtnNumber; // текущий номер контейнера
    QVector<QString> items; // временная выборка итем кодов
    int ctnCol = -1;
    bool ctnColFound = false;

    // если строка и столбец итем кода выбраны
    if(startRow > -1 && itemCol > -1)
    {
        // если выбрана пустая ячейка итем кода ищем в строке номер контейнера
        if(emptyCell(converter.qrXls[currentTab_1][startRow][itemCol]))
        {
            QMessageBox::information(this, "!", "Выбрана неправильная ячейка с item кодом!");
//            for(int col = 0; col < converter.qrXls[currentTab_1][startRow].size(); col++)
//            {
//                if(isCntNum(converter.qrXls[currentTab_1][startRow][col]))
//                {
//                    ctnNumberFirst = true;
//                    ctnCol = col;
//                    currentCtnNumber = converter.qrXls[currentTab_1][startRow][col];

//                    break;
//                }
//            }

//            if(!ctnNumberFirst)
//            {
//                QMessageBox::information(this, "!", "Неправильно отмечена ячейка с первым кодом!");
//                return;
//            }
        }
        else
        {
            // выборка данных из таблицы
            for(int row = startRow; row < converter.qrXls[currentTab_1].size(); row++)
            {
                if(!emptyCell(converter.qrXls[currentTab_1][row][itemCol])) // Если ячейка item не пустая добавляем коды в временный вектор
                {
//                    QString message = "Добавлен код: " + QString::fromStdString(converter.qrXls[currentTab_1][row][itemCol]);
//                    QMessageBox::information(this, "!", message);
                    items.push_back(converter.qrXls[currentTab_1][row][itemCol]);
                }
                else //если ячейка с итем кодом пустая смотрим номер контейнера и записываем данные в карту
                {
                    if(ctnCol == -1) // Если колонка с номером не определена
                    {
//                        QMessageBox::information(this, "!", "Поиск колонки с номером контейнера");
                        // Ищем в текущей строке ячейку с номером
                        for(int col = 0; col < converter.qrXls[currentTab_1][row].size(); col++)
                        {
                            if(isCntNum(converter.qrXls[currentTab_1][row][col].toStdString()))
                            {
                                ctnColFound = true;
                                ctnCol = col;
//                                QString message = "Колонка с номером контейнера найдена: " + QString::number(col);
//                                QMessageBox::information(this, "!", message);
                                break;
                            }
                        }
                    }

                    if(!ctnColFound)
                    {
                        QMessageBox::information(this, "!", "Не удалось найти номер контейнера!");
                        return;
                    }

                    if(isCntNum(converter.qrXls[currentTab_1][row][ctnCol].toStdString()))
                    {
                        ctnCounter++;
                        ctnColFound = true;
                        currentCtnNumber = converter.qrXls[currentTab_1][row][ctnCol];
                        // QString message = "Текущий номер контейнера: " + QString::fromStdString(converter.qrXls[currentTab_1][row][ctnCol]);
//                        QMessageBox::information(this, "!", message);
                    }
                    else
                    {
//                        QMessageBox::information(this, "!", "Номер контейнера пуст или имеет неправильный формат!");
                    }

                    // Если колонка с номером контейнера определена и текущая ячейка с номером содержит номер контейнера
                    if(ctnColFound && isCntNum(converter.qrXls[currentTab_1][row][ctnCol].toStdString()))
                    {
//                        QMessageBox::information(this, "!", "Добавление данных в карту!");
                        for(int i = 0; i < items.size(); i++)
                        {
                            ItemInfo itemInfo;
                            if(items.size() > 1)
                            {
                                itemInfo.fullLoad = true;
                            }
                            itemInfo.ctnNumber = currentCtnNumber;
                            collectItemsCtn(items[i], itemInfo);
                        }
                        items.clear();
                    }
                    else
                    {
//                        QMessageBox::information(this, "!", "Колонка с номером контейнера не определена!");
                        break;
                    }
                }
            }

//            QMessageBox::information(this, "!", "Данные обработаны. Размер карты: " + QString::number(itemCtn.size()));

            // заполнение результата выборки

            QVector<QString> colNames {"Арт.", "Номер к-ра", "Тип измерения"};
            converter.qrResult.push_back(colNames);

            for(const auto &it : itemCtn)
            {
                QVector<QString> tempRow;
                tempRow.push_back(it.first);
                tempRow.push_back(it.second.ctnNumber);
                if(it.second.fullLoad == 0)
                {
                    tempRow.push_back(QString::number(1));
                }
                else
                {
                    tempRow.push_back(QString::number(2));
                }
                converter.qrResult.push_back(tempRow);
            }

            //вывод таблицы с результатом
            showTable_1(converter.qrResult);
            ui->labelPositionNum->setText(QString::number(itemCtn.size()));
            ui->labelCtnNum->setText(QString::number(ctnCounter));
        }
    }
    else
    {
        QMessageBox::information(this, "!", "Не отмечена стартовая ячейка или выбрана неотмеченная влкадка!");
    }
}

// подготовка результата
void containers::on_pushButtonResult_clicked()
{
    int itemCol = -1;
    int firstRow = -1;

    if(!converter.invoiceXls.empty())
    {
        for(int row = 0; row < converter.invoiceXls[currentTab_2].size(); row++)
        {
            // Поиск колонки с итем кодом
            if(itemCol == -1)
            {
                for(int col = 0; col < converter.invoiceXls[currentTab_2][row].size(); col++)
                {
//                    QString message = "converter.invoiceXls[currentTab_2][row][col]: " + QString::fromStdString(converter.invoiceXls[currentTab_2][row][col]);
//                    // QMessageBox::critical(this, "!", message);

                    auto it = itemCtn.find(converter.invoiceXls[currentTab_2][row][col]);

                    if(it != itemCtn.end())
                    {
                        // QString message = "It: " + QString::fromStdString(it->first);
                        // QMessageBox::critical(this, "!", message);
                        itemCol = col;
                        firstRow = row;
                        break;
                    }
                }
            }
            else break;
            // Поиск колонки с итем кодом
        }

        if(itemCol == -1)
        {
            QMessageBox::information(this, "!", "Не удалось определить колонку с item кодом!");
            return;
        }

        // если колонка найдена
        if(itemCol > -1)
        {            
            // QMessageBox::critical(this, "!", "itemCtn.size(): " + QString::number(itemCtn.size()));
            for(int row = firstRow; row < converter.invoiceXls[currentTab_2].size(); row++)
            {
                // QMessageBox::critical(this, "!", "Row: " + QString::number(row));
                // QMessageBox::critical(this, "!",
//                                      "Item: " + QString::fromStdString(converter.invoiceXls[currentTab_2][row][itemCol]));
                auto range = itemCtn.equal_range(converter.invoiceXls[currentTab_2][row][itemCol]);
                if(range.first != itemCtn.end() && range.second != itemCtn.end())
                {
                    itemNum++;
                    for(auto i = range.first; i != range.second; i++)
                    {
                        // QString message = "i.first: " + QString::fromStdString(i->first);
                        // QMessageBox::critical(this, "!", message);

                        QVector<QString> tempRow;
                        tempRow.push_back(QString::number(itemNum));
                        tempRow.push_back(i->second.ctnNumber);
                        tempRow.push_back("CN");
                        if(i->second.fullLoad)
                        {
                            tempRow.push_back(QString::number(2));
                        }
                        else
                        {
                            tempRow.push_back(QString::number(1));
                        }
                        converter.result.push_back(tempRow);
                    }
                }
                else
                {
                    auto it = itemCtn.find(converter.invoiceXls[currentTab_2][row][itemCol]);
                    if(it != itemCtn.end())
                    {
                        itemNum++;
                        // QString message = "it.first: " + QString::fromStdString(it->first);
//                        QMessageBox::critical(this, "!", message);

                        QVector<QString> tempRow;
                        tempRow.push_back(QString::number(itemNum));
                        tempRow.push_back(it->second.ctnNumber);
                        tempRow.push_back("CN");
                        if(it->second.fullLoad)
                        {
                            tempRow.push_back(QString::number(2));
                        }
                        else
                        {
                            tempRow.push_back(QString::number(1));
                        }
                        converter.result.push_back(tempRow);
                    }
                    else
                    {
//                        QMessageBox::critical(this, "!", "Больше кодов нет. Товаров: " + QString::number(converter.result.size()));
                        break;
                    }
                }

                if(emptyCell(converter.invoiceXls[currentTab_2][row][itemCol])) break;
            }
        }

        showTable_2(converter.result);
    }
    else
    {
        QMessageBox::information(this, "!", "Сначала нужно открыть файл инвойса!");
    }
}

// Сохранить результат
void containers::on_pushButtonSave_clicked()
{
    if(converter.result.size() > 0)
    {
        lastPath = QFileInfo(lastPath.path()).absolutePath() + "/containers.xls";

        QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить результат"), lastPath.absolutePath(), tr("Таблица xls (*.xls)"));
        if (fileName != "")
        {
            try
            {
                std::wstring path = fileName.toStdWString();
                converter.saveContainers(path);
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

// Новый
void containers::on_pushButton_clicked()
{
    ui->tableWidget_1->clear();
    ui->tableWidget_2->clear();
    ui->labelCtnNum->clear();
    ui->labelInv->clear();
    ui->labelItemCol->clear();
    ui->labelItemRow->clear();
    ui->labelPositionNum->clear();
    ui->labelSpec->clear();
    ui->labelTab->setText(QString::number(0));
    ui->labelTab_2->setText(QString::number(0));
    ui->labelTabs->setText(QString::number(0));
    ui->labelTabs_2->setText(QString::number(0));

    currentTab_1 = 0;
    currentTab_2 = 0;
    itemNum = 0;
    itemCtn.clear();
    converter.clear();
}

