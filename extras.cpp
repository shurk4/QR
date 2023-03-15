#include "extras.h"

Extras::Extras()
{

}

QVector<QVector<QString>> Extras::toQvecConvert(std::vector<std::vector<std::string>> &data)
{
//    QVector<QVector<QString>> qData(data.size());
    QVector<QVector<QString>> qData;

    for(int row = 0; row < data.size(); row++)
    {
//        QVector<QString> tempRow(data[row].size());
        QVector<QString> tempRow;

        for(int col = 0; col < data[row].size(); col++)
        {
            tempRow.push_back(QString::fromStdString(data[row][col]));
        }

        qData.push_back(tempRow);
    }

    return qData;
}

void Extras::showTable(QVector<QVector<QString> > &data, QTableWidget *table)
{
        table->clear();

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
}

std::string Extras::IntToSymbol(int in)
{
    std::string str;
        str += ('A' + in);
        return str;
}