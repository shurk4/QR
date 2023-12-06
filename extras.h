#ifndef EXTRAS_H
#define EXTRAS_H
#pragma once

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QRegExp>
#include <QDebug>

struct CellSettings
{
    int mergedRows = 0;
    int mergedCols = 0;
};

class Extras
{
public:
    Extras();

    static QVector<QVector<QString>> toQvecConvert(std::vector<std::vector<std::string>> const &data);

    static void showTable(const QVector<QVector<QString>> &data, QTableWidget *table);
    static void setTableSettings(QVector<QVector<CellSettings>> settings, QTableWidget *table);

    static void scaleTable(const int size, QTableWidget *table);

    static std::string IntToSymbol(const int in);

    static bool itQty(const std::string str);
    static bool isCtnNumber(QString str);
    static bool emptyCell(const QString &str);

    void readConfig();
};
#endif // EXTRAS_H
