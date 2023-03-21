#ifndef EXTRAS_H
#define EXTRAS_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

class Extras
{
public:
    Extras();

    static QVector<QVector<QString>> toQvecConvert(std::vector<std::vector<std::string>> &data);

    static void showTable(QVector<QVector<QString>> &data, QTableWidget *table);
    static void scaleTable(int size, QTableWidget *table);

    static std::string IntToSymbol(int in);

    static bool emptyCell(QString &str);
};

#endif // EXTRAS_H
