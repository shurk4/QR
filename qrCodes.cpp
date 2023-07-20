#include "qrCodes.h"

using namespace ExcelFormat;

//std::vector<entry> codes
Result::Result(QR &qrIn, Invoice &invoiceIn)
{
    qr.startRow = qrIn.startRow;
    qr.stopRow = qrIn.stopRow;
    qr.itemCol = qrIn.itemCol;
    qr.qrCol = qrIn.qrCol;

    std::cout << "Start row = " << qr.startRow << "\n"
              << "Stop row = " << qr.stopRow << "\n"
              << "ItemCol = " << qr.itemCol << "\n"
              << "qrCol = " << qr.qrCol << "\n";

    qr.xlsArr = qrIn.xlsArr;

    invoice.startRow = invoiceIn.startRow;
    invoice.stopRow = invoiceIn.stopRow;
    invoice.itemCol = invoiceIn.itemCol;
    invoice.qtyCol = invoiceIn.qtyCol;

    std::cout << "Start row = " << invoice.startRow << "\n"
              << "Stop row = " << invoice.stopRow << "\n"
              << "ItemCol = " << invoice.itemCol << "\n"
              << "qrCol = " << invoice.qtyCol << "\n";

    invoice.xlsArr = invoiceIn.xlsArr;
}

void Result::getResult()
{
    resultData.reserve(qr.xlsArr.size());
    int itemNum = 1;
    std::cout << "\nGetting result\n";
    for(int i = invoice.startRow; i <= invoice.stopRow; i++) // меньше заменено на <=
    {
        std::cout << "Item code: " << invoice.xlsArr[i][invoice.itemCol] << " qty: " << invoice.xlsArr[i][invoice.qtyCol] << "\t";
        int qty = std::stoi(invoice.xlsArr[i][invoice.qtyCol]);
        int startQty = qty;
        while(qty > 0)
        {
            bool notFound = true;
            int itemCounter = 0;

            for(int j = 0; j < qr.xlsArr.size(); j++)
            {
                if(qr.xlsArr[j][qr.itemCol] == invoice.xlsArr[i][invoice.itemCol] && qty > 0)
                {
                    notFound = false;

                    qty--;
                    itemCounter ++;

                    entryResult tempResult;
                    tempResult.item = invoice.xlsArr[i][invoice.itemCol];
                    tempResult.itemCounter = std::to_string(itemCounter);
                    tempResult.itemNum = std::to_string(itemNum);

                    for(int k = 0; k < 31; k++)
                    {
                        std::string tempQr = qr.xlsArr[j][qr.qrCol];
                        tempResult.idQr += tempQr[k];
                    }

                    qr.xlsArr[j][qr.itemCol] = " ";
                    qr.xlsArr[j][qr.qrCol] = " ";

                    resultData.emplace_back(tempResult);
                }
//                if(j == qr.xlsArr.size() - 1)
//                {
//                    notFound = true;
//                    qty = 0;
//                    break;
//                }
            }
            if(notFound)
            {
                std::cout << "Not found " << qty << "/" << startQty << "\n";
                qty = 0;
                break;
            }
        }
        itemNum++;        
        std::cout << "Founded " << qty << "/" << startQty << "\n";
    }
}

void Result::writeResult(const char* path)
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

    for(int i = 0; i < resultData.size(); i++)
    {
        int row = i + 1;

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 0);
        cell->Set(i + 1);
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 1);
        cell->Set(&(resultData[i].itemNum[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 2);
        cell->Set(&(resultData[i].itemCounter[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 3);
        cell->Set(&(resultData[i].idQr[0]));
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 4);
        cell->Set("301");
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 5);
        cell->Set("0");
        cell->SetFormat(fmt);

        fmt.set_format_string(XLS_FORMAT_TEXT);
        cell = sheet->Cell(row, 6);
        cell->Set(&(resultData[i].item[0]));
        cell->SetFormat(fmt);
    }

    xls.SaveAs(path);
}


// class Invoice
Invoice::Invoice()
{

}

void Invoice::read(std::string path)
{
    readXls(xlsArr, path);
}

std::vector<entry> Invoice::getItems()
{
    return items;
}
// class Invoice

// class QR

QR::QR()
{

}

void QR::read(std::string path)
{
    readXls(xlsArr, path);
}

std::vector<entry> QR::getItems()
{
    return items;
}

void readXls(std::vector<std::vector<std::string>> &xlsArr, std::string path)
{
    BasicExcel xls(&(path[0]));

//    if(stopRow == 0) stopRow = sheet->GetTotalRows();
//    for(int tab = 0; tab < xls.GetTotalWorkSheets(); tab++)
//    {
        BasicExcelWorksheet* sheet = xls.GetWorksheet(0); // получение нужной страницы из файла

        xlsArr.resize(sheet->GetTotalRows());
        for(int row = 0; row < sheet->GetTotalRows(); row++)
        {
            xlsArr[row].resize(sheet->GetTotalCols());
            for(int col = 0; col < sheet->GetTotalCols(); col++)
            {
                std::stringstream tempStream;
                std::string tempString;
                tempStream << *(sheet->Cell(row, col));
                tempStream >> xlsArr[row][col];
            }
        }
//    }
}

bool ctnNumber(std::string str)
{
    if(str.size() == 11)
    {
        for(int i = 0; i < str.size(); i++)
        {

        }
        return true;
    }
    else return false;
}
