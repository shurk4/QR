#ifndef XLSCONVERTER_H
#define XLSCONVERTER_H
#pragma once

#include "XlsFormat.h"
#include <QString>
#include <QVector>
#include <QTextStream>
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>
#include <iostream>
#include "OpenXLSX/OpenXLSX.hpp"

using namespace ExcelFormat;

struct StructData
{
    int itemCol = -1;
    int startRow = -1;
    int stopRow = -1;
    QString sheetName;
};

struct StructInvoice : StructData
{    
    int qtyCol = -1;
};

struct StructQR : StructData
{
    int qrCol = -1;
};

class xlsConverter
{
public:    
    bool emptyCell(std::string &str);

    std::vector<StructInvoice> invoiceSheetSettings;
    std::vector<std::vector<std::vector<std::string>>> invoiceXls;
    std::vector<std::vector<std::string>> invoiceResult;

    std::vector<StructQR> qrSheetSettings;
    std::vector<std::vector<std::vector<std::string>>> qrXls;
    std::vector<std::vector<std::string>> qrResult;

    QVector<QString> invoiceSheetNames;
    QVector<QString> qrSheetNames;

    //Артикулы QR
    std::map<std::string, int> qrArticules;
    std::vector<std::vector<std::string>> qrArtuculesVec;
    // Подготовка Артикулов
    void calculateQrArticules();
    // Получить подсчитанные Арт.
    bool qrArtCalculated = false;
    void clearArt();

    std::vector<std::vector<std::string>> result;
    std::vector<std::vector<std::string>> resultInfo;

    void readXls(std::wstring path, std::vector<std::vector<std::vector<std::string>>> &tempXls, QVector<QString> &sheetNames);
    void readXlsX(std::string path, std::vector<std::vector<std::vector<std::string>>> &tempXls, QVector<QString> &sheetNames);

    void clearInvoiceData();
    void clearQrData();
    void clearResult();
    void clear();

    void saveResult(std::wstring path);
    void saveContainers(std::wstring patch);

    xlsConverter();
};

#endif // XLSCONVERTER_H
