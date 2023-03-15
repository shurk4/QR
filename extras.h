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
    static std::string IntToSymbol(int in);
};

#endif // EXTRAS_H
