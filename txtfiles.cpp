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
    readConfig();

    hideFilesWidgets();
    ui->widgetItemsInfo->hide();
    ui->widgetAddItems->hide();

    helpWindow = new help(this);
    helpWindow->showTextfilesHelp();
    helpWindow->setModal(false);
    helpWindow->hide();
}

txtFiles::~txtFiles()
{
    config->write();
    delete ui;
}

void txtFiles::readConfig()
{
    config = new UserConfig;
    QString temp;
    //Последний открытый путь к файлам
    if(config->get("lastPath", temp))
    {
        QDir dir(temp);

        lastPath.setPath(temp);
    }
    else
    {
        lastPath.setPath(QDir::homePath());
    }
}

void txtFiles::hideFilesWidgets()
{
    ui->widgetTabsPanel->hide();
    qDebug() << "widgetTabsPanel - hided";
    ui->WidgetInvAnalyze->hide();
    qDebug() << "WidgetInvAnalyze - hidded";
    ui->widgetQrButtons->hide();
    qDebug() << "widgetQrButtons - hided";
}

void txtFiles::setDisplayType(DisplayType type)
{
    qDebug() <<"setDisplayType" << type;
    hideFilesWidgets();
    qDebug() << "Widgets hided!";

    switch (type)
    {
    case (INV):
        ui->widgetTabsPanel->show();
        ui->WidgetInvAnalyze->show();
        displayType = INV;
        break;

    case (QR):
        ui->widgetTabsPanel->show();
        ui->widgetQrButtons->show();
        displayType = QR;
        break;

    case (TXT):
        displayType = TXT;
        break;

    case (RESULT):
        ui->tableWidget_1->scrollToBottom();
        displayType = RESULT;
        break;

    default:
        break;
    }
    qDebug() <<"setDisplayType - done";
}

void txtFiles::showTable_1(const QVector<QVector<QString>> &inTab)
{
    switch (displayType) {
    case INV:
        ui->labelTab->setText(QString::number(currentTab + 1));
        ui->labelTabs->setText(QString::number(converter.invoiceXls.size()));
        break;
    case QR:
        ui->labelTab->setText(QString::number(currentTabQr + 1));
        ui->labelTabs->setText(QString::number(converter.qrXls.size()));
        break;

    default:
        break;
    }

    Extras::showTable(inTab, ui->tableWidget_1);
    if(!converter.invoiceSheetNames.empty()) ui->labelTabName->setText(converter.invoiceSheetNames[currentTab]);
}

void txtFiles::showTable_2(const QVector<QVector<QString>> &table)
{
    // Вывести данные в таблицу
    Extras::showTable(table, ui->tableWidget_1);
}

void txtFiles::showDocs()
{
    if(!converter.qrReady())
    {
        QMessageBox::information(this, "!", "Не добавлено ни одного документа!");
        return;
    }

    ui->labelCodesInDoc->setText(QString::number(converter.getQrQtyInItem(currentDoc)));

    ui->listWidget->clear();

    for(int i = 0; i < converter.getQrItemsQty(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(converter.getQrItemName(i));
        if(converter.getQrStatus(i) == ADDED)
        {
            item->setBackgroundColor("#a1ff9f");
        }

        ui->listWidget->addItem(item);
    }

    ui->listWidget->setCurrentRow(0);

    ui->labelFilesNum->setText(QString::number(converter.getQrItemsQty()));
    setDisplayType(TXT);
}

void txtFiles::showAutoButton()
{
    if(converter.qrReady() && converter.invoiceReady())
    {

    }
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
        setDisplayType(INV);
        lastPath.setPath(invPathTemp);
        config->set("lastPath", lastPath.absolutePath());

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


        QString invoiceFileName = "temp/inv" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

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

        ui->labelInv->setText(lastPath.dirName());

        QFile::remove(invoiceFileName);

        // !!! ТАБЛИЦА !!!
        converter.invoiceSheetSettings.resize(converter.invoiceXls.size());
        currentTab = 0;
        showTable_1(converter.invoiceXls[currentTab]);

        invoiceShowed = true;
        ui->pushButtonShow->setText("Результат");
    }
}

void txtFiles::on_pushButtonDown_clicked()
{
    switch (displayType)
    {
    case INV:
        if(currentTab > 0 && converter.invoiceXls.size() > 0)
        {
            currentTab--;
            showTable_1(converter.invoiceXls[currentTab]);
        }
        break;
    case QR:
        if(currentTabQr > 0 && converter.qrXls.size() > 0)
        {
            currentTabQr--;
            showTable_1(converter.qrXls[currentTabQr]);
        }
        break;
    default: break;
    }
}

void txtFiles::on_pushButtonUp_clicked()
{
    switch (displayType) {
    case INV:
        if(currentTab < converter.invoiceXls.size() - 1 && converter.invoiceXls.size() > 0)
        {
            currentTab++;
            showTable_1(converter.invoiceXls[currentTab]);
        }
    case QR:
        if(currentTabQr < converter.qrXls.size() - 1 && converter.qrXls.size() > 0)
        {
            currentTabQr++;
            showTable_1(converter.qrXls[currentTabQr]);
        }
        break;
    default:
        break;
    }

}

void txtFiles::on_pushButtonTxt_clicked()
{
    // Получение списка файлов
    fileNames = QFileDialog::getOpenFileNames(nullptr, "Выбрать текстовые файлы", lastPath.absolutePath() , "Текстовые файлы(*.txt)");

    if(!fileNames.isEmpty())
    {
        lastPath.setPath(fileNames[0]);
        // Получение данных из списка файлов
        for (auto xfile : fileNames)
        {
            QFile file (xfile);
            // Получение имени файла
            QFileInfo info(file.fileName());
//            QrInfo tempData;
//            tempData.name = info.fileName();

//            tempData.status = NEW;
//            converter.addQrInfo(tempData);

            // Загрузка данных из файла в поток
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);

            // Обработка данных
            QVector<QVector<QString>> doc; // Данные из одного документа
            while (!in.atEnd())
            {
                QVector<QString> col;
                QString line = in.readLine();
                col.push_back(line);
                doc.push_back(col);
            }
            converter.addQrTxt(info.fileName(), doc);
        }

        currentDoc = 0;
        showTable_2(converter.getQrItem(currentDoc));

        showDocs();
    }
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

void txtFiles::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentDoc = ui->listWidget->currentRow();
    QVector<QVector<QString>> tempItem = converter.getQrItem(currentDoc);
    showTable_2(tempItem);
    ui->labelCodesInDoc->setText(QString::number(converter.getQrQtyInItem(currentDoc)));
    setDisplayType(TXT);
}

// Добавить коды из файла
void txtFiles::toCodes()
{
    if(converter.getQrStatus(currentDoc) == ADDED)
    {
        QMessageBox::information(this, "!?", "Этот файл уже добавлен!");
    }
    else
    {
        itemNum++;
        int numGroup = 0;
        converter.addItem(currentDoc); // Добавляет номер выбранного итем в вектор добавленных итемов и устанавливает ему статус ADDED
        markItemsTable();

        QVector<QVector<QString>> &tempItem = converter.getQrItem(currentDoc);

        converter.result.reserve(converter.result.size() + converter.getQrQtyInItem(currentDoc));

        for(int i = 0; i < converter.getQrQtyInItem(currentDoc); i++)
        {
            serial++;
            numGroup++;

            QVector<QString> tempResultRow;

            // Добавление порядкового номера
            tempResultRow.push_back(QString::number(serial));

            // Добавление "№товара"
            tempResultRow.push_back(QString::number(itemNum));


            // Добавление "№группы"
            tempResultRow.push_back(QString::number(numGroup));

            // добавление QR "Идентификатор"
            QString qrCut;
            QString qr = tempItem[i][0];
            for(int k = 0; k < 31; k++)
            {
                qrCut += qr[k];
            }
            tempResultRow.push_back(qrCut);

            // добавление "Код вида"
            tempResultRow.push_back(QString::number(301));

            // добавление "Уровень маркировки"
            tempResultRow.push_back(QString::number(0));

            // добавление Item code
            QString itemCode;
            QString itemName = converter.getQrItemName(currentDoc);

            for(int j = 0; j < itemName.size() - 4; j++)
            {
                if(itemName[j] > 122)
                {
                    continue;
                }
                else
                {
                    itemCode.push_back(itemName[j]);
                }
            }
            tempResultRow.push_back(itemCode);

            // Добавление полученной строки в таблицу
            converter.result.push_back(tempResultRow);

            ui->listWidget->currentItem()->setBackgroundColor("#a1ff9f");
            ui->listWidget->currentItem()->setFlags(Qt::ItemFlag::ItemIsEnabled);
        }

        showTable_1(converter.result);
        setDisplayType(RESULT);
        ui->labelResultItems->setText(QString::number(itemNum));
        ui->labelResultCodes->setText(QString::number(converter.result.size()));
        ui->pushButtonShow->setText("Инвойс");
        invoiceShowed = false;

        ui->widgetAddItems->show();
    }
}

void txtFiles::on_pushButtonAddQr_clicked()
{
    toCodes();
}

void txtFiles::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    toCodes();
}


void txtFiles::on_pushButtonOpenQr_clicked()
{
    converter.qrXls.clear();
    converter.qrSheetNames.clear();
    setDisplayType(QR);

    QString qrPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл QR", lastPath.absolutePath(), "*.xls *.xlsx");
    if(qrPathTemp.isEmpty())
    {
        ui->labelInv->setText("Выберите файл!");
    }
    else
    {
        lastPath.setPath(QFileInfo(qrPathTemp).absolutePath());
        config->set("lastPath", lastPath.absolutePath());

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

        QString qrFileName = "temp/qr" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(qrPathTemp, qrFileName);

        if(!copySucces) QMessageBox::information(this, "Открытие файла", "Не удалось создать временную копию файла!");

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

        ui->labelInv->setText(QFileInfo(qrPathTemp).fileName());

        QFile::remove(qrFileName);

        // !!! ТАБЛИЦА !!!
        currentTabQr = 0;
        converter.qrSheetSettings.resize(converter.qrXls.size());
//        selectedQrCols.resize(converter.qrXls.size());

//        showQr = true;
        showTable_1(converter.qrXls[currentTabQr]);
    }
}


void txtFiles::on_pushButtonFirstCell_clicked()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        int row = ui->tableWidget_1->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].qtyCol = col;
        converter.invoiceSheetSettings[currentTab].startRow = row;

        ui->labelInvCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
        ui->labelRowFirst->setText(QString::number(row + 1));
    }
}


void txtFiles::on_pushButtonLastCell_clicked()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int row = ui->tableWidget_1->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].stopRow = row;
        ui->labelRowEnd->setText(QString::number(row + 1));
    }
}


void txtFiles::on_pushButtonAnalyze_clicked()
{
    items = converter.getItemsForTxt(currentTab);
    if(items.empty()) QMessageBox::critical(this, "", "items empty");
    else
    {        
        qDebug() << "items = converter.getItemsForTxt(currentTab) - NOT EMPTY";
        Extras::showTable(items, ui->tableWidgetItems);
        qDebug() << "Extras::showTable(items, ui->tableWidgetItems) - done";
        ui->tableWidgetItems->setHorizontalHeaderLabels(QStringList() << "Ctn num" << "Name" << "Qty");
        ui->tableWidgetItems->verticalHeader()->setVisible(false);
        bool setColor = false;

        qDebug() << "Loop started. items.size() = " << items.size();
        for(size_t i = 0; i < items.size(); i++)
        {
            if(i != 0 && items[i][0] != items[i - 1][0])
            {
                setColor = !setColor;
            }

            if(setColor)
            {
                for(size_t j = 0; j < items[i].size(); j++)
                {
                    QTableWidgetItem* item = ui->tableWidgetItems->item(i, j);
                    item->setBackgroundColor(Qt::lightGray);
                }
            }
        }
        qDebug() << "Loop ended.";

        setDisplayType(TXT);
        qDebug() << "setDisplayType(TXT)";

        ui->widgetItemsInfo->show();
        qDebug() << "widgetItemsInfo showed";

        ui->tableWidgetItems->horizontalHeader()->setStretchLastSection(true);

        qDebug() << "widgetItemsInfo showed";

        ui->labelItemsNum->setText(QString::number(converter.getItemsPosQty()));
        qDebug() << "labelItemsNum - showed";
        ui->labelCtnsNum->setText(QString::number(converter.getCtnsQty()));
        qDebug() << "labelCtnsNum - showed";
        ui->labelOverallItems->setText(QString::number(converter.getItemsQty()));
        qDebug() << "labelOverallItems - showed";
    }
}

void txtFiles::on_pushButtonItemCol_clicked()
{
    int col = ui->tableWidget_1->selectionModel()->currentIndex().column();

    converter.invoiceSheetSettings[currentTab].itemCol = col;
    ui->labelItemCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
}


void txtFiles::on_pushButtonQrCol_clicked()
{
    if(converter.qrXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        // Взять данные из выбранной ячейки

        converter.qrSheetSettings[currentTabQr].qrCol = col;
        ui->labelQrCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

void txtFiles::on_pushButtonQrItemCol_clicked()
{
    if(converter.qrXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        // Взять данные из выбранной ячейки

        converter.qrSheetSettings[currentTabQr].itemCol = col;
        ui->labelQrItemCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

void txtFiles::on_pushButtonQrAnalyze_clicked()
{
    if(converter.haveQrSettings(currentTab))
    {
        converter.qrAnalyze(currentTab);

        showDocs();
        ui->widgetQrButtons->hide();
    }
    else
    {
        if(converter.qrSheetSettings[currentTabQr].itemCol <= -1)
            QMessageBox::information(this, "!", "Не выбрана калонка Item!");
        else
            QMessageBox::information(this, "!", "Не выбрана колонка QR");
    }
}

void txtFiles::on_pushButtonHelp_clicked()
{
    if(helpWindow->isHidden()) helpWindow->show();
    else helpWindow->hide();
}

void txtFiles::on_pushButtonAuto_clicked()
{
    for(int i = 0; i < items.size(); ++i)
    {
        int itemNumber;
        for(int j = 0; j < items[i].size(); ++j)
        {
//            if(converter.getQrItemName(i).contains(items[j]))
        }
    }
}

void txtFiles::on_pushButtonClearAll_clicked()
{
    itemNum = 0;
    items.clear();
    selectedRows.clear();

    converter.clearTxt();
    ui->tableWidgetItems->clear();
    ui->tableWidget_1->clear();
    ui->listWidget->clear();
    ui->labelInv->clear();
    ui->labelCodesInDoc->clear();
    ui->labelResultCodes->clear();
    ui->labelResultItems->clear();
    ui->labelItemsNum->clear();
    ui->labelOverallItems->clear();
    ui->labelCtnsNum->clear();
    ui->labelFilesNum->clear();

    ui->widgetTabsPanel->hide();
    ui->WidgetInvAnalyze->hide();
    ui->widgetQrButtons->hide();
    ui->widgetItemsInfo->hide();
    ui->widgetAddItems->hide();
}

void txtFiles::on_pushButtonShowResult_clicked()
{
    if(converter.resultEmpty()) QMessageBox::information(this, "", "Таблица результатов пуста!");
    showTable_1(converter.result);
}

void txtFiles::on_pushButtonClearFiles_clicked()
{
    ui->listWidget->clear();
    converter.clearResult();
    converter.clearQrTxt();

}

void txtFiles::on_pushButtonClearItems_clicked()
{
    converter.clearInvoiceData();
    items.clear();
    selectedRows.clear();
    ui->tableWidgetItems->clear();
    ui->widgetItemsInfo->hide();
}

void txtFiles::on_pushButtonCancelUndoResult_clicked()
{
    if(converter.undoResult() == -1) QMessageBox::information(this, "", "Таблица результатов пуста!");
    else
    {
        itemNum--;
        ui->labelResultItems->setText(QString::number(ui->labelResultItems->text().toInt() - 1));
        ui->labelResultCodes->setText(QString::number(ui->labelResultCodes->text().toInt() - converter.getQrQtyInItem(currentDoc)));
        showDocs();
        if(itemNum > 0) on_pushButtonShowResult_clicked();
    }
}

void txtFiles::on_pushButtonUndoRedoResult_clicked()
{
    currentDoc = converter.redoResult();
    if(currentDoc == -1)
    {
        currentDoc = 0;
        QMessageBox::information(this, "", "Нет отменённых действий!");
    }
    else
    {
        toCodes();
        showDocs();
    }
}

void txtFiles::on_pushButtonClearResult_clicked()
{
    converter.clearResult();
    converter.clearQrInfo();
    ui->labelCodesInDoc->clear();
    ui->labelResultCodes->clear();
    ui->labelResultItems->clear();
    showDocs();
    on_pushButtonShowResult_clicked();
}

void txtFiles::on_pushButtonMergeFiles_clicked()
{
    selectedRows.clear();
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();

    selectedRows.reserve(selectedItems.size());

    for(auto &i : ui->listWidget->selectedItems())
    {
        selectedRows.push_back(ui->listWidget->row(i));
    }

    Dialog *dialogWindow = new Dialog(MERGE_ITEMS, selectedItems);
    connect(dialogWindow, SIGNAL(sendResultString(QString)), this, SLOT(mergeItems(QString)));
    dialogWindow->exec();
}

void txtFiles::mergeItems(QString newName)
{
    QVector<int> test;
    converter.mergeItems(selectedRows, newName);
    showDocs();
}

void txtFiles::markItemsTable()
{
    // if(Инвойс не пустой)
    for(size_t row = 0; row < items.size(); row++)
    {
        QString tempName = converter.getQrItemName(currentDoc);
        QString tempQty = QString::number(converter.getQrQtyInItem(currentDoc));
        if((tempName.contains(items[row][0]) || tempName.contains(items[row][1])) && items[row][2] == tempQty)
        {
            QMessageBox::information(this, "", "Added codes contains in item row: " + QString::number(row));
            for(size_t col = 0; col < items[row].size(); col++)
            {
                QTableWidgetItem* item = ui->tableWidgetItems->item(row, col);
                QBrush bg(Qt::green);
                item->setBackground(bg);
            }
        }
//        else
//        {
//            QMessageBox::information(this, "", "Added codes not found in items table");
//        }
    }
}
