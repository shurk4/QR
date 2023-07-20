#ifndef QRCODES_H
#define QRCODES_H
#pragma once

#include "XlsFormat.h"
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>
#include <exception>
#include <QMessageBox>

class qrCodesException : public std::exception
{
    const char* ex;
    const char* what() const noexcept override
    {
        return ex;
    }

public:
    explicit qrCodesException(const char* text) : ex(text){}
};

using namespace ExcelFormat;

struct entry
{
    std::string item,
            qrOrQty;
};

struct entryResult
{
    std::string itemCounter,
                idQr,
                item,
                itemNum;
};

bool ctnNumber(std::string str);

void readXls(std::vector<std::vector<std::string>>& xlsArr, std::string path);

class Invoice
{
    std::vector<entry> items;

public:
    Invoice();

    int itemCol;
    int qtyCol;
    int startRow;
    int stopRow;

    std::vector<std::vector<std::string>> xlsArr;
    void read(std::string path);
    std::vector<entry> getItems();
};

class QR
{
    std::vector<entry> items;
public:
    QR();

    int itemCol;
    int qrCol;
    int startRow;
    int stopRow;

    std::vector<std::vector<std::string>> xlsArr;
    void read(std::string path);
    std::vector<entry> getItems();
};


class Result
{
    Invoice invoice;
    QR qr;

public:
    Result(QR& qr, Invoice& invoice);

    void getResult();
    void writeResult(const char* path);

    std::vector<entryResult> resultData;

};

#endif // QRCODES_H
