#include "txtfiles.h"
#include "qdatetime.h"
#include "ui_txtfiles.h"

txtFiles::txtFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::txtFiles)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    this->resize(1280, 720);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);

    this->styleSheet().clear();
    setStyle();

    readConfig();
    ui->splitter->setStretchFactor(0, 1); // столбец с ид 0 занимает всё доступное простанство
    readReg();

    hideFilesWidgets();
    ui->widgetItemsInfo->hide();
    ui->widgetAddItems->hide();

    helpWindow = new help(this);
    helpWindow->showTextfilesHelp();
    helpWindow->setModal(false);
    helpWindow->hide();

    ui->tableWidget_1->setContextMenuPolicy(Qt::CustomContextMenu);
    // Подключение сигнала контекстного меню таблицы к слоту отображения меню окна
    connect(ui->tableWidget_1, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    createInvMenu();
    createQrMenu();
}

void txtFiles::setStyle()
{
    qDebug("setStyle");
    QFile styleF;
    styleF.setFileName("://res/txtStyle.css");

    if(styleF.open(QFile::ReadOnly))
    {
        QString qssStr = styleF.readAll();
        this->setStyleSheet(qssStr);
        styleF.close();
        qDebug("Стили установлены");
    }
    else qDebug("Не удалось открыть файл стилей");

        ui->tableWidget_1->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_1->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_1-> setAlternatingRowColors(true);
}

txtFiles::~txtFiles()
{
    writeReg();
    config->write();
    toLog("Работа завершена.");
    delete config;
    delete helpWindow;
    delete ui;
}

void txtFiles::readReg()
{
    QSettings settings("ShurkSoft", "QR to TKS");
    settings.beginGroup("txt");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("mainSplitter").toByteArray());
    settings.endGroup();
    toLog("Конфигурация загрежена.");
}

void txtFiles::writeReg()
{
    QSettings settings("ShurkSoft", "QR to TKS");
    settings.beginGroup("txt");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("mainSplitter", ui->splitter->saveState());
    settings.endGroup();
    toLog("Кофигурация сохранена.");
}

void txtFiles::readConfig()
{
    config = new UserConfig;
    QString temp;
    //Последний открытый путь к файлам
    toLog("Установка последнего использованный пути к файлу.");
    if(config->get("lastPath", temp))
    {
        QDir dir(temp);

        lastPath.setPath(temp);

        toLog(temp + " \n.");
    }
    else
    {
        lastPath.setPath(QDir::homePath());

        toLog(QDir::homePath() + " \n.");
    }
}

void txtFiles::hideFilesWidgets()
{
    ui->widgetTabsPanel->hide();
    ui->WidgetInvAnalyze->hide();
    ui->widgetQrButtons->hide();
    toLog("Спрятаны:");
    toLog("_widgetTabsPanel");
    toLog("_WidgetInvAnalyze");
    toLog("_widgetQrButtons");
}

void txtFiles::setDisplayType(DisplayType type)
{
    hideFilesWidgets();

    toLog("setDisplayType - установка набора виджетов:");

    switch (type)
    {
    case (INV):
        toLog("-показан INV");
        ui->widgetTabsPanel->show();
        ui->WidgetInvAnalyze->show();        
        toLog("--widgetTabsPanel");
        toLog("--WidgetInvAnalyze");
        displayType = INV;
        break;

    case (QR):
        toLog("-показан QR");
        ui->widgetTabsPanel->show();
        ui->widgetQrButtons->show();
        toLog("--widgetTabsPanel");
        toLog("--widgetQrButtons");
        displayType = QR;
        break;

    case (TXT):
        toLog("-показан TXT");
        displayType = TXT;
        break;

    case (RESULT):
        toLog("-показан RESULT");
        ui->tableWidget_1->scrollToBottom();
        displayType = RESULT;
        toLog("--tableWidget_1");
        break;

    default:
        break;
    }
    toLog("");
}

void txtFiles::showTable_1(const QVector<QVector<QString>> &inTab)
{
    toLog("Вывод таблицы showTable_1.");
    switch (displayType) {
    case INV:
        toLog("  -- Таблица инвойса. Текущая вкладка: " + QString::number(currentTab));
        ui->labelTab->setText(QString::number(currentTab + 1));
        ui->labelTabs->setText(QString::number(converter.invoiceXls.size()));
        if(!converter.invoiceSheetNames.empty()) ui->labelTabName->setText(converter.invoiceSheetNames[currentTab]);
        break;
    case QR:
        toLog("  -- Таблица QR кодов. Текущая вкладка: " + QString::number(currentTabQr));
        ui->labelTab->setText(QString::number(currentTabQr + 1));
        ui->labelTabs->setText(QString::number(converter.qrXls.size()));
        if(!converter.qrSheetNames.empty())
        {
            ui->labelTabName->setText(converter.qrSheetNames[currentTabQr]);
        }
        else toLog("!!! --- !!! Таблица с названиями страниц таблицы QR пустая!!!");
        break;

    default:
        break;
    }

    Extras::showTable(inTab, ui->tableWidget_1);
    toLog("Таблица отображена.");
//    switch (displayType) {
//    case INV:
//        toLog("Форматирования таблицы инвойса");
//        if(converter.haveInvCellsSettings())
//        {
//            Extras::setTableSettings(converter.getInvPageCellsSettings(currentTab), ui->tableWidget_1);
//        }
//        else toLog("Нет данных форматирования");
//        break;
//    case QR:
//        toLog("Форматирования таблицы QR кодов");
//        if(converter.haveQrCellsSettings())
//        {
//            Extras::setTableSettings(converter.getQrPageCellsSettings(currentTabQr), ui->tableWidget_1);
//        }
//        else toLog("Нет данных форматирования");
//        break;

//    default:
//        break;
//    }
}

void txtFiles::showTable_2(const QVector<QVector<QString>> &table)
{
    toLog("Вывод таблицы showTable_2.");
    // Вывести данные в таблицу
    Extras::showTable(table, ui->tableWidget_1);
    toLog("Вывод таблицы showTable_1.");
}

void txtFiles::showDocs()
{
    toLog("Вывод имеющихся списков кодов.");
    currentDoc = 0;

    qDebug() << "showDocs";
    if(!converter.qrReady())
    {
        QMessageBox::information(this, "!", "Не добавлено ни одного документа!");
        return;
    }

    ui->labelCodesInDoc->setText(QString::number(converter.getQrQtyInItem(currentDoc)));
    qDebug() << "converter.getQrQtyInItem(currentDoc): " << converter.getQrQtyInItem(currentDoc);

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

void txtFiles::toLog(QString _log)
{
    QString logString = "Text Files: " + _log;
    emit log(logString);
}

void txtFiles::showContextMenu(QPoint pos)
{
    toLog("Вызвано контекстное меню");
    switch (displayType) {
    case INV:
        invMenu->popup(ui->tableWidget_1->viewport()->mapToGlobal(pos));
        break;
    case QR:
        qrMenu->popup(ui->tableWidget_1->viewport()->mapToGlobal(pos));
        break;
    case TXT:
        break;
    case RESULT:
        break;
    }
}

void txtFiles::on_pushButtonInv_clicked()
{    
    toLog("Открытие файла инвойса");
    converter.clearInvoiceData();
    QString invPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл инвойса", lastPath.absolutePath(), "*.xls *.xlsx");

    if(invPathTemp.isEmpty())
    {
        toLog("Файл не выбран");
        ui->labelInv->setText("Выберите файл!");
    }
    else
    {
        toLog("Выбран файл: " + invPathTemp);
        setDisplayType(INV);
        lastPath.setPath(invPathTemp);
        config->set("lastPath", lastPath.absolutePath());

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists())
        {
            toLog("Создана папка для временных копий открываемых файлов");
            QDir().mkdir("temp"); // Создание папки temp
        }

        // определение разрешения
        if(invPathTemp.right(4) == "xlsx" || invPathTemp.right(4) == "XLSX")
        {
            ext = ".xlsx";
        }
        else
        {
            ext = ".xls";
        }        

        toLog("Подготовка к копированию временного файла");
        QString invoiceFileName = "temp/inv" + date.toString("ddMMyy") + time.toString("hhmm") + ext; // Имя копии файла

        copySucces = QFile::copy(invPathTemp, invoiceFileName);

        if(!copySucces)
        {
            QMessageBox::information(this, "Открытие файла", "Не удалось создать временную копию файла!");
            toLog("Не удалось создать временную копию файла");
            return;
        }

        if(ext == ".xlsx")
        {
            std::string path = invoiceFileName.toStdString();
            converter.readXlsX(path, converter.invoiceXls, converter.invoiceSheetNames);
        }
        else
        {
            std::wstring path = invoiceFileName.toStdWString();
            converter.readXls(path, converter.invoiceXls, converter.invoiceSheetNames, TypeINV);
        }

        ui->labelInv->setText(lastPath.dirName());

        QFile::remove(invoiceFileName);
        toLog("Файл инвойса прочитан, копия файла удалена");

        // !!! ТАБЛИЦА !!!
        converter.invoiceSheetSettings.resize(converter.invoiceXls.size());
        currentTab = 0;
        showTable_1(converter.invoiceXls[currentTab]);
        toLog("Данные файла инвойса выведены в таблицу");

        invoiceShowed = true;
        ui->pushButtonShow->setText("Результат");
    }
}

void txtFiles::on_pushButtonTxt_clicked()
{
    toLog("Открытие текстовых файлов");
    // Получение списка файлов
    fileNames = QFileDialog::getOpenFileNames(nullptr, "Выбрать текстовые файлы", lastPath.absolutePath() , "Текстовые файлы(*.txt)");

    if(!fileNames.isEmpty())
    {
        toLog("Выбраны файлы:");
        lastPath.setPath(fileNames[0]);
        // Получение данных из списка файлов
        for (auto xfile : fileNames)
        {
            QFile file (xfile);
            // Получение имени файла
            QFileInfo info(file.fileName());
            toLog("   - " + info.absoluteFilePath());

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
            toLog("   -- Обработан");
        }

        showTable_2(converter.getQrItem(currentDoc));

        showDocs();
    }
    else
    {
        toLog("Файлы не выбраны");
    }
}

void txtFiles::on_pushButtonOpenQr_clicked()
{
    toLog("Открытие таблицы с QR кодами.");
    converter.qrXls.clear();
    converter.qrSheetNames.clear();
    setDisplayType(QR);

    QString qrPathTemp = QFileDialog::getOpenFileName(this, "Выберите файл QR", lastPath.absolutePath(), "*.xls *.xlsx");
    if(qrPathTemp.isEmpty())
    {
        toLog("Файл не выбран");
        ui->labelInv->setText("Выберите файл!");
    }
    else
    {
        toLog("Выбран файл: " + qrPathTemp);
        lastPath.setPath(QFileInfo(qrPathTemp).absolutePath());
        config->set("lastPath", lastPath.absolutePath());

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString ext; // расширение файла
        bool copySucces = false;

        if(!QDir("temp").exists())
        {
            toLog("Создана папка для временных копий открываемых файлов");
            QDir().mkdir("temp"); // Создание папки temp
        }

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

        if(!copySucces)
        {
            QMessageBox::information(this, "Открытие файла", "Не удалось создать временную копию файла!");
            toLog("Не удалось создать копию файла");
        }

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
        toLog("Таблица с QR кодами прочитана, копия файла удалена");

        // !!! ТАБЛИЦА !!!
        currentTabQr = 0;
        converter.qrSheetSettings.resize(converter.qrXls.size());
        //        selectedQrCols.resize(converter.qrXls.size());

        //        showQr = true;
        showTable_1(converter.qrXls[currentTabQr]);
        toLog("QR коды выведены в таблицу");
    }
}

void txtFiles::on_pushButtonDown_clicked()
{
    switch (displayType)
    {
    case INV:
        if(currentTab > 0 && converter.invoiceXls.size() > 0)
        {
            toLog("Переключение страницы таблицы инфвойса вниз.");
            currentTab--;
            showTable_1(converter.invoiceXls[currentTab]);
        }
        break;
    case QR:
        if(currentTabQr > 0 && converter.qrXls.size() > 0)
        {
            toLog("Переключение страницы таблицы Qr вниз.");
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
            toLog("Переключение страницы таблицы инфвойса вверх.");
            currentTab++;
            showTable_1(converter.invoiceXls[currentTab]);
        }
    case QR:
        if(currentTabQr < converter.qrXls.size() - 1 && converter.qrXls.size() > 0)
        {
            toLog("Переключение страницы таблицы Qr вверх.");
            currentTabQr++;
            showTable_1(converter.qrXls[currentTabQr]);
        }
        break;
    default:
        break;
    }
}

void txtFiles::on_pushButtonSave_clicked()
{
    toLog("Сохранение результата.");
    if(converter.result.size() > 0)
    {
        lastPath = QFileInfo(lastPath.path()).absolutePath() + "/result.xls";

        toLog("Путь сохранения: " + lastPath.absolutePath());
        QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить результат"), lastPath.absolutePath(), tr("Таблица xls (*.xls)"));
        if (fileName != "")
        {
            try
            {
                std::wstring path = fileName.toStdWString();
                converter.saveResult(path);
                QMessageBox::information(this, "!", "Файл сохранён!");
                toLog("Результат сохранен.");
            }
            catch (...)
            {
                QMessageBox::critical(this, "!", "Не удалось сохранить файл!");
                toLog("Не удалось сохранить результат.");
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
        toLog("Нажата кнопка отображения результатаю");
        showTable_1(converter.result);
        ui->pushButtonShow->setText("Инвойс");
        invoiceShowed = false;
    }
    else if(!invoiceShowed && converter.invoiceXls.size() > 0)
    {        
        toLog("Нажата кнопка отображения файла инвоса.");
        showTable_1(converter.invoiceXls[currentTab]);
        ui->pushButtonShow->setText("Результат");
        invoiceShowed = true;
    }
}

void txtFiles::on_listWidget_itemClicked(QListWidgetItem *item)
{
    toLog("Нажат итем в списке кодов");
    currentDoc = ui->listWidget->currentRow();
    QVector<QVector<QString>> tempItem = converter.getQrItem(currentDoc);
    showTable_2(tempItem);
    ui->labelCodesInDoc->setText(QString::number(converter.getQrQtyInItem(currentDoc)));
    setDisplayType(TXT);
}

// Добавить коды из файла
void txtFiles::toCodes()
{
    toLog("Добавление QR в список кодов.");
    if(converter.getQrStatus(currentDoc) == ADDED)
    {
        toLog("Эти данные уже добавлены.");
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

        toLog("Формирование данных в формат результата.");
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
        toLog("Данные сформированы и добавлены в результат.");

        showTable_1(converter.result);
        toLog("Таблица результата выведена на экран.");
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
    toLog("Нажата кнопка pushButtonAddQr");
    toCodes();
}

void txtFiles::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    toLog("Дважды клинкнут итем в списке кодов.");
    toCodes();
}

void txtFiles::on_pushButtonFirstCell_clicked()
{
    invoiceFirstCell();
}

void txtFiles::on_pushButtonLastCell_clicked()
{
    invoiceLastCell();
}

void txtFiles::on_pushButtonAnalyze_clicked()
{
    invoiceAnalize();
}

void txtFiles::on_pushButtonItemCol_clicked()
{
    invoiceItemCol();
}


void txtFiles::on_pushButtonQrCol_clicked()
{
    qrQrCol();
}

void txtFiles::on_pushButtonQrItemCol_clicked()
{
    qrItemCol();
}

void txtFiles::on_pushButtonQrAnalyze_clicked()
{
    qrAnalize();
}

void txtFiles::on_pushButtonHelp_clicked()
{
    toLog("Нажата кнопка помощи.");
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
    toLog("Нажата кнопка pushButtonClearAll - полная очистка программы.");
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

void txtFiles::on_pushButtonClearFiles_clicked()
{
    toLog("Нажата кнопка pushButtonClearFiles - очистка выборки QR кодов");
    ui->listWidget->clear();
    converter.clearResult();
    converter.clearQrTxt();
    toLog("Данные очищены.");

}

void txtFiles::on_pushButtonClearItems_clicked()
{
    toLog("Нажата кнопка pushButtonClearItems - очистка обработанных данных из инвойса о товарах.");
    converter.clearInvoiceData();
    items.clear();
    selectedRows.clear();
    ui->tableWidgetItems->clear();
    ui->widgetItemsInfo->hide();
}

void txtFiles::on_pushButtonCancelUndoResult_clicked()
{
    toLog("Нажата кнопка отмены добавления кодов.");
    if(converter.undoResult() == -1)
    {
        QMessageBox::information(this, "", "Таблица результатов пуста!");
        toLog("Таблица результатов пуста!");
    }

    else
    {
        itemNum--;
        ui->labelResultItems->setText(QString::number(ui->labelResultItems->text().toInt() - 1));
        ui->labelResultCodes->setText(QString::number(ui->labelResultCodes->text().toInt() - converter.getQrQtyInItem(currentDoc)));
        showDocs();
        if(itemNum > 0) showResult();
    }
}

void txtFiles::on_pushButtonUndoRedoResult_clicked()
{
    toLog("Нажата кнопка повторного добавления кодов после отмены.");
    currentDoc = converter.redoResult();
    if(currentDoc == -1)
    {
        currentDoc = 0;
        QMessageBox::information(this, "", "Нет отменённых действий!");
        toLog("Нет отменённых действий!");
    }
    else
    {
        toCodes();
        showDocs();
    }
}

void txtFiles::on_pushButtonClearResult_clicked()
{
    toLog("Нажата кнопка pushButtonClearResult - полная очистка таблицы результата.");
    converter.clearResult();
    converter.clearQrInfo();
    ui->labelCodesInDoc->clear();
    ui->labelResultCodes->clear();
    ui->labelResultItems->clear();
    showDocs();
    showResult();
}

void txtFiles::on_pushButtonMergeFiles_clicked()
{
    toLog("Нажата кнопка pushButtonMergeFiles - объединение QR кодов");
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
    toLog("Получен результат объединения кодов, новое имя: " + newName);
    QVector<int> test;
    converter.mergeItems(selectedRows, newName);
    showDocs();
}

void txtFiles::markItemsTable()
{
    toLog("markItemsTable - раскраска таблицы с товарами");
    // if(Инвойс не пустой)
    for(size_t row = 0; row < items.size(); row++)
    {
        QString tempName = converter.getQrItemName(currentDoc);
        QString tempQty = QString::number(converter.getQrQtyInItem(currentDoc));
        if((tempName.contains(items[row][0]) || tempName.contains(items[row][1])) && items[row][2] == tempQty)
        {
            for(size_t col = 0; col < items[row].size(); col++)
            {
                QTableWidgetItem* item = ui->tableWidgetItems->item(row, col);
                QBrush bg(Qt::green);
                item->setBackground(bg);
            }
        }
    }
}

void txtFiles::createInvMenu()
{
    invMenu = new QMenu();
    invMenu->addAction(ui->actionItemQtyFirstCell);
    invMenu->addAction(ui->actionItemQtyLastCell);
    invMenu->addAction(ui->actionInvQtyRange);
    invMenu->addSeparator();
    invMenu->addAction(ui->actionInvItemNameCol);
    invMenu->addSeparator();
    invMenu->addAction(ui->actionInvAnalize);
}

void txtFiles::createQrMenu()
{
    qrMenu = new QMenu();
    qrMenu->addAction(ui->actionQrItemCol);
    qrMenu->addAction(ui->actionQrQrCol);
    qrMenu->addSeparator();
    qrMenu->addAction(ui->actionQrAnalize);
}

void txtFiles::showResult()
{
    toLog("Отображение результата.");
    if(converter.resultEmpty()) QMessageBox::information(this, "", "Таблица результатов пуста!");
    showTable_1(converter.result);
}

void txtFiles::invoiceFirstCell()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        int row = ui->tableWidget_1->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].qtyCol = col;
        converter.invoiceSheetSettings[currentTab].startRow = row;

        ui->labelInvCol->setText("Диапазон кол-ва Кол: " + QString::fromStdString(Extras::IntToSymbol(col)));
//        ui->labelRowFirst->setText(QString::number(row + 1));
        ui->pushButtonFirstCell->setText("Первая - " + QString::number(row + 1));
        toLog("Выбрана первая ячейка количества.\n   Колонка: " + QString::number(col) + " Строка: " + QString::number(row));
    }
}

void txtFiles::invoiceLastCell()
{
    if(converter.invoiceXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int row = ui->tableWidget_1->selectionModel()->currentIndex().row();
        // Взять данные из выбранной ячейки

        converter.invoiceSheetSettings[currentTab].stopRow = row;
//        ui->labelRowEnd->setText(QString::number(row + 1));
        ui->pushButtonLastCell->setText("Последняя - " + QString::number(row + 1));
        toLog("Выбрана последняя ячейка количества\n   Строка: " + QString::number(row));
    }
}

void txtFiles::invoiceRangeCell()
{
    QModelIndexList indexes = ui->tableWidget_1->selectionModel()->selectedIndexes();
    toLog("Выбран диапазон ячеек: ");

    // данные первого значения диапазона
    int col = indexes[0].column();
    int firstRow = indexes[0].row();
    int lastRow = indexes[indexes.size() - 1].row();

    // данные последнего значения диапазона


    converter.invoiceSheetSettings[currentTab].qtyCol = col;
    converter.invoiceSheetSettings[currentTab].startRow = firstRow;
    converter.invoiceSheetSettings[currentTab].stopRow = lastRow;

    ui->labelInvCol->setText("Диапазон кол-ва Кол: " + QString::fromStdString(Extras::IntToSymbol(col)));
    ui->pushButtonFirstCell->setText("Первая - " + QString::number(firstRow + 1));
//    ui->labelRowFirst->setText(QString::number(firstRow + 1));
    ui->pushButtonLastCell->setText("Последняя - " + QString::number(lastRow + 1));
//    ui->labelRowEnd->setText(QString::number(lastRow + 1));
}

void txtFiles::invoiceItemCol()
{
    int col = ui->tableWidget_1->selectionModel()->currentIndex().column();

    toLog("Выбрана колонка итемов: " + QString::number(col));
    converter.invoiceSheetSettings[currentTab].itemCol = col;
//    ui->labelItemCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
    ui->pushButtonItemCol->setText("Колонка итем - " + QString::number(col));
}

void txtFiles::invoiceAnalize()
{
    toLog("Нажата кнопка pushButtonAnalyze - анализ данных инвойса");
    items = converter.getItemsForTxt(currentTab);
    if(items.empty())
    {
        QMessageBox::critical(this, "", "Не отмечены поля для выборки данных");
        toLog("Не отмечены поля для выборки данных.");
    }
    else
    {
        toLog("Анализ данных из инвойса.");
        Extras::showTable(items, ui->tableWidgetItems);
        ui->tableWidgetItems->setHorizontalHeaderLabels(QStringList() << "Ctn num" << "Name" << "Qty");
        ui->tableWidgetItems->verticalHeader()->setVisible(false);
        bool setColor = false;

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
                    item->setBackgroundColor("#e0ecff");
                }
            }
        }

        setDisplayType(TXT);

        ui->widgetItemsInfo->show();
        toLog("Вывод данных о товарах в таблицу.");

        ui->tableWidgetItems->horizontalHeader()->setStretchLastSection(true);

        ui->labelItemsNum->setText(QString::number(converter.getItemsPosQty()));
        ui->labelCtnsNum->setText(QString::number(converter.getCtnsQty()));
        ui->labelOverallItems->setText(QString::number(converter.getItemsQty()));
    }
}

void txtFiles::qrQrCol()
{
    if(converter.qrXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        // Взять данные из выбранной ячейки

        converter.qrSheetSettings[currentTabQr].qrCol = col;
        ui->labelQrCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
        toLog("Выбрана колонка QR кодов: " + QString::number(col));
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
        toLog("Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

void txtFiles::qrItemCol()
{
    if(converter.qrXls.size() > 0)
    {
        // Взять данные из выбранной ячейки
        int col = ui->tableWidget_1->selectionModel()->currentIndex().column();
        // Взять данные из выбранной ячейки

        converter.qrSheetSettings[currentTabQr].itemCol = col;
        ui->labelQrItemCol->setText(QString::fromStdString(Extras::IntToSymbol(col)));
        toLog("Выбрана колонка итемов для сопоставления QR кодов: " + QString::number(col));
    }
    else
    {
        QMessageBox::information(this, "!?", "Файл не открыт или отображается недопустимая для этого действия таблица");
        toLog("Файл не открыт или отображается недопустимая для этого действия таблица");
    }
}

void txtFiles::qrAnalize()
{
    toLog("Нажата кнопка: pushButtonQrAnalyze - анализ данных в таблице QR кодов.");
    if(converter.haveQrSettings(currentTabQr))
    {
        converter.qrAnalyze(currentTabQr);
        toLog("Анализ завершён.");

        showDocs();
        ui->widgetQrButtons->hide();
    }
    else
    {
        if(converter.qrSheetSettings[currentTabQr].itemCol <= -1)
            QMessageBox::information(this, "!", "Не выбрана калонка Item!");
        else
            QMessageBox::information(this, "!", "Не выбрана колонка QR");

        toLog("Не выбраны данные для анализа.");
    }
    toLog("\n");
}

void txtFiles::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void txtFiles::on_actionItemQtyFirstCell_triggered()
{
    invoiceFirstCell();
}

void txtFiles::on_actionItemQtyLastCell_triggered()
{
    invoiceLastCell();
}

void txtFiles::on_actionInvQtyRange_triggered()
{
    invoiceRangeCell();
}

void txtFiles::on_actionInvAnalize_triggered()
{
    invoiceAnalize();
}

void txtFiles::on_actionInvItemNameCol_triggered()
{
    invoiceItemCol();
}

void txtFiles::on_actionQrQrCol_triggered()
{
    qrQrCol();
}

void txtFiles::on_actionQrItemCol_triggered()
{
    qrItemCol();
}

void txtFiles::on_actionQrAnalize_triggered()
{
    qrAnalize();
}
