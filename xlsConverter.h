#ifndef XLSCONVERTER_H
#define XLSCONVERTER_H
#pragma once

#include <QString>
#include <QVector>
#include <QDebug>
#include <QTextStream>
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>
#include <iostream>
#include <algorithm>

#include "XlsFormat.h"
#include "OpenXLSX/OpenXLSX.hpp"
#include "extras.h"

using namespace ExcelFormat;

// Структуры для нового хранения найденных QR кодов используется для сопоставления данных векторов qrInfo и qrCodes
enum DocType
{
    NONE,
    TypeINV,
    TypeQR
};

enum QrStatus
{
    NEW,
    ADDED,
    DELETED,
    EMPTY
};

struct QrInfo
{
    QString name;
    QrStatus status;
};

// Структуры выборки кодов
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
    int itemCol = -1;
};

struct StructQR : StructData
{
    int qrCol = -1;
};

class xlsConverter
{
public:
    xlsConverter();

    std::vector<StructInvoice> invoiceSheetSettings;

    QVector<QVector<QVector<QString>>>  invoiceXls;
    QVector<QString> invoiceSheetNames;

    QVector<QVector<QVector<QString>>> qrXls;
    QVector<QString> qrSheetNames;
    std::vector<StructQR> qrSheetSettings;
    QVector<QVector<QString>> qrResult;


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

    void readXls(std::wstring path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames, DocType type = NONE);
    void readXlsX(std::string path, QVector<QVector<QVector<QString>>> &tempXls, QVector<QString> &sheetNames);

    void clearInvoiceData();
    void clearQrData();
    void clearResult();
    void clearTxt();
    void clear();

    void saveResult(std::wstring path);
    void saveContainers(std::wstring patch);
    void saveTable(const QVector<QVector<QString>> &table, const std::wstring path);


    // Новые методы, переделать класс в нормальный вид

    int getItemsPosQty(); // Возвращает количество найденных ПОЗИЦИЙ ТОВАРОВ при выборке из инвойса/спец-ии
    int getCtnsQty(); // Возвращает количество КОНТЕЙНЕРОВ найденных при выборке из инвойса/спец-ии
    int getItemsQty(); // Возвращает общее количество ТОВАРОВ найденных при выборке из инвойса/спец-ии
    QVector<QVector<QString>> getInvoiceResult();

    bool invoiceEmpty();
    bool resultEmpty();
    QVector<QVector<QString>> getItemsBasic(int tab); // сделать выборку товаров для основного режима qrDialog
    QVector<QVector<QString>> getItemsForTxt(int tab); // сделать выборку товаров для текстового режима(txtfiles) (Порядок столбцов: ctn - 0, item - 1, qty - 2)

    // Новые методы
    bool emptyCell(QString &str);

    bool invoiceReady();

    void qrAnalyze(int tab);
    void addQrTxt(const QString name, const QVector<QVector<QString>> &codes);
    void clearQrTxt();
    void mergeItems(QVector<int> &selectedRows, QString &name);

    bool haveQrSettings(int tab);
    bool qrReady();
    bool qrItemIndex(QString itemName, int &num);

    int getQrQtyInItem(int item);
    int getQrItemsQty();
    QString getQrItemName(int item);
    QrStatus getQrStatus(int item);
    QVector<QVector<QString>> &getQrItem(int item);

    // Добавление кодов в результат
    void addQrInfo(QrInfo info);
    void updateQrStatus(int item);
    void setQrInfoStatus(int item, QrStatus status);

    // Работа с invoiceResult
    void addItem(int item);
    void clearInvoiceResult();
    void clearQrInfo();
    void addInvoiceResultRow(const QVector<QString> row);
    int undoResult(); // Отменить последний добавленный итем - возвращает номер позиции удаляемых из результата данных в QlistWidget
    int redoResult(); // Вернуть отменённый итем - возвращает номер позиции возвращаемых в результат данных в QlistWidget

    CellSettings getInvCellSettings(int page, int row, int col);
    CellSettings getQrCellSettings(int page, int row, int col);

    QVector<QVector<CellSettings>> getInvPageCellsSettings(int page);
    bool haveInvCellsSettings();
    QVector<QVector<CellSettings>> getQrPageCellsSettings(int page);
    bool haveQrCellsSettings();

    // Новые методы

    // подготовка к переносу в приват
    QVector<QVector<QString>> invoiceResult; // Результат выборки данных из инвойса(структура различается в зависимости от режима(qrDialog / txtFiles)

private:
    // Новые переменные. Перенести все переменные в приват и сделать для них методы

    // Данные полученные в процессе работы
    int itemsQty = 0; // Количество позиций товаров
    QVector<QString> ctnNumbers; // Список контейнеров в порядке указанном в инвойсе/спец-ии

    // Список контейнеров и итемов выбранных из текстовых файлов и таблиц отображаемый в списке listWidget
    QVector<QrInfo> qrInfo; // Содержит информацию по кодам найденным к указанному item коду QrInfo.name
    QVector<QVector<QVector<QString>>> qrCodes; // Содержит qr коды отсортированные по итемам, порядок соответствует qrInfo
    QList<int> addedItems;
    QList<int> undoItems;

    // Форматирование ячеек
    QVector<QVector<QVector<CellSettings>>> qrCellSettings;
    QVector<QVector<QVector<CellSettings>>> invCellSettings;
};

#endif // XLSCONVERTER_H
