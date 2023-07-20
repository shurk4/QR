#include "extras.h"

Extras::Extras()
{

}

QVector<QVector<QString>> Extras::toQvecConvert(const std::vector<std::vector<std::string>> &data)
{
    QVector<QVector<QString>> qData;

    for(int row = 0; row < data.size(); row++)
    {
        QVector<QString> tempRow;

        for(int col = 0; col < data[row].size(); col++)
        {
            tempRow.push_back(QString::fromStdString(data[row][col]));
        }

        qData.push_back(tempRow);
    }

    return qData;
}

void Extras::showTable(const QVector<QVector<QString>> &data, QTableWidget *table)
{
    table->clear();
    if(data.empty()) return;

    int rows;

    if(data.size() > 10000)
    {
        rows = 10000;
    }
    else
    {
        rows = data.size();
    }

    table->setRowCount(rows);
    table->setColumnCount(data[0].size());
    table->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G"); // Имена столбцов вместо цифр

    for(int row = 0; row < data.size(); row++)
    {
        if(data[row].size() > table->columnCount())
        {
            table->setColumnCount(data[row].size());
        }
        for(int col = 0; col < data[row].size(); col++)
        {
            QTableWidgetItem *tbl = new QTableWidgetItem(data[row][col]);

            table->setItem(row, col, tbl);
        }
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void Extras::scaleTable(const int size, QTableWidget *table)
{
    QFont font;
    font.setPointSize(size);

    table->setFont(font);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

std::string Extras::IntToSymbol(const int in)
{
    std::string str;
        str += ('A' + in);
        return str;
}

bool Extras::emptyCell(const QString &str)
{
    return str.size() == 0 || str[0] == '\0';
}
