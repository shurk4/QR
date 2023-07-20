#ifndef XLSCONVERTER_H
#define XLSCONVERTER_H
#pragma once

#include <QString>
#include <QVector>
#include <QTextStream>
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>
#include <iostream>

#include "XlsFormat.h"
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
    bool emptyCell(QString &str);

    std::vector<StructInvoice> invoiceSheetSettings;

    QVector<QVector<QVector<QString>>>  invoiceXls;
    QVector<QVector<QString>> invoiceResult;

    std::vector<StructQR> qrSheetSettings;
    QVector<QVector<QVector<QString>>> qrXls;
    QVector<QVector<QString>> qrResult;

    QVector<QString> invoiceSheetNames;
    QVector<QString> qrSheetNames;

    //Артикулы QR
    std::map<QString, int> qrArticules;
    QVector<QVector<QString>> qrArtuculesVec;
    // Подготовка Артикулов
    void calculateQrArticules();
    // Получить подсчитанные Арт.
    bool qrArtCalculated = false;
    void clearArt();

    QVector<QVector<QString>> result;
    QVector<QVector<QString>> resultInfo;

    void readXls(std::wstring path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames);
    void readXlsX(std::string path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames);

    void clearInvoiceData();
    void clearQrData();
    void clearResult();
    void clear();

    void saveResult(std::wstring path);
    void saveContainers(std::wstring patch);

    xlsConverter();
};

#endif // XLSCONVERTER_H
