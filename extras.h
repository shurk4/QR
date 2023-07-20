#ifndef EXTRAS_H
#define EXTRAS_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

class Extras
{
public:
    Extras();

    static QVector<QVector<QString>> toQvecConvert(std::vector<std::vector<std::string>> const &data);

    static void showTable(const QVector<QVector<QString>> &data, QTableWidget *table);
    static void scaleTable(const int size, QTableWidget *table);

    static std::string IntToSymbol(const int in);

    static bool emptyCell(const QString &str);
};

#endif // EXTRAS_H
