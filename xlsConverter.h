#ifndef XLSCONVERTER_H
#define XLSCONVERTER_H
#pragma once

#include "XlsFormat.h"
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>

using namespace ExcelFormat;

struct StructInvoice
{
    int itemCol = -1;
    int qtyCol = -1;
    int startRow = -1;
    int stopRow = -1;
};

struct StructQR
{
    int itemCol = -1;
    int qrCol = -1;
    int startRow = -1;
    int stopRow = -1;
};

class xlsConverter
{
public:
    std::vector<StructInvoice> invoiceSheetSettings;
    std::vector<std::vector<std::vector<std::string>>> invoiceXls;
    std::vector<std::vector<std::string>> invoiceResult;

    std::vector<StructQR> qrSheetSettings;
    std::vector<std::vector<std::vector<std::string>>> qrXls;
    std::vector<std::vector<std::string>> qrResult;

    std::vector<std::vector<std::string>> result;
    std::vector<std::vector<std::string>> resultInfo;

    void readXls(std::wstring path, std::vector<std::vector<std::vector<std::string>>> &tempXls);

    void clearInvoiceData();
    void clearQrData();

    void saveResult(std::wstring path);

    xlsConverter();
};

#endif // XLSCONVERTER_H
