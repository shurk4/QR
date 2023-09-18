#include "xlsConverter.h"

using namespace OpenXLSX;

xlsConverter::xlsConverter()
{

}

void xlsConverter::readXls(std::wstring path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames)
{
    BasicExcel xls;

    xls.Load(&(path[0]));

    tempXls.resize(xls.GetTotalWorkSheets());
    for(int tab = 0; tab < xls.GetTotalWorkSheets(); tab++)
    {
        BasicExcelWorksheet* sheet = xls.GetWorksheet(tab); // получение нужной страницы из файла

        wchar_t* wName;
        std::wstring nameWS;

        char* name;
        std::string nameS;

        QString qTempName;

        if(xls.GetUnicodeSheetName(tab) == NULL)
        {
             std::string tempName = xls.GetAnsiSheetName(tab);

             qTempName = QString::fromStdString(tempName);
        }
        else
        {
            std::wstring tempName = xls.GetUnicodeSheetName(tab);

            qTempName = QString::fromStdWString(tempName);
        }

        sheetNames.push_back(qTempName);

        tempXls[tab].resize(sheet->GetTotalRows());
        for(int row = 0; row < sheet->GetTotalRows(); row++)
        {
            tempXls[tab][row].resize(sheet->GetTotalCols());
            for(int col = 0; col < sheet->GetTotalCols(); col++)
            {
                if(sheet->Cell(row, col)->GetWString() != NULL)
                {
                    tempXls[tab][row][col] = QString::fromStdWString(sheet->Cell(row, col)->GetWString());
                }
                else if(sheet->Cell(row, col)->GetString() != NULL)
                {
                    tempXls[tab][row][col] = sheet->Cell(row, col)->GetString();
                }
                else if(sheet->Cell(row, col)->GetDouble() != NULL)
                {
                    double cellData = sheet->Cell(row, col)->GetDouble();
                    double integer;
                    double fractial = std::modf(cellData, &integer);

                    if(fractial == 0)
                    {
                        tempXls[tab][row][col] = QString::number(integer, 'f', 0);
                    }
                    else
                    {
                        tempXls[tab][row][col] = QString::number(sheet->Cell(row, col)->GetDouble(), 'f', 2);
                    }
                }
                else if(sheet->Cell(row, col)->GetInteger() != NULL)
                {
                    tempXls[tab][row][col] = QString::number(sheet->Cell(row, col)->GetInteger());
                }
                else
                {
                    std::stringstream tempStream;
                    tempXls[tab][row][col] = QString::fromStdString(tempStream.str());
                }
            }
        }
    }
}

void xlsConverter::readXlsX(std::string path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames)
{
    int maxCols = 0;

    XLDocument doc;
    doc.open(path);

    auto sheets = doc.workbook().worksheetNames();

    for(auto it : sheets)
    {
        QVector<QVector<QString>> tempSheet;

        auto wks = doc.workbook().worksheet(it);
        sheetNames.push_back(QString::fromStdString(wks.name()));

        for (auto& row : wks.rows())
        {
            if(row.cellCount() <= 0) continue;

            QVector<QString> tempRow;
            for (auto& value : std::vector<XLCellValue>(row.values()))
            {
                QString tempString;

                if(value.typeAsString() == "empty")
                {
                    tempString = "\0";
                }
                else
                {
                    std::stringstream tempStream;
                    tempStream << value;
                    tempString = QString::fromStdString(tempStream.str());
                }

                tempRow.push_back(tempString);
            }
            if(tempRow.size() > maxCols) maxCols = tempRow.size();

            tempSheet.push_back(tempRow);
        }
        // Выравнивание количества столбцов
        for(int row = 0; row < tempSheet.size(); row++)
        {
            tempSheet[row].resize(maxCols);
        }
        // Выравнивание количества столбцов

        tempXls.push_back(tempSheet);
    }
}

void xlsConverter::clearInvoiceData()
{
    invoiceResult.clear();
    invoiceSheetSettings.clear();
    invoiceXls.clear();
    invoiceSheetNames.clear();
}

void xlsConverter::clearQrData()
{
    qrSheetSettings.clear();
    qrResult.clear();
    qrInfo.clear();
    qrXls.clear();
}

void xlsConverter::clearResult()
{
    result.clear();
    resultInfo.clear();

    addedItems.clear();
    undoItems.clear();
}

void xlsConverter::clearTxt()
{
    itemsQty = 0;
    ctnNumbers.clear();
    qrInfo.clear();
    qrCodes.clear();
    clear();
}

void xlsConverter::clear()
{
    clearInvoiceData();
    clearQrData();
    clearArt();
    clearResult();
}

bool xlsConverter::emptyCell(QString &str)
{
    return str.size() == 0 || str[0] == '\0' || str == "NO DATA";
}

bool xlsConverter::invoiceReady()
{
    return !invoiceResult.empty();
}

void xlsConverter::qrAnalyze(int tab)
{
    int item = qrSheetSettings[tab].itemCol;
    int qr = qrSheetSettings[tab].qrCol;
    QVector<QVector<QString>> newItemQr;
    QVector<QVector<QString>> &qrTab = qrXls[tab];

    for(int i = 0; i < qrTab.size(); i++)
    {
        if(qrTab[i][qr].size() < 31)
            continue;

        QrInfo tempInfo;
        tempInfo.name = qrTab[i][item];
        tempInfo.status = NEW;

        QVector<QString> code;
        QString line = qrTab[i][qr];
        code.push_back(line);
        newItemQr.push_back(code);

        if(i == qrTab.size() - 1 || (i + 1 < qrTab.size() && tempInfo.name != qrTab[i + 1][item]))
        {
            qrInfo.push_back(tempInfo);
            qrCodes.push_back(newItemQr);
            newItemQr.clear();
        }
    }
}

void xlsConverter::addQrTxt(const QString name, const QVector<QVector<QString>> &codes)
{
    QrInfo info;
    info.name = name;
    info.status = NEW;
    qrInfo.push_back(info);
    qrCodes.push_back(codes);
}

void xlsConverter::clearQrTxt()
{
    qrInfo.clear();
    qrCodes.clear();
}

void xlsConverter::mergeItems(const QVector<int> &selectedRows, QString &name)
{
    qDebug() << "Selected items:";

    for(auto i : selectedRows)
    {
        qDebug() << i;
    }

    qDebug() << "qrInfo size: " << qrInfo.size();
    int baseRow = selectedRows[0];
    qDebug() << "Base row: " << baseRow;
    qrInfo[baseRow].name = name;
    qDebug() << "New item name: " << qrInfo[baseRow].name;
    qDebug() << "qrCodes size: " << qrCodes.size();

    for(size_t i = 1; i < selectedRows.size(); i++)
    {
        qrCodes[baseRow].reserve(qrCodes[baseRow].size() + qrCodes[selectedRows[i]].size());

        for(auto &it : qrCodes[selectedRows[i]])
        {
            qrCodes[baseRow].push_back(it);
        }
    }

    qDebug() << "Qr merge complited";

    for(size_t i = 1; i < selectedRows.size(); i++)
    {
        qrInfo.erase(qrInfo.begin() + selectedRows[i]);
        qrCodes.erase(qrCodes.begin() + selectedRows[i]);
    }

    qDebug() << "Old item deleted";
}

bool xlsConverter::haveQrSettings(int tab)
{
    return qrSheetSettings[tab].itemCol > -1 && qrSheetSettings[tab].qrCol > -1;
}

bool xlsConverter::qrReady()
{
    return !qrCodes.empty();
}

bool xlsConverter::qrItemIndex(QString itemName, int &num)
{
    for(size_t i = 0; i < qrInfo.size(); ++i)
    {
        if(qrInfo[i].name.contains(itemName))
        {
            num = i;
            return true;
        }
    }
    return false;
}

// Получить количество кодов указанного итема(для поиска не по номеру использовать номер из qrInfo)
int xlsConverter::getQrQtyInItem(int item)
{
    return qrCodes[item].size();
}

// Получить количество итемов с обработанными кодами
int xlsConverter::getQrItemsQty()
{
    return qrInfo.size();
}

QString xlsConverter::getQrItemName(int item)
{
    return qrInfo[item].name;
}

QrStatus xlsConverter::getQrStatus(int item)
{
    if(item < qrInfo.size()) return qrInfo[item].status;
    else return EMPTY;
}

QVector<QVector<QString> > &xlsConverter::getQrItem(int item)
{
    return qrCodes[item];
}

void xlsConverter::addQrInfo(QrInfo info)
{
    qrInfo.push_back(info);
}

void xlsConverter::setQrInfoStatus(int item, QrStatus status)
{
    if(qrInfo.size() < item) qrInfo.resize(item + 1);
    qrInfo[item].status = status;
}

void xlsConverter::addItem(int item)
{
    addedItems.push_back(item);
    setQrInfoStatus(item, ADDED);
}

void xlsConverter::calculateQrArticules()
{
    if(!qrArtCalculated)
    {
        for(int row = 0; row < qrResult.size(); row++)
        {
            QString tempQr = qrResult[row][0];
            QString tempArt;

            // Создание артикула для сопоставления
            for(int col = 1; col < qrResult[row].size(); col++)
            {
                if(emptyCell(qrResult[row][col])) continue;
                tempArt += qrResult[row][col] + "_";
            }

            qrResult[row].clear();
            qrResult[row].push_back(tempQr);
            qrResult[row].push_back(tempArt);

            if(qrArticules.empty())
            {
                qrArticules.emplace(tempArt, 1);
            }
            else
            {
                auto it = qrArticules.find(tempArt);

                if(it == qrArticules.end())
                {
                    qrArticules.emplace(tempArt, 1);
                }
                else
                {
                    it->second++;
                }
            }
        }

        for(auto it : qrArticules)
        {
            QVector<QString> tempVec;
            tempVec.push_back(it.first);
            tempVec.push_back(QString::number(it.second));

            qrArtuculesVec.push_back(tempVec);
        }
    }
    qrArtCalculated = true;
}

void xlsConverter::clearArt()
{
    qrArticules.clear();
    qrArtuculesVec.clear();
    qrArtCalculated = false;
}

void xlsConverter::saveResult(std::wstring path)
{
    BasicExcel xls; // объявление переменной эксель

    // create sheet 1 and get the associated BasicExcelWorksheet pointer
    xls.New(1); // создание одной страницы
    BasicExcelWorksheet* sheet = xls.GetWorksheet(0); // получаем текущую рабочую страницу по индексу из созданной переменной эксель

    XLSFormatManager fmt_mgr(xls);
    CellFormat fmt(fmt_mgr);
    BasicExcelCell* cell;

    // HEADER
    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 1);
    wchar_t* numItem = L"№тов.";
    cell->Set(numItem);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 2);
    wchar_t* numGroup = L"№группы";
    cell->Set(numGroup);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 3);
    wchar_t* id = L"Идентификатор";
    cell->Set(id);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 4);
    wchar_t* codeType = L"Код вида";
    cell->Set(codeType);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 5);
    wchar_t* markLevel {L"Уровень маркировки"};
    cell->Set(markLevel);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 6);
    cell->Set("Item code");
    cell->SetFormat(fmt);
    // HEADER

    for(int i = 0; i < result.size(); i++)
    {
        int row = i + 1;

        for(int col = 0; col < result[i].size(); col++)
        {
            fmt.set_format_string(XLS_FORMAT_TEXT);
            cell = sheet->Cell(row, col);
            cell->Set(&(result[i][col].toStdWString()[0])); // ПРОВЕРИТЬ запись файла!!!
            cell->SetFormat(fmt);
        }
    }

    xls.SaveAs(&(path[0]));
}

void xlsConverter::saveContainers(std::wstring patch)
{
    BasicExcel xls; // объявление переменной эксель

    // create sheet 1 and get the associated BasicExcelWorksheet pointer
    xls.New(1); // создание одной страницы
    BasicExcelWorksheet* sheet = xls.GetWorksheet(0); // получаем текущую рабочую страницу по индексу из созданной переменной эксель

    XLSFormatManager fmt_mgr(xls);
    CellFormat fmt(fmt_mgr);
    BasicExcelCell* cell;

    // HEADER
    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 0);
    wchar_t* numItem = L"Номер товара";
    cell->Set(numItem);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 1);
    wchar_t* numGroup = L"Текст1";
    cell->Set(numGroup);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 2);
    wchar_t* id = L"Идентификатор текста";
    cell->Set(id);
    cell->SetFormat(fmt);

    fmt.set_format_string(XLS_FORMAT_TEXT);
    cell = sheet->Cell(0, 3);
    wchar_t* codeType = L"Тип измерения";
    cell->Set(codeType);
    cell->SetFormat(fmt);
    // HEADER

    for(int i = 0; i < result.size(); i++)
    {
        int row = i + 1;

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 0);
//        wchar_t* numItem = L"Номер товара";
        cell->Set(&(result[i][0].toStdWString()[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 1);
//        wchar_t* numGroup = L"Текст1";
        cell->Set(&(result[i][1].toStdWString()[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 2);
//        wchar_t* id = L"Идентификатор текста";
        cell->Set(&(result[i][2].toStdWString()[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 3);
//        wchar_t* codeType = L"Тип измерения";
        cell->Set(&(result[i][3].toStdWString()[0]));
        cell->SetFormat(fmt);
    }

    xls.SaveAs(&(patch[0]));
}

void xlsConverter::saveTable(const QVector<QVector<QString>> &table, const std::wstring path)
{
    BasicExcel xls; // объявление переменной эксель

    // create sheet 1 and get the associated BasicExcelWorksheet pointer
    xls.New(1); // создание одной страницы
    BasicExcelWorksheet* sheet = xls.GetWorksheet(0); // получаем текущую рабочую страницу по индексу из созданной переменной эксель

    XLSFormatManager fmt_mgr(xls);
    CellFormat fmt(fmt_mgr);
    fmt.set_format_string(XLS_FORMAT_TEXT);

    BasicExcelCell* cell;

    for(int row = 0; row < table.size(); row++)
    {
        for(int col = 0; col < table[row].size(); col++)
        {
            cell = sheet->Cell(row, col);
            cell->Set(&(table[row][col].toStdWString()[0]));
            cell->SetFormat(fmt);
        }
    }
    xls.SaveAs(&(path[0]));
}

int xlsConverter::getItemsPosQty()
{
    return invoiceResult.size();
}

int xlsConverter::getCtnsQty()
{
    if(ctnNumbers.empty()) return 1;
    return ctnNumbers.size();
}

int xlsConverter::getItemsQty()
{
    return itemsQty;
}

QVector<QVector<QString> > xlsConverter::getInvoiceResult()
{
    return invoiceResult;
}

bool xlsConverter::invoiceEmpty()
{
    return invoiceXls.empty();
}

bool xlsConverter::resultEmpty()
{
    return result.empty();
}

QVector<QVector<QString> > xlsConverter::getItemsBasic(int tab)
{
    if(!invoiceSheetSettings.empty()
        && (invoiceSheetSettings[tab].qtyCol > 0
            || invoiceSheetSettings[tab].qtyCol < invoiceXls[tab][0].size()))
    {
        clearInvoiceResult();

        int colsNum;
        int stopRow = invoiceXls[tab].size();

        if(invoiceSheetSettings[tab].stopRow > -1
            &&invoiceSheetSettings[tab].stopRow > invoiceSheetSettings[tab].startRow)
        {
            stopRow = invoiceSheetSettings[tab].stopRow + 1;
        }
        std::vector<int> notEmptyCells;

        // поиск строк товаров
        for(int row = invoiceSheetSettings[tab].startRow; row < stopRow; row++)
        {
            QVector<QString> tempRow;
            bool notEmpty = true;

            if(row == invoiceSheetSettings[tab].startRow)
            {
                for(int col = 0; col < invoiceXls[tab][row].size(); col++)
                {
                    if(!Extras::emptyCell(invoiceXls[tab][row][col]))
                    {
                        QString cellData = invoiceXls[tab][row][col];
                        notEmptyCells.push_back(col);
                        tempRow.push_back(cellData);
                        if(col == invoiceSheetSettings[tab].qtyCol)
                        {
                            itemsQty += cellData.toInt();
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
                    QString cellData = invoiceXls[tab][row][notEmptyCells[i]];

                    if(Extras::emptyCell(cellData))
                    {
                        emptyRowsCounter++;
                    }

                    if(notEmptyCells[i] == invoiceSheetSettings[tab].qtyCol && !Extras::itQty(cellData.toStdString()))
                    {
                        notEmpty = false;
                    }

                    tempRow[i] = cellData;

                    if(notEmptyCells[i] == invoiceSheetSettings[tab].qtyCol)
                    {
                        itemsQty += cellData.toInt();
                        swap(tempRow[i], tempRow[0]);
                    }
                }
                if(emptyRowsCounter > notEmptyCells.size() / 2) notEmpty = false;
            }

            if(notEmpty)
            {
                invoiceResult.push_back(tempRow);
            }
        }
    }
    else
    {
        QVector<QString> col{"No items"};
        invoiceResult.push_back(col);
    }

    return invoiceResult;
}

QVector<QVector<QString>> xlsConverter::getItemsForTxt(int tab)
{
    invoiceResult.clear();

    int qtyCol = invoiceSheetSettings[tab].qtyCol;
    int qtyFirstCell = invoiceSheetSettings[tab].startRow;
    int qtyLastCell = invoiceSheetSettings[tab].stopRow;
    int itemCol = invoiceSheetSettings[tab].itemCol;

    if(qtyCol == -1 || qtyFirstCell == -1 || qtyLastCell == - 1 ||  itemCol == -1) return invoiceResult;

    for(size_t row = qtyFirstCell - 1; row <= qtyLastCell + 1; row++)
    {
        for(size_t col = 0; col < invoiceXls[tab][row].size(); col++)
        {
            if(Extras::isCtnNumber(invoiceXls[tab][row][col]))
            {
                ctnNumbers.push_back(invoiceXls[tab][row][col]);
            }
        }
    }

    if(invoiceXls.empty() || invoiceXls[tab].empty()) return invoiceResult;

    int currentCtn = 0;
    for(size_t row = qtyFirstCell; row <= qtyLastCell; row++)
    {
        QVector<QString> rowVec;
        rowVec.resize(3);
        bool founded = false;

        for(size_t col = 0; col < invoiceXls[tab][row].size(); col++)
        {
            if(Extras::isCtnNumber(invoiceXls[tab][row][col]))
            {
                currentCtn++;
            }

            if(!ctnNumbers.empty()) rowVec[0] = ctnNumbers[currentCtn];
            else rowVec[0] = "        -        ";

            if(col == itemCol && invoiceXls[tab][row][col].size() > 0 && Extras::itQty(invoiceXls[tab][row][qtyCol].toStdString()))
            {
                rowVec[1] = invoiceXls[tab][row][col];
                founded = true;
            }
            else if(col == qtyCol)
            {
                if(invoiceXls[tab][row][col].size() > 0 && Extras::itQty(invoiceXls[tab][row][col].toStdString()))
                {
                    rowVec[2] = invoiceXls[tab][row][col];
                    itemsQty += invoiceXls[tab][row][col].toInt();
                    founded = true;
                }
            }
        }
        if(founded) invoiceResult.push_back(rowVec);
    }
    return invoiceResult;
}

void xlsConverter::clearInvoiceResult()
{
    invoiceResult.clear();
}

void xlsConverter::clearQrInfo()
{
    if(qrInfo.empty()) return;

    for(auto &it : qrInfo)
    {
        it.status = EMPTY;
    }
}

void xlsConverter::addInvoiceResultRow(const QVector<QString> row)
{
    invoiceResult.push_back(row);
}

int xlsConverter::undoResult()
{
    if(addedItems.empty() || result.empty())
    {
        return -1;
    }

    int itemInvoiceNumber = addedItems.back();
    addedItems.erase(addedItems.end() - 1);
    undoItems.push_back(itemInvoiceNumber);

    int itemResultNumber = result.back()[1].toInt();
    while(!result.empty() && result.back()[1].toInt() == itemResultNumber)
    {
        result.pop_back();
    }

    setQrInfoStatus(itemInvoiceNumber, DELETED);
    return itemInvoiceNumber;
}

int xlsConverter::redoResult()
{
    if(undoItems.empty())
    {
        return -1;
    }

    int undoItem = undoItems.back();
    undoItems.pop_back();
    return undoItem;
}
